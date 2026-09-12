/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOG

/* USER CODE BEGIN Private defines */
/* 调试 LED */
#define DEBUG_LED_Pin  GPIO_PIN_9     /* PF9: LED0 (低电平点亮)  */
#define DEBUG_LED_GPIO_Port GPIOF

/* ---- 智慧农业执行器/报警/L610 引脚（USER CODE，CubeMX 不管理）---- */
#define FAN_Pin        GPIO_PIN_0     /* PG0: 通风风扇 */
#define FAN_GPIO_Port  GPIOG
#define PUMP_Pin       GPIO_PIN_1     /* PG1: 灌溉水泵 */
#define PUMP_GPIO_Port GPIOG
#define ALARM_LED_Pin  GPIO_PIN_2     /* PG2: 告警指示灯 */
#define ALARM_LED_GPIO_Port GPIOG
#define BUZZER_Pin     GPIO_PIN_3     /* PG3: 声光报警蜂鸣 */
#define BUZZER_GPIO_Port GPIOG
#define GROW_LIGHT_Pin GPIO_PIN_4     /* PG4: 补光灯（软PWM） */
#define GROW_LIGHT_GPIO_Port GPIOG
#define DHT11_Pin      GPIO_PIN_5     /* PG5: DHT11 单总线 */
#define DHT11_GPIO_Port GPIOG

/* L610 模组 USART1: PA9=TX, PA10=RX */

/* 4 物理按键引脚定义（Keypad 输入） */
#define KEY0_Pin    GPIO_PIN_4       /* PE4: 上键 (Pull-Up)   */
#define KEY0_GPIO_Port GPIOE
#define KEY1_Pin    GPIO_PIN_3       /* PE3: 确认键 (Pull-Up) */
#define KEY1_GPIO_Port GPIOE
#define KEY2_Pin    GPIO_PIN_2       /* PE2: 下键 (Pull-Up)   */
#define KEY2_GPIO_Port GPIOE
#define KEY_UP_Pin  GPIO_PIN_0       /* PA0: 返回键 (Pull-Down) ⚠️高电平有效 */
#define KEY_UP_GPIO_Port GPIOA
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
