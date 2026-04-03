/*
* Copyright 2026 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen(lv_ui *ui)
{
    //Write codes screen
    ui->screen = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_tabview_1
    ui->screen_tabview_1 = lv_tabview_create(ui->screen);
    lv_obj_set_pos(ui->screen_tabview_1, -2, 0);
    lv_obj_set_size(ui->screen_tabview_1, 323, 240);
    lv_obj_set_scrollbar_mode(ui->screen_tabview_1, LV_SCROLLBAR_MODE_OFF);
    lv_tabview_set_tab_bar_position(ui->screen_tabview_1, LV_DIR_TOP);
    lv_tabview_set_tab_bar_size(ui->screen_tabview_1, 50);
    ui->screen_tabview_1_tab_1 = lv_tabview_add_tab(ui->screen_tabview_1, "basic");
    ui->screen_tabview_1_tab_2 = lv_tabview_add_tab(ui->screen_tabview_1, "control");
    ui->screen_tabview_1_tab_3 = lv_tabview_add_tab(ui->screen_tabview_1, "threshold");

    //Write style for screen_tabview_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_tabview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_tabview_1, lv_color_hex(0xeaeff3), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_tabview_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_tabview_1, 4, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_tabview_1, 100, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_tabview_1, lv_color_hex(0xc0c0c0), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_tabview_1, LV_BORDER_SIDE_LEFT | LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT | LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_tabview_1, 15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_tabview_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_tabview_1, lv_color_hex(0x4d4d4d), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_tabview_1, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_tabview_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_tabview_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_tabview_1, 13, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_tabview_1_extra_btnm_main_default
    static lv_style_t style_screen_tabview_1_extra_btnm_main_default;
    ui_init_style(&style_screen_tabview_1_extra_btnm_main_default);

    lv_style_set_bg_opa(&style_screen_tabview_1_extra_btnm_main_default, 255);
    lv_style_set_bg_color(&style_screen_tabview_1_extra_btnm_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_tabview_1_extra_btnm_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_tabview_1_extra_btnm_main_default, 0);
    lv_style_set_radius(&style_screen_tabview_1_extra_btnm_main_default, 0);
    for(uint32_t i = 0; i < lv_tabview_get_tab_count(ui->screen_tabview_1); i++)
    {
        lv_obj_add_style(lv_obj_get_child(lv_tabview_get_tab_bar(ui->screen_tabview_1), i), &style_screen_tabview_1_extra_btnm_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    }

    //Write style state: LV_STATE_DEFAULT for &style_screen_tabview_1_extra_btnm_items_default
    static lv_style_t style_screen_tabview_1_extra_btnm_items_default;
    ui_init_style(&style_screen_tabview_1_extra_btnm_items_default);

    lv_style_set_text_color(&style_screen_tabview_1_extra_btnm_items_default, lv_color_hex(0x4d4d4d));
    lv_style_set_text_font(&style_screen_tabview_1_extra_btnm_items_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_tabview_1_extra_btnm_items_default, 255);
    for(uint32_t i = 0; i < lv_tabview_get_tab_count(ui->screen_tabview_1); i++)
    {
        lv_obj_add_style(lv_obj_get_child(lv_tabview_get_tab_bar(ui->screen_tabview_1), i), &style_screen_tabview_1_extra_btnm_items_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    }

    //Write style state: LV_STATE_CHECKED for &style_screen_tabview_1_extra_btnm_items_checked
    static lv_style_t style_screen_tabview_1_extra_btnm_items_checked;
    ui_init_style(&style_screen_tabview_1_extra_btnm_items_checked);

    lv_style_set_bg_opa(&style_screen_tabview_1_extra_btnm_items_checked, 60);
    lv_style_set_bg_color(&style_screen_tabview_1_extra_btnm_items_checked, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_screen_tabview_1_extra_btnm_items_checked, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_tabview_1_extra_btnm_items_checked, 4);
    lv_style_set_border_opa(&style_screen_tabview_1_extra_btnm_items_checked, 255);
    lv_style_set_border_color(&style_screen_tabview_1_extra_btnm_items_checked, lv_color_hex(0x2195f6));
    lv_style_set_border_side(&style_screen_tabview_1_extra_btnm_items_checked, LV_BORDER_SIDE_BOTTOM);
    lv_style_set_radius(&style_screen_tabview_1_extra_btnm_items_checked, 0);
    lv_style_set_text_color(&style_screen_tabview_1_extra_btnm_items_checked, lv_color_hex(0x2195f6));
    lv_style_set_text_font(&style_screen_tabview_1_extra_btnm_items_checked, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_tabview_1_extra_btnm_items_checked, 255);
    for(uint32_t i = 0; i < lv_tabview_get_tab_count(ui->screen_tabview_1); i++)
    {
        lv_obj_add_style(lv_obj_get_child(lv_tabview_get_tab_bar(ui->screen_tabview_1), i), &style_screen_tabview_1_extra_btnm_items_checked, LV_PART_MAIN|LV_STATE_CHECKED);
    }

    //Write codes basic
    lv_obj_t * screen_tabview_1_tab_1_label = lv_label_create(ui->screen_tabview_1_tab_1);
    lv_label_set_text(screen_tabview_1_tab_1_label, "");

    //Write codes screen_label_humi
    ui->screen_label_humi = lv_label_create(ui->screen_tabview_1_tab_1);
    lv_obj_set_pos(ui->screen_label_humi, 110, 21);
    lv_obj_set_size(ui->screen_label_humi, 145, 64);
    lv_label_set_text(ui->screen_label_humi, "humi:");
    lv_label_set_long_mode(ui->screen_label_humi, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_humi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_humi, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_humi, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_humi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_humi, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_temp
    ui->screen_label_temp = lv_label_create(ui->screen_tabview_1_tab_1);
    lv_obj_set_pos(ui->screen_label_temp, -41, 16);
    lv_obj_set_size(ui->screen_label_temp, 145, 64);
    lv_label_set_text(ui->screen_label_temp, "taemp:");
    lv_label_set_long_mode(ui->screen_label_temp, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_temp, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_temp, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_light
    ui->screen_label_light = lv_label_create(ui->screen_tabview_1_tab_1);
    lv_obj_set_pos(ui->screen_label_light, -48, 105);
    lv_obj_set_size(ui->screen_label_light, 145, 64);
    lv_label_set_text(ui->screen_label_light, "light:");
    lv_label_set_long_mode(ui->screen_label_light, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_light, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_light, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_light, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_light, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_light, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_co2
    ui->screen_label_co2 = lv_label_create(ui->screen_tabview_1_tab_1);
    lv_obj_set_pos(ui->screen_label_co2, 111, 105);
    lv_obj_set_size(ui->screen_label_co2, 145, 64);
    lv_label_set_text(ui->screen_label_co2, "CO2:");
    lv_label_set_long_mode(ui->screen_label_co2, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_co2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_co2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_co2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_co2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_co2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_co2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes control
    lv_obj_t * screen_tabview_1_tab_2_label = lv_label_create(ui->screen_tabview_1_tab_2);
    lv_label_set_text(screen_tabview_1_tab_2_label, "");

    //Write codes screen_sw_fan
    ui->screen_sw_fan = lv_switch_create(ui->screen_tabview_1_tab_2);
    lv_obj_set_pos(ui->screen_sw_fan, 171, 40);
    lv_obj_set_size(ui->screen_sw_fan, 96, 27);

    //Write style for screen_sw_fan, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sw_fan, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sw_fan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_fan, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sw_fan, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_sw_fan, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_sw_fan, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_sw_fan, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_sw_fan, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_sw_fan, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sw_fan, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sw_fan, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sw_fan, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_fan, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_sw_pump
    ui->screen_sw_pump = lv_switch_create(ui->screen_tabview_1_tab_2);
    lv_obj_set_pos(ui->screen_sw_pump, -6, 40);
    lv_obj_set_size(ui->screen_sw_pump, 96, 27);

    //Write style for screen_sw_pump, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_sw_pump, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sw_pump, lv_color_hex(0xe6e2e6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sw_pump, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sw_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_pump, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_sw_pump, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_sw_pump, Part: LV_PART_INDICATOR, State: LV_STATE_CHECKED.
    lv_obj_set_style_bg_opa(ui->screen_sw_pump, 255, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_color(ui->screen_sw_pump, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_bg_grad_dir(ui->screen_sw_pump, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_CHECKED);
    lv_obj_set_style_border_width(ui->screen_sw_pump, 0, LV_PART_INDICATOR|LV_STATE_CHECKED);

    //Write style for screen_sw_pump, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_sw_pump, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_sw_pump, lv_color_hex(0xffffff), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_sw_pump, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_sw_pump, 0, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_sw_pump, 10, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_Lable_lightcontrol
    ui->screen_Lable_lightcontrol = lv_label_create(ui->screen_tabview_1_tab_2);
    lv_obj_set_pos(ui->screen_Lable_lightcontrol, 90, 114);
    lv_obj_set_size(ui->screen_Lable_lightcontrol, 100, 32);
    lv_label_set_text(ui->screen_Lable_lightcontrol, "lightness");
    lv_label_set_long_mode(ui->screen_Lable_lightcontrol, LV_LABEL_LONG_WRAP);

    //Write style for screen_Lable_lightcontrol, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Lable_lightcontrol, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Lable_lightcontrol, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Lable_lightcontrol, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Lable_lightcontrol, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Lable_lightcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_slider_light
    ui->screen_slider_light = lv_slider_create(ui->screen_tabview_1_tab_2);
    lv_obj_set_pos(ui->screen_slider_light, 30, 139);
    lv_obj_set_size(ui->screen_slider_light, 216, 4);
    lv_slider_set_range(ui->screen_slider_light, 0, 100);
    lv_slider_set_mode(ui->screen_slider_light, LV_SLIDER_MODE_NORMAL);
    lv_slider_set_value(ui->screen_slider_light, 50, LV_ANIM_OFF);

    //Write style for screen_slider_light, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_light, 60, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_light, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_light, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_light, 8, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(ui->screen_slider_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_slider_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_slider_light, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_light, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_light, lv_color_hex(0x2195f6), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_light, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_light, 8, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write style for screen_slider_light, Part: LV_PART_KNOB, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_slider_light, 255, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_slider_light, lv_color_hex(0x2195f6), LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_slider_light, LV_GRAD_DIR_NONE, LV_PART_KNOB|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_slider_light, 8, LV_PART_KNOB|LV_STATE_DEFAULT);

    //Write codes screen_Lable_fancontrol
    ui->screen_Lable_fancontrol = lv_label_create(ui->screen_tabview_1_tab_2);
    lv_obj_set_pos(ui->screen_Lable_fancontrol, 174, 3);
    lv_obj_set_size(ui->screen_Lable_fancontrol, 100, 32);
    lv_label_set_text(ui->screen_Lable_fancontrol, "fan");
    lv_label_set_long_mode(ui->screen_Lable_fancontrol, LV_LABEL_LONG_WRAP);

    //Write style for screen_Lable_fancontrol, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_Lable_fancontrol, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_Lable_fancontrol, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_Lable_fancontrol, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_Lable_fancontrol, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_Lable_fancontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_pumpcontrol
    ui->screen_label_pumpcontrol = lv_label_create(ui->screen_tabview_1_tab_2);
    lv_obj_set_pos(ui->screen_label_pumpcontrol, -8, 3);
    lv_obj_set_size(ui->screen_label_pumpcontrol, 100, 32);
    lv_label_set_text(ui->screen_label_pumpcontrol, "pump");
    lv_label_set_long_mode(ui->screen_label_pumpcontrol, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_pumpcontrol, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_pumpcontrol, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_pumpcontrol, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_pumpcontrol, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_pumpcontrol, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_pumpcontrol, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes threshold
    lv_obj_t * screen_tabview_1_tab_3_label = lv_label_create(ui->screen_tabview_1_tab_3);
    lv_label_set_text(screen_tabview_1_tab_3_label, "");

    //Write codes screen_spinbox_humi
    ui->screen_spinbox_humi = lv_spinbox_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_pos(ui->screen_spinbox_humi, 152, 48);
    lv_obj_set_width(ui->screen_spinbox_humi, 70);
    lv_obj_set_height(ui->screen_spinbox_humi, 40);
    lv_spinbox_set_digit_format(ui->screen_spinbox_humi, 5, 3);
    lv_spinbox_set_range(ui->screen_spinbox_humi, -99999, 99999);
    int32_t screen_spinbox_humi_h = lv_obj_get_height(ui->screen_spinbox_humi);
    ui->screen_spinbox_humi_btn_plus = lv_button_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_size(ui->screen_spinbox_humi_btn_plus, screen_spinbox_humi_h, screen_spinbox_humi_h);
    lv_obj_align_to(ui->screen_spinbox_humi_btn_plus, ui->screen_spinbox_humi, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(ui->screen_spinbox_humi_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->screen_spinbox_humi_btn_plus, lv_screen_spinbox_humi_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->screen_spinbox_humi_btn_minus = lv_button_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_size(ui->screen_spinbox_humi_btn_minus, screen_spinbox_humi_h, screen_spinbox_humi_h);
    lv_obj_align_to(ui->screen_spinbox_humi_btn_minus, ui->screen_spinbox_humi, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(ui->screen_spinbox_humi_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->screen_spinbox_humi_btn_minus, lv_screen_spinbox_humi_decrement_event_cb, LV_EVENT_ALL, NULL);

    //Write style for screen_spinbox_humi, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_spinbox_humi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_spinbox_humi, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_spinbox_humi, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_spinbox_humi, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_spinbox_humi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_spinbox_humi, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_spinbox_humi, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_spinbox_humi, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_spinbox_humi, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_spinbox_humi, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_spinbox_humi, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_spinbox_humi, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_spinbox_humi, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_spinbox_humi, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_spinbox_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_spinbox_humi, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_spinbox_humi, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_spinbox_humi, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_spinbox_humi, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_spinbox_humi, &lv_font_montserratMedium_12, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_spinbox_humi, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_spinbox_humi, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_spinbox_humi, lv_color_hex(0x2195f6), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_spinbox_humi, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_spinbox_humi_extra_btns_main_default
    static lv_style_t style_screen_spinbox_humi_extra_btns_main_default;
    ui_init_style(&style_screen_spinbox_humi_extra_btns_main_default);

    lv_style_set_text_color(&style_screen_spinbox_humi_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_spinbox_humi_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_spinbox_humi_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_screen_spinbox_humi_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_spinbox_humi_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_screen_spinbox_humi_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_spinbox_humi_extra_btns_main_default, 0);
    lv_style_set_radius(&style_screen_spinbox_humi_extra_btns_main_default, 5);
    lv_style_set_shadow_width(&style_screen_spinbox_humi_extra_btns_main_default, 0);
    lv_obj_add_style(ui->screen_spinbox_humi_btn_plus, &style_screen_spinbox_humi_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_spinbox_humi_btn_minus, &style_screen_spinbox_humi_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_spinbox_temp
    ui->screen_spinbox_temp = lv_spinbox_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_pos(ui->screen_spinbox_temp, 152, -8);
    lv_obj_set_width(ui->screen_spinbox_temp, 70);
    lv_obj_set_height(ui->screen_spinbox_temp, 40);
    lv_spinbox_set_digit_format(ui->screen_spinbox_temp, 5, 3);
    lv_spinbox_set_range(ui->screen_spinbox_temp, -99999, 99999);
    int32_t screen_spinbox_temp_h = lv_obj_get_height(ui->screen_spinbox_temp);
    ui->screen_spinbox_temp_btn_plus = lv_button_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_size(ui->screen_spinbox_temp_btn_plus, screen_spinbox_temp_h, screen_spinbox_temp_h);
    lv_obj_align_to(ui->screen_spinbox_temp_btn_plus, ui->screen_spinbox_temp, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(ui->screen_spinbox_temp_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->screen_spinbox_temp_btn_plus, lv_screen_spinbox_temp_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->screen_spinbox_temp_btn_minus = lv_button_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_size(ui->screen_spinbox_temp_btn_minus, screen_spinbox_temp_h, screen_spinbox_temp_h);
    lv_obj_align_to(ui->screen_spinbox_temp_btn_minus, ui->screen_spinbox_temp, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(ui->screen_spinbox_temp_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->screen_spinbox_temp_btn_minus, lv_screen_spinbox_temp_decrement_event_cb, LV_EVENT_ALL, NULL);

    //Write style for screen_spinbox_temp, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_spinbox_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_spinbox_temp, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_spinbox_temp, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_spinbox_temp, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_spinbox_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_spinbox_temp, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_spinbox_temp, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_spinbox_temp, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_spinbox_temp, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_spinbox_temp, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_spinbox_temp, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_spinbox_temp, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_spinbox_temp, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_spinbox_temp, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_spinbox_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_spinbox_temp, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_spinbox_temp, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_spinbox_temp, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_spinbox_temp, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_spinbox_temp, &lv_font_montserratMedium_12, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_spinbox_temp, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_spinbox_temp, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_spinbox_temp, lv_color_hex(0x2195f6), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_spinbox_temp, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_spinbox_temp_extra_btns_main_default
    static lv_style_t style_screen_spinbox_temp_extra_btns_main_default;
    ui_init_style(&style_screen_spinbox_temp_extra_btns_main_default);

    lv_style_set_text_color(&style_screen_spinbox_temp_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_spinbox_temp_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_spinbox_temp_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_screen_spinbox_temp_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_spinbox_temp_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_screen_spinbox_temp_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_spinbox_temp_extra_btns_main_default, 0);
    lv_style_set_radius(&style_screen_spinbox_temp_extra_btns_main_default, 5);
    lv_style_set_shadow_width(&style_screen_spinbox_temp_extra_btns_main_default, 0);
    lv_obj_add_style(ui->screen_spinbox_temp_btn_plus, &style_screen_spinbox_temp_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_spinbox_temp_btn_minus, &style_screen_spinbox_temp_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_3
    ui->screen_label_3 = lv_label_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_pos(ui->screen_label_3, -12, 58);
    lv_obj_set_size(ui->screen_label_3, 100, 32);
    lv_label_set_text(ui->screen_label_3, "humi");
    lv_label_set_long_mode(ui->screen_label_3, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_3, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_3, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_2
    ui->screen_label_2 = lv_label_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_pos(ui->screen_label_2, -12, 3);
    lv_obj_set_size(ui->screen_label_2, 100, 32);
    lv_label_set_text(ui->screen_label_2, "temp\n");
    lv_label_set_long_mode(ui->screen_label_2, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_2, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_2, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_label_4
    ui->screen_label_4 = lv_label_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_pos(ui->screen_label_4, -12, 113);
    lv_obj_set_size(ui->screen_label_4, 100, 32);
    lv_label_set_text(ui->screen_label_4, "lightness");
    lv_label_set_long_mode(ui->screen_label_4, LV_LABEL_LONG_WRAP);

    //Write style for screen_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_label_4, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_label_4, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_spinbox_light
    ui->screen_spinbox_light = lv_spinbox_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_pos(ui->screen_spinbox_light, 152, 104);
    lv_obj_set_width(ui->screen_spinbox_light, 70);
    lv_obj_set_height(ui->screen_spinbox_light, 40);
    lv_spinbox_set_digit_format(ui->screen_spinbox_light, 5, 3);
    lv_spinbox_set_range(ui->screen_spinbox_light, -99999, 99999);
    int32_t screen_spinbox_light_h = lv_obj_get_height(ui->screen_spinbox_light);
    ui->screen_spinbox_light_btn_plus = lv_button_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_size(ui->screen_spinbox_light_btn_plus, screen_spinbox_light_h, screen_spinbox_light_h);
    lv_obj_align_to(ui->screen_spinbox_light_btn_plus, ui->screen_spinbox_light, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_obj_set_style_bg_image_src(ui->screen_spinbox_light_btn_plus, LV_SYMBOL_PLUS, 0);
    lv_obj_add_event_cb(ui->screen_spinbox_light_btn_plus, lv_screen_spinbox_light_increment_event_cb, LV_EVENT_ALL, NULL);
    ui->screen_spinbox_light_btn_minus = lv_button_create(ui->screen_tabview_1_tab_3);
    lv_obj_set_size(ui->screen_spinbox_light_btn_minus, screen_spinbox_light_h, screen_spinbox_light_h);
    lv_obj_align_to(ui->screen_spinbox_light_btn_minus, ui->screen_spinbox_light, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_set_style_bg_image_src(ui->screen_spinbox_light_btn_minus, LV_SYMBOL_MINUS, 0);
    lv_obj_add_event_cb(ui->screen_spinbox_light_btn_minus, lv_screen_spinbox_light_decrement_event_cb, LV_EVENT_ALL, NULL);

    //Write style for screen_spinbox_light, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_spinbox_light, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_spinbox_light, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_spinbox_light, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_spinbox_light, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_spinbox_light, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_spinbox_light, lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_spinbox_light, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_spinbox_light, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_spinbox_light, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_spinbox_light, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_spinbox_light, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_spinbox_light, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_spinbox_light, &lv_font_montserratMedium_12, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_spinbox_light, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_spinbox_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_spinbox_light, 5, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_spinbox_light, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_spinbox_light, Part: LV_PART_CURSOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_text_color(ui->screen_spinbox_light, lv_color_hex(0xffffff), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_spinbox_light, &lv_font_montserratMedium_12, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_spinbox_light, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_spinbox_light, 255, LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_spinbox_light, lv_color_hex(0x2195f6), LV_PART_CURSOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_spinbox_light, LV_GRAD_DIR_NONE, LV_PART_CURSOR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_spinbox_light_extra_btns_main_default
    static lv_style_t style_screen_spinbox_light_extra_btns_main_default;
    ui_init_style(&style_screen_spinbox_light_extra_btns_main_default);

    lv_style_set_text_color(&style_screen_spinbox_light_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_spinbox_light_extra_btns_main_default, &lv_font_montserratMedium_12);
    lv_style_set_text_opa(&style_screen_spinbox_light_extra_btns_main_default, 255);
    lv_style_set_bg_opa(&style_screen_spinbox_light_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_spinbox_light_extra_btns_main_default, lv_color_hex(0x2195f6));
    lv_style_set_bg_grad_dir(&style_screen_spinbox_light_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_spinbox_light_extra_btns_main_default, 0);
    lv_style_set_radius(&style_screen_spinbox_light_extra_btns_main_default, 5);
    lv_style_set_shadow_width(&style_screen_spinbox_light_extra_btns_main_default, 0);
    lv_obj_add_style(ui->screen_spinbox_light_btn_plus, &style_screen_spinbox_light_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_spinbox_light_btn_minus, &style_screen_spinbox_light_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen);

    //Init events for screen.
    events_init_screen(ui);
}
