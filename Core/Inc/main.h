#ifndef MAIN_H
#define MAIN_H

#include "FreeRTOS.h"
#include "stm32f1xx_hal.h"

#define LED1_Pin GPIO_PIN_2
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_3
#define LED2_GPIO_Port GPIOC

void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize);

void Error_Handler(void);

#endif
