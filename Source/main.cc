
#include "stm32f4xx_conf.h" 
#include "stm32f4xx_rcc.h"
#include "main.h"
#include "stm32f4xx.h"


#include "Peripheral.h"

#include "UARTDevice.h"

#include "stm32f4xx_adc.h"


#include "ADC.h"
#include "GPIO.h"
#include "PWM.h"
#include "DAC.h"

#include "arm_math.h"

#include "TimerDMA.h"

#include "Path.h"

#include "Control.h"

#include "Scheduler.h"


void SysTick_Handler(void)
{
    Scheduler::getScheduler().TimerUpdate();
}
/*
 * ADC Inputs
 * Sens1 PA0
 * Sens2 PA1
 * Sens3 PA2
 * Sens4 PA3
 * Angle1A PA5
 * Angle1B PA6
 * Temp1 PA7
 * Angle2A PA8
 * Angle2B PA9
 * Temp2 PA10
 *
 * DAC Output
 * LaserAdj PA4
 *
 * GPIO laser on off
 * LaserOn PB0
 *
 * UART
 * TX PB10
 * RX PB11
 *
 * PWM Output Control
 * IN1 PB6
 * IN2 PB7
 * IN3 PB8
 * IN4 PB9
 *
 *
 *
 */


//CGPIO_PP LaserOn(RCC_AHB1Periph_GPIOC, GPIOC, GPIO_Pin_6);

CGPIO_OD StatusLED(RCC_AHB1Periph_GPIOB, GPIOB, GPIO_Pin_0);
//
//CGPIO_PP Reset1(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_7);
//CGPIO_PP Reset2(RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_8);
//
//CUART2Port DebugUart;
//
//CPWM PWM_CONTROLLER;
//
//extern ControlManager CONTROL_MANAGER;
//
//CADC DMA_ADC(&CONTROL_MANAGER);
//int nFrequency = 100000;
//CTimerDMA ADC_Trigger(nFrequency);
//
CDAC DAC_ADJ;
//
//
//CommandInterface CMD_INTERFACE(&CONTROL_MANAGER,
//        &DebugUart,
//        &StatusLED);
//
//
//ControlManager CONTROL_MANAGER(&CMD_INTERFACE,
//        &PWM_CONTROLLER,
//        &LaserOn,
//        &DAC_ADJ,
//        1.0/(float)nFrequency);
//
////CGPIO_OD DAC_ADJ( RCC_AHB1Periph_GPIOA, GPIOA, GPIO_Pin_4);
//
//
//void UART3Interupt(void)
//{
//    DebugUart.HandleInterupt();
//}
//


int main(void)
{


//
//    PWM_CONTROLLER.Init();
//    PWM_CONTROLLER.SetChannel1(15.0);
//    PWM_CONTROLLER.SetChannel2(15.0);
//    PWM_CONTROLLER.SetChannel3(15.0);
//    PWM_CONTROLLER.SetChannel4(15.0);
//
//
//    Reset1.Init();
//    Reset2.Init();
//    Reset1.SetLow();
//    Reset2.SetLow();

    StatusLED.Init();
//    LaserOn.Init();
//
//    Reset1.SetHigh();
//    Reset2.SetHigh();
//
//    //wait for the mirrors to position to about center before starting the control loops
//    for(int x = 0; x < 100000; x++)
//    {
//        ;
//    }
//
//
//    DebugUart.Init();
//
//    DMA_ADC.Init();
//
//    DAC_ADJ.Init();
//    DAC_ADJ.SetDAC(2.00);
//    LaserOn.SetHigh();
//    ADC_Trigger.Init();
//    //DAC_ADJ.Init();
//    //DAC_ADJ.SetLow();
//
//    CMD_INTERFACE.MainWileLoop();

    while(1)
    {
        StatusLED.Toggle();
        for( int x = 0; x < 200000; x++)
        {
            for( int y = 0; x < 20000; x++)
                {
                ;
                }
        }

    }
}
