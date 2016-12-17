/**
  ******************************************************************************
  * File Name          : main.h
  * Description        : This file contains the common defines of the application
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
  /* Includes ------------------------------------------------------------------*/

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/

#define SD_CD_Pin GPIO_PIN_13
#define SD_CD_GPIO_Port GPIOC
#define SD_WP_Pin GPIO_PIN_14
#define SD_WP_GPIO_Port GPIOC
#define Angle_1_Pin GPIO_PIN_0
#define Angle_1_GPIO_Port GPIOA
#define Angle_2_Pin GPIO_PIN_1
#define Angle_2_GPIO_Port GPIOA
#define Stepper_DAC_Pin GPIO_PIN_4
#define Stepper_DAC_GPIO_Port GPIOA
#define Laser_DAC_Pin GPIO_PIN_5
#define Laser_DAC_GPIO_Port GPIOA
#define LED_OUT_Pin GPIO_PIN_0
#define LED_OUT_GPIO_Port GPIOB
#define MS1_Pin GPIO_PIN_12
#define MS1_GPIO_Port GPIOB
#define MS2_Pin GPIO_PIN_13
#define MS2_GPIO_Port GPIOB
#define MS3_Pin GPIO_PIN_14
#define MS3_GPIO_Port GPIOB
#define HomeSwitch_Pin GPIO_PIN_15
#define HomeSwitch_GPIO_Port GPIOB
#define PWM_IN4_Pin GPIO_PIN_8
#define PWM_IN4_GPIO_Port GPIOA
#define PWM_IN3_Pin GPIO_PIN_9
#define PWM_IN3_GPIO_Port GPIOA
#define PWM_IN2_Pin GPIO_PIN_10
#define PWM_IN2_GPIO_Port GPIOA
#define PWM_IN1_Pin GPIO_PIN_11
#define PWM_IN1_GPIO_Port GPIOA
#define ENCODER_A_Pin GPIO_PIN_6
#define ENCODER_A_GPIO_Port GPIOB
#define ENCODER_B_Pin GPIO_PIN_7
#define ENCODER_B_GPIO_Port GPIOB
#define Step_Pin GPIO_PIN_8
#define Step_GPIO_Port GPIOB
#define Dir_Pin GPIO_PIN_9
#define Dir_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/**
  * @}
  */ 

/**
  * @}
*/ 

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
