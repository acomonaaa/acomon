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

/* 鍒濆鍖栧畬鎴愪俊鍙烽噺锛歞efaultTask 瀹屾垚纭欢鍒濆鍖栧悗閫氱煡 guiTask */
osSemaphoreId_t xInitDoneSem;

/* GUI 浜掓枼閿侊紝淇濇姢 LVGL API 绾跨▼瀹夊叏 */
osMutexId_t xGuiMutex;

/* GUI Guider 鍏ㄥ眬 UI 缁撴瀯浣?*/
extern lv_ui guider_ui;
/* USER CODE END Variables */

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};

/* Definitions for guiTask */
osThreadId_t guiTaskHandle;
const osThreadAttr_t guiTask_attributes = {
  .name = "guiTask",
  .stack_size = 4096 * 4,
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
  /* 鍒涘缓鍒濆鍖栧畬鎴愪俊鍙烽噺锛堝垵濮嬭鏁颁负0锛岀瓑寰?defaultTask Release锛?*/
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

  /* 1. 鍒濆鍖?LCD 纭欢锛堟牎鍑嗛渶瑕佺洿鎺ユ搷浣?LCD锛?*/
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

  /* 4. 触摸已禁用，不注册输入设备 */

  /* 5. 鍒涘缓 GUI 浜掓枼閿?*/
  xGuiMutex = osMutexNew(NULL);

  /* 6. 閫氱煡 guiTask锛氱‖浠跺垵濮嬪寲鍏ㄩ儴瀹屾垚 */
  osSemaphoreRelease(xInitDoneSem);

  /* 鍒濆鍖栧畬鎴愶紝defaultTask 涓嶅啀闇€瑕侊紝鍙互鍒犻櫎鑷韩 */
  vTaskDelete(NULL);

  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartGUITask */
/**
  * @brief  Function implementing the guiTask thread.
  *         璐熻矗 GUI Guider UI 鍒濆鍖栧拰 lv_timer_handler 涓诲惊鐜?  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartGUITask */
void StartGUITask(void *argument)
{
  /* USER CODE BEGIN StartGUITask */

  /* 闃诲绛夊緟 defaultTask 瀹屾垚 LCD/LVGL/瑙︽懜鍒濆鍖栵紙淇″彿閲忓悓姝ワ紝闈炲浐瀹氬欢鏃讹級 */
  osSemaphoreAcquire(xInitDoneSem, osWaitForever);

  /* 鑾峰彇閿佸悗鍒濆鍖?GUI Guider UI */
  if (osMutexAcquire(xGuiMutex, osWaitForever) == osOK) {
    setup_ui(&guider_ui);
    events_init(&guider_ui);
    custom_init(&guider_ui);
    osMutexRelease(xGuiMutex);
  }

  /* GUI 涓诲惊鐜?*/
  for (;;) {
    if (osMutexAcquire(xGuiMutex, osWaitForever) == osOK) {
      lv_timer_handler();
      osMutexRelease(xGuiMutex);
    }
    osDelay(5);
  }

  /* USER CODE END StartGUITask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
