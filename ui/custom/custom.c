/*
* Copyright 2024 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and be bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include "lvgl.h"
#include "custom.h"
#include "system_data.h"
#include "history_ring.h"
#include "main.h"          /* HAL GPIO / LCD_BL 引脚宏定义 */

/* 引用在 lv_port_indev.c 中创建的全局焦点组 */
extern lv_group_t *g_keypad_group;

/*********************
 *      DEFINES
 *********************/

/* ========== Spotify Design System Color Palette ========== */
#define SPOTIFY_BG_DEEP         0x121212  /* Near Black - deepest background */
#define SPOTIFY_BG_SURFACE      0x181818  /* Dark Surface - cards, containers */
#define SPOTIFY_BG_INTERACTIVE  0x1f1f1f  /* Mid Dark - button backgrounds */
#define SPOTIFY_BG_CARD         0x282828  /* Slightly lighter card for contrast on dark */
#define SPOTIFY_GREEN           0x1ed760  /* Primary brand accent */
#define SPOTIFY_GREEN_BORDER    0x1db954  /* Green accent border variant */
#define SPOTIFY_TEXT_WHITE      0xffffff  /* Primary text */
#define SPOTIFY_TEXT_SILVER     0xb3b3b3  /* Secondary text, muted labels */

/*
 * Layout budget for 320x240 screen with GUI Guider's fixed positions:
 *
 *   Screen:        320 x 240
 *   Tabview:       323 x 240, pos(-2, 0)
 *   Tab Bar:       50px height (FIXED by GUI Guider)
 *   Content Area:  190px height per tab (240 - 50)
 *
 *   Tab 1 labels:     y=21 & y=105, h=64 each -> max_y=169  (<190 OK)
 *   Tab 2 switches:   y=40, h=27 -> max_y=67
 *   Tab 2 light lbl:  y=114, h=32 -> max_y=146
 *   Tab 2 slider:     y=139, h=4 -> max_y=143  (CRITICAL: only 4px!)
 *   Tab 3 spinboxes:  y=-8/48/104, h=40 -> max_y=144 (<190 OK)
 *
 * Style adaptation rules:
 *   1. NEVER change obj size/pos (set by GUI Guider)
 *   2. NEVER add border to tabview container (would shrink content area)
 *   3. Tab bar padding: keep original (LVGL default), only change colors
 *   4. Slider knob: NO min_height (original track is only 4px!)
 *   5. Spinbox padding: preserve original values (GUI Guider used pad=10)
 *   6. All radius values must be compatible with element heights
 */

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void spotify_apply_screen_style(lv_obj_t *screen);
static void spotify_apply_tabview_style(lv_obj_t *tabview);
static void spotify_apply_basic_tab(lv_ui *ui);
static void spotify_apply_control_tab(lv_ui *ui);
static void spotify_apply_threshold_tab(lv_ui *ui);
static void spotify_apply_label_style(lv_obj_t *label, const char *text);
static void setup_group_focus_style(void);
static void spotify_apply_focus_style(lv_obj_t *obj);
static void spotify_register_group_objects(lv_ui *ui);
static void spotify_init_threshold_spinboxes(lv_ui *ui);
static void ui_update_timer_cb(lv_timer_t *timer);
static void history_chart_create(lv_obj_t *parent);
static void history_chart_refresh(void);

/* ========== 新的焦点管理系统 ========== */
static void init_tab_focus(lv_ui *ui);
static void init_content_focus(lv_ui *ui);
static void refresh_tab_focus_visual(void);
static void refresh_content_focus_visual(void);
static void switch_to_tab(int tab_idx);
static void enter_content_level(void);
static void exit_content_level(void);
static void enter_edit_mode(void);
static void exit_edit_mode(void);
static void handle_content_level_keys(uint8_t key_code);
static void handle_edit_level_keys(uint8_t key_code);

/* ========== 按键定义 ========== */
/* 按键物理连接：
 *   KEY0 (PE4) = GPIO_PIN_RESET 按下 -> 退出键 (ESC/BACK)
 *   KEY1 (PE3) = GPIO_PIN_RESET 按下 -> 右移 / 增加
 *   KEY2 (PE2) = GPIO_PIN_RESET 按下 -> 左移 / 进入 / 操作 / 减少
 */
typedef enum {
    UI_HW_KEY_NONE = 0,
    UI_HW_KEY_LEFT,    /* KEY2: 左移 */
    UI_HW_KEY_RIGHT,   /* KEY1: 右移 */
    UI_HW_KEY_ENTER,   /* KEY2: 进入/操作 */
    UI_HW_KEY_ESC      /* KEY0: 退出 */
} ui_hw_key_t;

/* ========== 焦点层级枚举 ========== */
typedef enum {
    LEVEL_TAB = 0,      /* Tab级焦点：控制顶部Tab按钮左右移动 */
    LEVEL_CONTENT,      /* 栏内焦点：在Tab内容区域的控件间左右移动 */
    LEVEL_EDIT          /* 编辑模式：编辑Spinbox/Slider数值 */
} focus_level_t;

/* ========== 控件类型枚举 ========== */
typedef enum {
    CTRL_TYPE_SWITCH = 0,   /* 开关类型 */
    CTRL_TYPE_SLIDER,       /* 滑块类型 */
    CTRL_TYPE_SPINBOX       /* 数值输入类型 */
} ctrl_type_t;

/* ========== 栏内控件项定义 ========== */
typedef struct {
    lv_obj_t *obj;           /* 控件对象 */
    ctrl_type_t type;        /* 控件类型 */
    int8_t    col;            /* 所在列 (0, 1, 2...) */
    int8_t    row;            /* 所在行 (0, 1, 2...) */
} content_item_t;

/**********************
 *  STATIC VARIABLES
 **********************/

/* ========== 焦点系统变量 ========== */
static focus_level_t s_focus_level = LEVEL_TAB;   /* 当前焦点层级 */
static int s_tab_idx = 0;                          /* 当前Tab索引 (0=Basic, 1=Control, 2=Threshold) */
static int s_content_idx = 0;                      /* 栏内控件索引 */
static lv_obj_t *s_tab_btn_0 = NULL;              /* Tab按钮0 (Basic) */
static lv_obj_t *s_tab_btn_1 = NULL;              /* Tab按钮1 (Control) */
static lv_obj_t *s_tab_btn_2 = NULL;              /* Tab按钮2 (Threshold) */

/* ========== 栏内控件布局（列优先：第一列从上到下，再第二列...） ========== */
/* Control Tab: 第0列=[sw_fan, sw_pump], 第1列=[slider_light] */
static content_item_t s_control_items[] = {
    {NULL, CTRL_TYPE_SWITCH, 0, 0},   /* [0] sw_fan      */
    {NULL, CTRL_TYPE_SWITCH, 0, 1},   /* [1] sw_pump     */
    {NULL, CTRL_TYPE_SLIDER,  1, 0},   /* [2] slider_light */
};
static int s_control_count = 3;

/* Threshold Tab: 第0列=[spinbox_humi, spinbox_temp], 第1列=[spinbox_light] */
static content_item_t s_threshold_items[] = {
    {NULL, CTRL_TYPE_SPINBOX, 0, 0},   /* [0] spinbox_humi  */
    {NULL, CTRL_TYPE_SPINBOX, 0, 1},   /* [1] spinbox_temp  */
    {NULL, CTRL_TYPE_SPINBOX, 1, 0},   /* [2] spinbox_light */
};
static int s_threshold_count = 3;

/* ========== 获取当前Tab对应的栏内控件列表 ========== */
static content_item_t* get_current_content_list(void)
{
    if (s_tab_idx == 1) {
        return s_control_items;
    } else if (s_tab_idx == 2) {
        return s_threshold_items;
    }
    return NULL;
}

static int get_current_content_count(void)
{
    if (s_tab_idx == 1) {
        return s_control_count;
    } else if (s_tab_idx == 2) {
        return s_threshold_count;
    }
    return 0;
}

/**
 * Apply Spotify-inspired dark theme to the entire UI.
 * Called from custom_init() after GUI Guider sets up the base layout.
 *
 * Key principle: Only modify COLOR/FONT properties.
 * Never touch SIZE/PADDING/POSITION properties set by GUI Guider,
 * unless compensated precisely to stay within 320x240 budget.
 */
void custom_init(lv_ui *ui)
{
    if (!ui || !ui->screen) return;

    /* 1. Screen: deep black background */
    spotify_apply_screen_style(ui->screen);

    /* 2. Tabview: dark surface with Spotify green active indicator */
    spotify_apply_tabview_style(ui->screen_tabview_1);

    /* 3. Tab 1 "basic" — sensor data display */
    spotify_apply_basic_tab(ui);

    /* 4. Tab 2 "control" — switches + slider with green accent */
    spotify_apply_control_tab(ui);

    /* 5. Tab 3 "threshold" — spinboxes with dark input style */
    spotify_apply_threshold_tab(ui);

    /* 6. 用共享数据初始化阈值控件，避免界面和业务默认值脱节 */
    spotify_init_threshold_spinboxes(ui);

    /* 6.5 control 页底部历史曲线（history_ring → lv_chart） */
    if (ui->screen_tabview_1_tab_2) {
        history_chart_create(ui->screen_tabview_1_tab_2);
    }

    /* 7. 启动传感器数据刷新定时器（每 500ms 从全局字典读取并刷新屏幕） */
    lv_timer_create(ui_update_timer_cb, 500, NULL);

    /* 8. 注册真正可交互的控件到焦点组 */
    spotify_register_group_objects(ui);

    /* 8.5 配置 LVGL Group 焦点样式 */
    setup_group_focus_style();

    /* 9. 初始化焦点系统 */
    init_tab_focus(ui);          /* 初始化Tab级焦点 */
    init_content_focus(ui);      /* 初始化栏内控件引用 */
    switch_to_tab(0);            /* 默认显示Basic Tab */
}

/* ==================== Screen Style ==================== */
static void spotify_apply_screen_style(lv_obj_t *screen)
{
    lv_obj_set_style_bg_color(screen, lv_color_hex(SPOTIFY_BG_DEEP), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(screen, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/* ==================== Tabview Style ==================== */
static void spotify_apply_tabview_style(lv_obj_t *tabview)
{
    /*
     * Tabview: 323x240, pos(-2,0). NO border (saves 8px width).
     * Tab bar stays at original 50px height — no extra padding added.
     */
    lv_obj_set_style_bg_color(tabview, lv_color_hex(SPOTIFY_BG_DEEP), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(tabview, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);  /* No border! */
    lv_obj_set_style_radius(tabview, 15, LV_PART_MAIN | LV_STATE_DEFAULT);     /* Match original 15 */
    lv_obj_set_style_shadow_width(tabview, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* Tab bar: only color, no size changes */
    lv_obj_t *tab_bar = lv_tabview_get_tab_bar(tabview);
    if (tab_bar) {
        lv_obj_set_style_bg_color(tab_bar, lv_color_hex(SPOTIFY_BG_INTERACTIVE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(tab_bar, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(tab_bar, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    }

    /* Individual tab buttons: only text color/font, zero geometry impact */
    uint32_t tab_count = lv_tabview_get_tab_count(tabview);
    for (uint32_t i = 0; i < tab_count; i++) {
        lv_obj_t *btn = lv_obj_get_child(tab_bar, i);
        if (!btn) continue;

        /* Default state: silver text */
        lv_obj_set_style_text_color(btn, lv_color_hex(SPOTIFY_TEXT_SILVER), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(btn, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);

        /* Checked/active state: subtle green bg + green bottom border + white text */
        lv_obj_set_style_bg_opa(btn, 60, LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_bg_color(btn, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_border_width(btn, 2, LV_PART_MAIN | LV_STATE_CHECKED);     /* Thin 2px border */
        lv_obj_set_style_border_opa(btn, 255, LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_border_color(btn, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_border_side(btn, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_text_color(btn, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_MAIN | LV_STATE_CHECKED);
        lv_obj_set_style_text_opa(btn, 255, LV_PART_MAIN | LV_STATE_CHECKED);
    }

    /* Tab content pages: dark background only */
    for (uint32_t i = 0; i < tab_count; i++) {
        lv_obj_t *content = lv_tabview_get_content(tabview);
        if (content) {
            lv_obj_t *page = lv_obj_get_child(content, i);
            if (page) {
                lv_obj_set_style_bg_color(page, lv_color_hex(SPOTIFY_BG_DEEP), LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_bg_opa(page, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    }
}

/* ==================== Tab 1: Basic (Sensor Data) ==================== */
static void spotify_apply_basic_tab(lv_ui *ui)
{
    spotify_apply_label_style(ui->screen_label_humi, "HUMIDITY\n--.-%");
    spotify_apply_label_style(ui->screen_label_temp, "TEMP\n--.- C");
    spotify_apply_label_style(ui->screen_label_light, "LIGHT\n---- lx");
    spotify_apply_label_style(ui->screen_label_co2, "CO2\n---- ppm");
}

static void spotify_apply_label_style(lv_obj_t *label, const char *text)
{
    if (!label) return;
    lv_label_set_text(label, text);
    lv_obj_set_style_text_color(label, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserratMedium_16, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(label, 230, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(label, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/**
 * @brief 为控件应用焦点样式（禁用 LVGL 默认 outline）
 */
static void spotify_apply_focus_style(lv_obj_t *obj)
{
    if (!obj) return;

    /* 禁用 LVGL Group 默认的蓝色 outline 焦点框 */
    lv_obj_set_style_outline_width(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(obj, 0, LV_PART_MAIN | LV_STATE_FOCUSED);

    /* 清除其他可能的状态样式 */
    lv_obj_set_style_outline_width(obj, 0, LV_PART_MAIN | LV_STATE_EDITED);
    lv_obj_set_style_outline_opa(obj, 0, LV_PART_MAIN | LV_STATE_EDITED);
}

/**
 * @brief 初始化Tab按钮焦点引用
 */
static void init_tab_focus(lv_ui *ui)
{
    if (!ui || !ui->screen_tabview_1) return;

    /* 获取Tab按钮 */
    lv_obj_t *tab_bar = lv_tabview_get_tab_bar(ui->screen_tabview_1);
    if (tab_bar) {
        s_tab_btn_0 = lv_obj_get_child(tab_bar, 0);  /* Basic */
        s_tab_btn_1 = lv_obj_get_child(tab_bar, 1);  /* Control */
        s_tab_btn_2 = lv_obj_get_child(tab_bar, 2);  /* Threshold */
    }
}

/**
 * @brief 初始化栏内控件引用
 */
static void init_content_focus(lv_ui *ui)
{
    if (!ui) return;

    /* Control Tab 控件 */
    s_control_items[0].obj = ui->screen_sw_fan;
    s_control_items[1].obj = ui->screen_sw_pump;
    s_control_items[2].obj = ui->screen_slider_light;

    /* Threshold Tab 控件 */
    s_threshold_items[0].obj = ui->screen_spinbox_humi;
    s_threshold_items[1].obj = ui->screen_spinbox_temp;
    s_threshold_items[2].obj = ui->screen_spinbox_light;

    /* 禁用所有控件的默认焦点样式 */
    for (int i = 0; i < s_control_count; i++) {
        if (s_control_items[i].obj) {
            spotify_apply_focus_style(s_control_items[i].obj);
        }
    }
    for (int i = 0; i < s_threshold_count; i++) {
        if (s_threshold_items[i].obj) {
            spotify_apply_focus_style(s_threshold_items[i].obj);
        }
    }
}

/**
 * @brief 刷新Tab按钮的视觉焦点状态
 */
static void refresh_tab_focus_visual(void)
{
    /* 清除所有Tab按钮的焦点样式 */
    if (s_tab_btn_0) {
        lv_obj_set_style_bg_opa(s_tab_btn_0, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(s_tab_btn_0, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    }
    if (s_tab_btn_1) {
        lv_obj_set_style_bg_opa(s_tab_btn_1, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(s_tab_btn_1, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    }
    if (s_tab_btn_2) {
        lv_obj_set_style_bg_opa(s_tab_btn_2, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(s_tab_btn_2, 0, LV_PART_MAIN | LV_STATE_FOCUSED);
    }

    /* 高亮当前Tab按钮 */
    lv_obj_t *cur_tab_btn = NULL;
    switch (s_tab_idx) {
        case 0: cur_tab_btn = s_tab_btn_0; break;
        case 1: cur_tab_btn = s_tab_btn_1; break;
        case 2: cur_tab_btn = s_tab_btn_2; break;
    }

    if (cur_tab_btn) {
        /* Tab级焦点样式：绿色底部边框 */
        lv_obj_set_style_bg_opa(cur_tab_btn, 60, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_bg_color(cur_tab_btn, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_width(cur_tab_btn, 3, LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_color(cur_tab_btn, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_FOCUSED);
        lv_obj_set_style_border_side(cur_tab_btn, LV_BORDER_SIDE_BOTTOM, LV_PART_MAIN | LV_STATE_FOCUSED);
    }
}

/**
 * @brief 刷新栏内控件的视觉焦点状态
 */
static void refresh_content_focus_visual(void)
{
    content_item_t *items = get_current_content_list();
    int count = get_current_content_count();

    if (!items) return;

    /* 清除所有控件的焦点样式 */
    for (int i = 0; i < count; i++) {
        if (items[i].obj) {
            lv_obj_set_style_bg_opa(items[i].obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(items[i].obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }

    /* 高亮当前控件 */
    if (s_content_idx >= 0 && s_content_idx < count && items[s_content_idx].obj) {
        lv_obj_t *cur_obj = items[s_content_idx].obj;

        if (s_focus_level == LEVEL_EDIT) {
            /* 编辑模式：绿色半透明背景 + 绿色边框 */
            lv_obj_set_style_bg_color(cur_obj, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(cur_obj, 80, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(cur_obj, 3, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(cur_obj, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(cur_obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        } else {
            /* 栏内焦点：浅绿色边框 */
            lv_obj_set_style_border_width(cur_obj, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(cur_obj, lv_color_hex(SPOTIFY_GREEN_BORDER), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(cur_obj, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }

    lv_refr_now(NULL);
}

/**
 * @brief 切换到指定Tab
 */
static void switch_to_tab(int tab_idx)
{
    extern lv_ui guider_ui;

    if (tab_idx < 0) tab_idx = 2;
    if (tab_idx > 2) tab_idx = 0;

    s_tab_idx = tab_idx;

    /* 切换Tab显示 + 同步Tabview内部焦点 */
    if (guider_ui.screen_tabview_1) {
        lv_tabview_set_active(guider_ui.screen_tabview_1, tab_idx, LV_ANIM_OFF);
    }

    /* 刷新Tab焦点视觉 */
    refresh_tab_focus_visual();

    /* 重置栏内索引 */
    s_content_idx = 0;
}

/**
 * @brief 进入栏内焦点模式
 */
static void enter_content_level(void)
{
    s_focus_level = LEVEL_CONTENT;
    s_content_idx = 0;

    /* Basic Tab 没有可交互控件，直接切回Tab级 */
    if (s_tab_idx == 0) {
        s_focus_level = LEVEL_TAB;
        return;
    }

    /* 刷新栏内焦点视觉 */
    refresh_content_focus_visual();
}

/**
 * @brief 退出栏内焦点模式，回到Tab级
 */
static void exit_content_level(void)
{
    /* 清除栏内焦点样式 */
    content_item_t *items = get_current_content_list();
    int count = get_current_content_count();
    if (items) {
        for (int i = 0; i < count; i++) {
            if (items[i].obj) {
                lv_obj_set_style_bg_opa(items[i].obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_obj_set_style_border_width(items[i].obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            }
        }
    }

    s_focus_level = LEVEL_TAB;
    refresh_tab_focus_visual();
}

/**
 * @brief 进入数值编辑模式
 */
static void enter_edit_mode(void)
{
    content_item_t *items = get_current_content_list();
    int count = get_current_content_count();
    if (!items || s_content_idx < 0 || s_content_idx >= count) return;

    s_focus_level = LEVEL_EDIT;
    refresh_content_focus_visual();
}

/**
 * @brief 退出编辑模式，回到栏内焦点；spinbox 提交阈值到数据字典
 */
static void exit_edit_mode(void)
{
    if (s_focus_level == LEVEL_EDIT) {
        extern lv_ui guider_ui;
        if ((g_DataMutex != NULL) && (osMutexAcquire(g_DataMutex, osWaitForever) == osOK)) {
            if (guider_ui.screen_spinbox_temp) {
                g_SysData.thresh_temp = lv_spinbox_get_value(guider_ui.screen_spinbox_temp) / 1000.0f;
            }
            if (guider_ui.screen_spinbox_humi) {
                g_SysData.thresh_humi = lv_spinbox_get_value(guider_ui.screen_spinbox_humi) / 1000.0f;
            }
            if (guider_ui.screen_spinbox_light) {
                int32_t v = lv_spinbox_get_value(guider_ui.screen_spinbox_light);
                g_SysData.thresh_light = (uint32_t)((v < 0) ? 0 : (v / 1000));
            }
            osMutexRelease(g_DataMutex);
        }
    }
    s_focus_level = LEVEL_CONTENT;
    refresh_content_focus_visual();
}

/**
 * @brief 栏内焦点按键处理
 * - KEY_RIGHT (KEY1): 移动到下一个控件（右移/下移）
 * - KEY_LEFT (KEY2): 移动到上一个控件（左移/上移）
 * - KEY_ENTER (KEY2): 操作当前控件（开关切换/进入编辑模式）
 * - KEY_ESC (KEY0): 返回Tab级
 */
static void handle_content_level_keys(uint8_t key_code)
{
    content_item_t *items = get_current_content_list();
    int count = get_current_content_count();
    extern lv_ui guider_ui;

    if (!items || count == 0) {
        exit_content_level();
        return;
    }

    /* KEY_ESC: 直接退出到Tab级 */
    if (key_code == UI_HW_KEY_ESC) {
        exit_content_level();
        return;
    }

    /* KEY_ENTER: 操作当前控件 */
    if (key_code == UI_HW_KEY_ENTER) {
        content_item_t *cur = &items[s_content_idx];
        lv_obj_t *cur_obj = cur->obj;

        if (!cur_obj) return;

        /* 开关类型：直接切换状态 */
        if (cur->type == CTRL_TYPE_SWITCH) {
            bool checked = lv_obj_has_state(cur_obj, LV_STATE_CHECKED);
            if (checked) {
                lv_obj_remove_state(cur_obj, LV_STATE_CHECKED);
                if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
                    g_SysData.auto_mode = 0U;
                    if (cur_obj == guider_ui.screen_sw_fan)
                        g_SysData.fan_status = 0U;
                    else
                        g_SysData.pump_status = 0U;
                    osMutexRelease(g_DataMutex);
                }
            } else {
                lv_obj_add_state(cur_obj, LV_STATE_CHECKED);
                if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
                    g_SysData.auto_mode = 0U;
                    if (cur_obj == guider_ui.screen_sw_fan)
                        g_SysData.fan_status = 1U;
                    else
                        g_SysData.pump_status = 1U;
                    osMutexRelease(g_DataMutex);
                }
            }
            /* 开关操作完成，不退出编辑模式，保持在栏内焦点 */
            return;
        }

        /* 滑块/Spinbox类型：进入编辑模式 */
        if (cur->type == CTRL_TYPE_SLIDER || cur->type == CTRL_TYPE_SPINBOX) {
            enter_edit_mode();
            return;
        }
        return;
    }

    /* KEY_LEFT/KEY_RIGHT: 在控件间左右移动 */
    int direction = 0;
    if (key_code == UI_HW_KEY_RIGHT) {
        direction = 1;  /* 右移：下一个 */
    } else if (key_code == UI_HW_KEY_LEFT) {
        direction = -1; /* 左移：上一个 */
    }

    if (direction != 0) {
        content_item_t *cur = &items[s_content_idx];
        int8_t cur_col = cur->col;
        int8_t cur_row = cur->row;
        int next_idx = -1;

        if (direction > 0) {
            /* 右移：先在同列找下一行，再找下一列的第一行 */
            /* 先找同列的下一行 */
            for (int i = s_content_idx + 1; i < count; i++) {
                if (items[i].col == cur_col && items[i].row > cur_row) {
                    next_idx = i;
                    break;
                }
            }
            /* 没找到则找下一列的第一行 */
            if (next_idx < 0) {
                for (int i = 0; i < count; i++) {
                    if (items[i].col > cur_col) {
                        next_idx = i;
                        break;
                    }
                }
            }
            /* 循环回第一列 */
            if (next_idx < 0) {
                for (int i = 0; i < count; i++) {
                    if (items[i].col < cur_col) {
                        next_idx = i;
                    }
                }
            }
        } else {
            /* 左移：先在同列找上一行，再找上一列的最后一行 */
            /* 先找同列的上一行 */
            for (int i = s_content_idx - 1; i >= 0; i--) {
                if (items[i].col == cur_col && items[i].row < cur_row) {
                    next_idx = i;
                    break;
                }
            }
            /* 没找到则找上一列的最后一行 */
            if (next_idx < 0) {
                for (int i = count - 1; i >= 0; i--) {
                    if (items[i].col < cur_col) {
                        next_idx = i;
                        break;
                    }
                }
            }
            /* 循环回最后一列 */
            if (next_idx < 0) {
                for (int i = count - 1; i >= 0; i--) {
                    if (items[i].col > cur_col) {
                        next_idx = i;
                    }
                }
            }
        }

        if (next_idx >= 0 && next_idx < count) {
            s_content_idx = next_idx;
            refresh_content_focus_visual();
        }
    }
}

/**
 * @brief 编辑模式按键处理
 * - KEY_RIGHT (KEY1): 数值增加
 * - KEY_LEFT (KEY2): 数值减少
 * - KEY_ENTER (KEY2): 退出滑块编辑模式
 * - KEY_ESC (KEY0): 退出Spinbox编辑模式，返回栏内焦点
 */
static void handle_edit_level_keys(uint8_t key_code)
{
    content_item_t *items = get_current_content_list();
    int count = get_current_content_count();
    if (!items || s_content_idx < 0 || s_content_idx >= count) {
        exit_edit_mode();
        return;
    }

    content_item_t *cur = &items[s_content_idx];
    lv_obj_t *cur_obj = cur->obj;

    if (!cur_obj) {
        exit_edit_mode();
        return;
    }

    /* KEY_ESC: 退出编辑，返回栏内焦点 */
    if (key_code == UI_HW_KEY_ESC) {
        exit_edit_mode();
        return;
    }

    /* 根据控件类型处理 */
    switch (cur->type) {
        case CTRL_TYPE_SLIDER: {
            /* 滑块编辑模式 */
            if (key_code == UI_HW_KEY_ENTER) {
                /* KEY2: 退出滑块编辑，返回栏内焦点 */
                exit_edit_mode();
            } else if (key_code == UI_HW_KEY_RIGHT) {
                /* KEY1: 滑块值增加 */
                int32_t val = lv_slider_get_value(cur_obj);
                int32_t new_val = val + 10000;  /* 10% 步进 */
                if (new_val > 100000) new_val = 100000;
                lv_slider_set_value(cur_obj, new_val, LV_ANIM_OFF);
                if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
                    g_SysData.auto_mode = 0; /* 手动调光切手动，避免被自动覆盖 */
                    g_SysData.light_pwm = (uint16_t)(new_val / 1000);
                    osMutexRelease(g_DataMutex);
                }
            } else if (key_code == UI_HW_KEY_LEFT) {
                /* KEY2: 滑块值减少 */
                int32_t val = lv_slider_get_value(cur_obj);
                int32_t new_val = val - 10000;  /* 10% 步进 */
                if (new_val < 0) new_val = 0;
                lv_slider_set_value(cur_obj, new_val, LV_ANIM_OFF);
                if (osMutexAcquire(g_DataMutex, osWaitForever) == osOK) {
                    g_SysData.auto_mode = 0;
                    g_SysData.light_pwm = (uint16_t)(new_val / 1000);
                    osMutexRelease(g_DataMutex);
                }
            }
            break;
        }

        case CTRL_TYPE_SPINBOX: {
            /* Spinbox编辑模式 */
            if (key_code == UI_HW_KEY_RIGHT) {
                /* KEY1: 数值增加 */
                int32_t val = lv_spinbox_get_value(cur_obj);
                int32_t new_val = val + 1000;
                if (new_val > 999999) new_val = 999999;
                lv_spinbox_set_value(cur_obj, new_val);
                lv_spinbox_increment(cur_obj);
            } else if (key_code == UI_HW_KEY_LEFT) {
                /* KEY2: 数值减少 */
                int32_t val = lv_spinbox_get_value(cur_obj);
                int32_t new_val = val - 1000;
                if (new_val < -99999) new_val = -99999;
                lv_spinbox_set_value(cur_obj, new_val);
                lv_spinbox_decrement(cur_obj);
            }
            /* Spinbox编辑模式下KEY_ENTER无操作，保持编辑状态 */
            break;
        }

        default:
            break;
    }
}

static void spotify_init_threshold_spinboxes(lv_ui *ui)
{
    int32_t humi_threshold = 40000;
    int32_t temp_threshold = 30000;
    int32_t light_threshold = 50000;

    if (!ui) return;

    if ((g_DataMutex != NULL) && (osMutexAcquire(g_DataMutex, osWaitForever) == osOK)) {
        humi_threshold = (int32_t)(g_SysData.thresh_humi * 1000.0f);
        temp_threshold = (int32_t)(g_SysData.thresh_temp * 1000.0f);
        light_threshold = (int32_t)(g_SysData.thresh_light * 1000U);
        osMutexRelease(g_DataMutex);
    }

    lv_spinbox_set_step(ui->screen_spinbox_humi, 1000);
    lv_spinbox_set_step(ui->screen_spinbox_temp, 1000);
    lv_spinbox_set_step(ui->screen_spinbox_light, 1000);
    lv_spinbox_set_value(ui->screen_spinbox_humi, humi_threshold);
    lv_spinbox_set_value(ui->screen_spinbox_temp, temp_threshold);
    lv_spinbox_set_value(ui->screen_spinbox_light, light_threshold);
}

/**
 * @brief 注册控件到焦点组（用于禁用 LVGL Group 焦点样式）
 */
static void spotify_register_group_objects(lv_ui *ui)
{
    uint32_t i;
    lv_obj_t *controls[6];

    if (!ui) return;

    controls[0] = ui->screen_sw_fan;
    controls[1] = ui->screen_sw_pump;
    controls[2] = ui->screen_slider_light;
    controls[3] = ui->screen_spinbox_humi;
    controls[4] = ui->screen_spinbox_temp;
    controls[5] = ui->screen_spinbox_light;

    for (i = 0; i < 6; i++) {
        if (!controls[i]) continue;
        /* 禁用 LVGL Group 焦点样式，使用手动焦点系统 */
        spotify_apply_focus_style(controls[i]);
    }

    /* 诊断：LED常亮表示初始化成功 */
    HAL_GPIO_WritePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin, GPIO_PIN_SET);
}

/* ==================== Tab 2: Control (Switches + Slider) ==================== */

/* LVGL Group 焦点样式配置 */
static void setup_group_focus_style(void)
{
    if (!g_keypad_group) return;
    lv_group_set_default(g_keypad_group);
}
static void spotify_apply_control_tab(lv_ui *ui)
{
    /*
     * Switches: 96x27, radius adapted to 13 (pill-like but fits 27px height).
     * Knob: circle, auto-sized by LVGL to fit switch height.
     */

    /* --- Fan Switch --- */
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(SPOTIFY_BG_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_fan, 13, LV_PART_MAIN | LV_STATE_DEFAULT);  /* Pill: 27/2≈13 */
    lv_obj_set_style_border_width(ui->screen_sw_fan, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* Checked indicator: Spotify Green */
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(SPOTIFY_GREEN), LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);

    /* Knob: white circle */
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_fan, LV_RADIUS_CIRCLE, LV_PART_KNOB | LV_STATE_DEFAULT);
    
    /* Fan Switch ON 状态: 整个开关变绿色 */
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    /* --- Pump Switch --- */
    /* OFF 状态: 深灰色背景 */
    lv_obj_set_style_bg_color(ui->screen_sw_pump, lv_color_hex(SPOTIFY_BG_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sw_pump, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_pump, 13, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sw_pump, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /* ON 状态: 整个开关变绿色 */
    lv_obj_set_style_bg_color(ui->screen_sw_pump, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_CHECKED);
    lv_obj_set_style_bg_opa(ui->screen_sw_pump, 255, LV_PART_MAIN | LV_STATE_CHECKED);

    lv_obj_set_style_bg_color(ui->screen_sw_pump, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_sw_pump, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_pump, LV_RADIUS_CIRCLE, LV_PART_KNOB | LV_STATE_DEFAULT);

    /* --- Control Labels --- */
    lv_label_set_text(ui->screen_Lable_lightcontrol, "BRIGHTNESS");
    lv_obj_set_style_text_color(ui->screen_Lable_lightcontrol, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Lable_lightcontrol, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Lable_lightcontrol, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_label_set_text(ui->screen_Lable_fancontrol, "FAN");
    lv_obj_set_style_text_color(ui->screen_Lable_fancontrol, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Lable_fancontrol, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Lable_fancontrol, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_label_set_text(ui->screen_label_pumpcontrol, "PUMP");
    lv_obj_set_style_text_color(ui->screen_label_pumpcontrol, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_pumpcontrol, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_pumpcontrol, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    /*
     * Brightness Slider: 增大高度到 24px，便于显示和操作
     * - Track: 深灰色圆角背景
     * - Indicator: Spotify Green
     * - Knob: 白色圆点 24x24
     */
    lv_obj_set_style_height(ui->screen_slider_light, 24, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_light, lv_color_hex(SPOTIFY_BG_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_slider_light, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_light, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(ui->screen_slider_light, lv_color_hex(SPOTIFY_GREEN), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_slider_light, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_light, 12, LV_PART_INDICATOR | LV_STATE_DEFAULT);

    /* 滑块旋钮: 24x24 白色圆点 */
    lv_obj_set_style_width(ui->screen_slider_light, 24, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_height(ui->screen_slider_light, 24, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_light, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_slider_light, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_light, LV_RADIUS_CIRCLE, LV_PART_KNOB | LV_STATE_DEFAULT);
}

/* ==================== Tab 3: Threshold (Spinboxes) ==================== */
static void spotify_apply_threshold_tab(lv_ui *ui)
{
    /*
     * Spinbox: 70x40 (set by GUI Guider).
     * Original: border=2, pad=10, radius=5
     * Adaptation: reduce border 2→1 (saves 2px total), keep pad=10, keep radius=5
     * +/- Buttons: 40x40 (same as spinbox height) — circular green buttons
     */
    struct { lv_obj_t *spinbox; lv_obj_t *btn_plus; lv_obj_t *btn_minus; } items[] = {
        { ui->screen_spinbox_humi, ui->screen_spinbox_humi_btn_plus, ui->screen_spinbox_humi_btn_minus },
        { ui->screen_spinbox_temp, ui->screen_spinbox_temp_btn_plus, ui->screen_spinbox_temp_btn_minus },
        { ui->screen_spinbox_light, ui->screen_spinbox_light_btn_plus, ui->screen_spinbox_light_btn_minus },
    };

    for (int i = 0; i < 3; i++) {
        lv_obj_t *sb = items[i].spinbox;
        lv_obj_t *bp = items[i].btn_plus;
        lv_obj_t *bm = items[i].btn_minus;
        if (!sb) continue;

        /* Spinbox main: dark interactive bg */
        lv_obj_set_style_bg_color(sb, lv_color_hex(SPOTIFY_BG_INTERACTIVE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(sb, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        /* Border: 1px instead of original 2px (saves 2px total, keeps clean look) */
        lv_obj_set_style_border_color(sb, lv_color_hex(SPOTIFY_BG_CARD), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_opa(sb, 180, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(sb, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_side(sb, LV_BORDER_SIDE_FULL, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(sb, 5, LV_PART_MAIN | LV_STATE_DEFAULT);           /* Match original */
        /* Preserve original padding (10px) — GUI Guider laid out with these values */
        lv_obj_set_style_pad_top(sb, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(sb, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_left(sb, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(sb, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_shadow_width(sb, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

        /* Text: white on dark */
        lv_obj_set_style_text_color(sb, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_font(sb, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_text_opa(sb, 240, LV_PART_MAIN | LV_STATE_DEFAULT);

        /* Cursor selection: Spotify Green highlight */
        lv_obj_set_style_text_color(sb, lv_color_hex(SPOTIFY_TEXT_WHITE), LV_PART_CURSOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(sb, lv_color_hex(SPOTIFY_GREEN), LV_PART_CURSOR | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(sb, 200, LV_PART_CURSOR | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(sb, 4, LV_PART_CURSOR | LV_STATE_DEFAULT);

        /* +/- Buttons: 40x40 circular green buttons */
        if (bp) {
            lv_obj_set_style_bg_color(bp, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(bp, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(bp, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);  /* Circle */
            lv_obj_set_style_border_width(bp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(bp, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(bp, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(bp, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        }

        if (bm) {
            lv_obj_set_style_bg_color(bm, lv_color_hex(SPOTIFY_GREEN), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(bm, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(bm, LV_RADIUS_CIRCLE, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(bm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(bm, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_color(bm, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(bm, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }

    /* Threshold labels */
    lv_label_set_text(ui->screen_label_3, "HUMIDITY");
    lv_obj_set_style_text_color(ui->screen_label_3, lv_color_hex(SPOTIFY_TEXT_SILVER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_3, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_label_set_text(ui->screen_label_2, "TEMP");
    lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(SPOTIFY_TEXT_SILVER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_2, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_label_set_text(ui->screen_label_4, "LIGHTNESS");
    lv_obj_set_style_text_color(ui->screen_label_4, lv_color_hex(SPOTIFY_TEXT_SILVER), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_4, &lv_font_montserratMedium_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_4, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

/**
 * @brief 按键扫描（硬件相关）
 *
 * 按键映射：
 * - KEY0 (PE4) = 退出键 (ESC)
 * - KEY1 (PE3) = 右移 / 增加
 * - KEY2 (PE2) = 左移 / 进入 / 减少
 */
static uint8_t ui_keypad_scan(void)
{
    if (HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET) {
        return UI_HW_KEY_ESC;
    }
    if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET) {
        return UI_HW_KEY_RIGHT;
    }
    if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == GPIO_PIN_RESET) {
        return UI_HW_KEY_LEFT;
    }

    return UI_HW_KEY_NONE;
}

/* ==================== 按键处理入口（三层焦点系统） ==================== */

/**
 * @brief 按键处理（去抖 + 三层焦点分发）
 *
 * 按键映射：
 * - KEY0: 退出键 (ESC)
 * - KEY1: 右移 / 增加
 * - KEY2: 左移 / 进入 / 减少
 *
 * 三层焦点系统：
 * 1. LEVEL_TAB    - Tab级焦点：控制顶部Tab按钮左右移动
 *    - KEY2: Tab左移
 *    - KEY1: Tab右移
 *    - KEY2(再次): 进入栏内焦点
 *
 * 2. LEVEL_CONTENT - 栏内焦点：在Tab内容区域控件间移动
 *    - KEY1: 移动到下一个控件（右移）
 *    - KEY2: 移动到上一个控件（左移）
 *    - KEY2(再次): 操作控件（开关切换/进入编辑）
 *    - KEY0: 返回Tab级焦点
 *
 * 3. LEVEL_EDIT   - 编辑模式：编辑Spinbox/Slider数值
 *    - KEY1: 数值增加
 *    - KEY2: 数值减少
 *    - KEY2(滑块): 退出滑块编辑，返回栏内焦点
 *    - KEY0(Spinbox): 退出数值编辑，返回栏内焦点
 */
void custom_ui_process_keys(void)
{
    static uint8_t stable_key = UI_HW_KEY_NONE;
    static uint8_t last_sample = UI_HW_KEY_NONE;
    static uint32_t debounce_tick = 0U;
    static uint32_t repeat_tick = 0U;
    static uint32_t press_start_tick = 0U;
    static uint8_t last_confirmed_key = UI_HW_KEY_NONE;  /* 记录上次确认的键 */
    uint8_t sample;
    uint32_t now;

    sample = ui_keypad_scan();
    now = HAL_GetTick();

    /* 去抖：采样值变化时重置计数器 */
    if (sample != last_sample) {
        last_sample = sample;
        debounce_tick = now;
        if (sample == UI_HW_KEY_NONE) {
            stable_key = UI_HW_KEY_NONE;
            last_confirmed_key = UI_HW_KEY_NONE;  /* 按键释放时重置 */
        }
        return;
    }

    /* 无按键 */
    if (sample == UI_HW_KEY_NONE) {
        return;
    }

    /* 按键稳定 20ms 后确认按下 */
    if ((stable_key != sample) && ((now - debounce_tick) >= 20U)) {
        stable_key = sample;
        press_start_tick = now;
        repeat_tick = now;

        /* LED 短暂闪烁确认按键被识别（不阻塞） */
        static uint32_t diag_tick = 0;
        if ((now - diag_tick) > 200) {
            HAL_GPIO_TogglePin(DEBUG_LED_GPIO_Port, DEBUG_LED_Pin);
            diag_tick = now;
        }

        /* 分发到对应层级的处理函数 */
        switch (s_focus_level) {
            case LEVEL_TAB:
                /* Tab级：KEY1用于右移，KEY2用于左移或进入栏内 */
                if (sample == UI_HW_KEY_RIGHT) {
                    /* KEY1: 右移Tab */
                    switch_to_tab(s_tab_idx + 1);
                } else if (sample == UI_HW_KEY_LEFT) {
                    /* KEY2: 区分首次按下和再次按下 */
                    if (last_confirmed_key == UI_HW_KEY_LEFT) {
                        /* 再次按下KEY2: 进入栏内焦点 */
                        enter_content_level();
                    } else {
                        /* 首次按下KEY2: 左移Tab */
                        switch_to_tab(s_tab_idx - 1);
                    }
                } else if (sample == UI_HW_KEY_ESC) {
                    /* KEY0: 回到Basic Tab */
                    switch_to_tab(0);
                }
                last_confirmed_key = sample;
                break;

            case LEVEL_CONTENT:
                /* 栏内焦点 */
                if (sample == UI_HW_KEY_ESC) {
                    /* KEY0: 返回Tab级 */
                    exit_content_level();
                } else {
                    /* KEY1/KEY2: 移动或操作 */
                    handle_content_level_keys(sample);
                }
                last_confirmed_key = sample;
                break;

            case LEVEL_EDIT:
                /* 编辑模式 */
                handle_edit_level_keys(sample);
                last_confirmed_key = sample;
                break;
        }
        return;
    }

    /* 长按自动重复（编辑模式下支持，250ms后开始，150ms间隔） */
    if ((stable_key == sample) &&
        (s_focus_level == LEVEL_EDIT) &&
        ((sample == UI_HW_KEY_RIGHT) || (sample == UI_HW_KEY_LEFT)) &&
        ((now - press_start_tick) >= 250U) &&
        ((now - repeat_tick) >= 150U)) {
        repeat_tick = now;
        handle_edit_level_keys(sample);
    }
}

/* ==================== 历史曲线（history_ring → lv_chart） ==================== */
#define HIST_CHART_POINTS  32
static lv_obj_t *s_hist_chart;
static lv_chart_series_t *s_series_temp;
static lv_chart_series_t *s_series_humi;
static int32_t s_chart_temp[HIST_CHART_POINTS];
static int32_t s_chart_humi[HIST_CHART_POINTS];

static void history_chart_create(lv_obj_t *parent)
{
    if (!parent || s_hist_chart) return;

    s_hist_chart = lv_chart_create(parent);
    lv_obj_set_pos(s_hist_chart, 8, 148);
    lv_obj_set_size(s_hist_chart, 304, 38);
    lv_chart_set_type(s_hist_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_point_count(s_hist_chart, HIST_CHART_POINTS);
    lv_chart_set_range(s_hist_chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    lv_obj_set_style_bg_color(s_hist_chart, lv_color_hex(SPOTIFY_BG_SURFACE), LV_PART_MAIN);
    lv_obj_set_style_bg_opa(s_hist_chart, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_border_width(s_hist_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(s_hist_chart, 2, LV_PART_MAIN);

    s_series_temp = lv_chart_add_series(s_hist_chart, lv_color_hex(SPOTIFY_GREEN), LV_CHART_AXIS_PRIMARY_Y);
    s_series_humi = lv_chart_add_series(s_hist_chart, lv_color_hex(0x4fc3f7), LV_CHART_AXIS_PRIMARY_Y);
    lv_chart_set_series_ext_y_array(s_hist_chart, s_series_temp, s_chart_temp);
    lv_chart_set_series_ext_y_array(s_hist_chart, s_series_humi, s_chart_humi);
}

static void history_chart_refresh(void)
{
    if (!s_hist_chart || !s_series_temp || !s_series_humi) return;

    history_sample_t samples[HIST_CHART_POINTS];
    uint32_t n = history_ring_snapshot(samples, HIST_CHART_POINTS);
    if (n == 0) return;

    /* 对齐到数组尾部（最新在右） */
    uint32_t pad = HIST_CHART_POINTS - n;
    for (uint32_t i = 0; i < HIST_CHART_POINTS; i++) {
        if (i < pad) {
            s_chart_temp[i] = 0;
            s_chart_humi[i] = 0;
        } else {
            const history_sample_t *s = &samples[i - pad];
            float t = s->temp;
            float h = s->humi;
            if (t < 0) t = 0;
            if (t > 100) t = 100;
            if (h < 0) h = 0;
            if (h > 100) h = 100;
            s_chart_temp[i] = (int32_t)t;
            s_chart_humi[i] = (int32_t)h;
        }
    }
    lv_chart_refresh(s_hist_chart);
}

/* ==================== LVGL 数据刷新定时器 ==================== */

/**
 * @brief 每 500ms 被自动调用，从全局字典读取传感器数据并更新屏幕显示
 *
 * 此回调运行在 GUI Task 的上下文中（由 lv_timer_handler 驱动），
 * 因此调用所有 lv_ 前缀 API 都是线程安全的。
 */
static void ui_update_data(void);  /* 数据刷新函数声明 */

/**
 * @brief 每 500ms 被自动调用，从全局字典读取传感器数据并更新屏幕显示
 *
 * 此回调运行在 GUI Task 的上下文中（由 lv_timer_handler 驱动），
 * 因此调用所有 lv_ 前缀 API 都是线程安全的。
 */
static void ui_update_timer_cb(lv_timer_t *timer)
{
    (void)timer;
    ui_update_data();
    /* 每 2s 刷一次曲线，避免 500ms 全量 refresh 过重 */
    {
        static uint32_t tick;
        if ((++tick % 4) == 0) {
            history_chart_refresh();
        }
    }
}

/**
 * @brief 数据刷新核心逻辑（可被定时器回调或主循环直接调用）
 */
static void ui_update_data(void)
{
    extern lv_ui guider_ui;
    SystemData_t snapshot = {
        .temp = 25.0f,
        .humi = 50.0f,
        .light = 500U,
        .co2 = 400.0f,
        .thresh_temp = 30.0f,
        .thresh_humi = 40.0f,
        .thresh_light = 50U,
        .fan_status = 0U,
        .pump_status = 0U,
        .light_pwm = 0U,
        .auto_mode = 1U,
    };
    char buf[64];
    int32_t humi_x10;
    int32_t temp_x10;
    int32_t co2_x10;

    /* 仅读字典：阈值由编辑提交/云端命令写入，避免 100ms 回写冲掉云指令 */
    if ((g_DataMutex != NULL) && (osMutexAcquire(g_DataMutex, osWaitForever) == osOK)) {
        snapshot = g_SysData;
        osMutexRelease(g_DataMutex);
    } else {
        snapshot = g_SysData;
    }

    humi_x10 = (int32_t)(snapshot.humi * 10.0f + ((snapshot.humi >= 0.0f) ? 0.5f : -0.5f));
    temp_x10 = (int32_t)(snapshot.temp * 10.0f + ((snapshot.temp >= 0.0f) ? 0.5f : -0.5f));
    co2_x10 = (int32_t)(snapshot.co2 * 10.0f + ((snapshot.co2 >= 0.0f) ? 0.5f : -0.5f));

    snprintf(buf, sizeof(buf), "HUMIDITY\n%ld.%ld %%", (long)(humi_x10 / 10), (long)labs(humi_x10 % 10));
    lv_label_set_text(guider_ui.screen_label_humi, buf);

    /* TEMP 行附带模式徽标；LIGHT 行附带云状态；CO2 行附带报警 —— 不碰 tab3 字段标题 label_2/3/4 */
    {
        static const char *cloud_sfx[] = {" [OFF]", " [...]", " [ON]", " [WAIT]"};
        uint8_t cs = snapshot.cloud_state;
        if (cs > 3) cs = 0;
        snprintf(buf, sizeof(buf), "TEMP\n%ld.%ld C %s",
                 (long)(temp_x10 / 10), (long)labs(temp_x10 % 10),
                 snapshot.auto_mode ? "A" : "M");
        lv_label_set_text(guider_ui.screen_label_temp, buf);

        snprintf(buf, sizeof(buf), "LIGHT\n%lu lx%s", (unsigned long)snapshot.light, cloud_sfx[cs]);
        lv_label_set_text(guider_ui.screen_label_light, buf);

        snprintf(buf, sizeof(buf), "CO2\n%ld.%ld ppm%s",
                 (long)(co2_x10 / 10), (long)labs(co2_x10 % 10),
                 snapshot.alarm_active ? " !" : "");
        lv_label_set_text(guider_ui.screen_label_co2, buf);
    }

    if (guider_ui.screen_sw_fan) {
        bool checked = lv_obj_has_state(guider_ui.screen_sw_fan, LV_STATE_CHECKED);
        if (snapshot.fan_status && !checked) {
            lv_obj_add_state(guider_ui.screen_sw_fan, LV_STATE_CHECKED);
        }
        else if (!snapshot.fan_status && checked) {
            lv_obj_remove_state(guider_ui.screen_sw_fan, LV_STATE_CHECKED);
        }
    }

    if (guider_ui.screen_sw_pump) {
        bool checked = lv_obj_has_state(guider_ui.screen_sw_pump, LV_STATE_CHECKED);
        if (snapshot.pump_status && !checked) {
            lv_obj_add_state(guider_ui.screen_sw_pump, LV_STATE_CHECKED);
        }
        else if (!snapshot.pump_status && checked) {
            lv_obj_remove_state(guider_ui.screen_sw_pump, LV_STATE_CHECKED);
        }
    }

    if (guider_ui.screen_slider_light) {
        if (lv_slider_get_value(guider_ui.screen_slider_light) != snapshot.light_pwm) {
            lv_slider_set_value(guider_ui.screen_slider_light, snapshot.light_pwm, LV_ANIM_OFF);
        }
    }

    /* 字典阈值 → spinbox 同步（云/本地写入后回显；编辑中不抢焦点值） */
    if (s_focus_level != LEVEL_EDIT) {
        if (guider_ui.screen_spinbox_temp) {
            int32_t want = (int32_t)(snapshot.thresh_temp * 1000.0f + 0.5f);
            if (lv_spinbox_get_value(guider_ui.screen_spinbox_temp) != want) {
                lv_spinbox_set_value(guider_ui.screen_spinbox_temp, want);
            }
        }
        if (guider_ui.screen_spinbox_humi) {
            int32_t want = (int32_t)(snapshot.thresh_humi * 1000.0f + 0.5f);
            if (lv_spinbox_get_value(guider_ui.screen_spinbox_humi) != want) {
                lv_spinbox_set_value(guider_ui.screen_spinbox_humi, want);
            }
        }
        if (guider_ui.screen_spinbox_light) {
            int32_t want = (int32_t)(snapshot.thresh_light * 1000);
            if (lv_spinbox_get_value(guider_ui.screen_spinbox_light) != want) {
                lv_spinbox_set_value(guider_ui.screen_spinbox_light, want);
            }
        }
    }

    /* 云/模式/报警状态已并入 TEMP/LIGHT/CO2 数据行；不覆写 tab3 的 label_2/3/4 字段标题 */
}

/* 导出给 guiTask 主循环直接调用的包装函数（非 static，可被外部链接） */
void custom_ui_update_data(void)
{
    ui_update_data();
}
