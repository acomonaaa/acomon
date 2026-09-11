/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd.h"
#include "lvgl.h"
#include "lv_port_disp.h"
#include "../Inc/lv_port_indev.h"
#include "touch.h"
#include "gui_guider.h"
#include "events_init.h"
#include "custom.h"
#include "user_app.h"  /* 业务层隔离入口 - 仅此一处引入 */
#include "health_app.h"
#include "app_config.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* 初始化完成信号量：defaultTask 完成硬件初始化后通知 guiTask */
osSemaphoreId_t xInitDoneSem;

/* GUI 互斥锁，保护 LVGL API 线程安全 */
osMutexId_t xGuiMutex;

/* GUI Guider 全局 UI 结构体 */
extern lv_ui guider_ui;
/* USER CODE END Variables */

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 1536 * 4, /* 6KB：仅硬件初始化后自删除 */
  .priority = (osPriority_t) osPriorityAboveNormal,
};

/* Definitions for guiTask */
osThreadId_t guiTaskHandle;
const osThreadAttr_t guiTask_attributes = {
  .name = "guiTask",
  .stack_size = 3072 * 4, /* 12KB：LVGL 足够 */
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartGUITask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* 创建初始化完成信号量（初始计数为0，等待 defaultTask Release） */
  xInitDoneSem = osSemaphoreNew(1, 0, NULL);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* creation of guiTask */
  guiTaskHandle = osThreadNew(StartGUITask, NULL, &guiTask_attributes);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  /* 1. 初始化 LCD 硬件（校准需要直接操作 LCD） */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_12, GPIO_PIN_RESET);
  if (LCD_Init() != 0) {
    Error_Handler();
  }

  /* 2. 触摸功能已禁用，跳过触摸初始化和校准 */

  /* 3. 直接初始化 LVGL 和显示驱动 */
  /* LCD方向和清屏已在 lv_port_disp_init -> disp_init 中处理 */
  lv_init();
  lv_port_disp_init();

  /* 4. 初始化 LVGL 输入设备（Encoder Indev 仅提供 indev→Group 绑定，
   *    实际按键扫描由 custom_ui_process_keys() 手动完成，见 custom.c） */
  lv_port_indev_init();

  /* 5. 创建 GUI 互斥锁 */
  xGuiMutex = osMutexNew(NULL);

  /* 6. 初始化业务层：先准备共享数据和后台任务，再放行 GUI */
  User_App_Init();

  /* 7. 通知 guiTask：硬件初始化和业务层初始化均已完成 */
  osSemaphoreRelease(xInitDoneSem);

  /* 初始化完成：defaultTask 不再需要，可以删除自身（未注册心跳槽） */
  vTaskDelete(NULL);

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartGUITask */
/**
  * @brief  Function implementing the guiTask thread.
  *         负责 GUI Guider UI 初始化和 lv_timer_handler 主循环
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */

  /* 阻塞等待 defaultTask 完成 LCD/LVGL/触摸初始化（信号量同步，非固定延时） */
  osSemaphoreAcquire(xInitDoneSem, osWaitForever);

  /* 获取锁后初始化 GUI Guider UI */
  if (osMutexAcquire(xGuiMutex, osWaitForever) == osOK) {
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);
    custom_ui_update_data();
    osMutexRelease(xGuiMutex);
  }

  /* GUI 主循环：lv_timer_handler + 主循环数据刷新（绕过 lv_timer 定时问题） */
  for (;;) {
    health_beat(APP_HB_GUI);
    if (osMutexAcquire(xGuiMutex, osWaitForever) == osOK) {
      custom_ui_process_keys();
      lv_timer_handler();
      osMutexRelease(xGuiMutex);
    }
    /* 每 ~100ms 直接刷新传感器数据到屏幕（主循环方式，不依赖 lv_timer） */
    {
      static uint32_t _refresh_cnt = 0;
      if (++_refresh_cnt >= 20) {
        _refresh_cnt = 0;
        if (osMutexAcquire(xGuiMutex, osWaitForever) == osOK) {
          custom_ui_update_data();
          osMutexRelease(xGuiMutex);
        }
      }
    }
    osDelay(5);
  }

  /* USER CODE END StartGUITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
