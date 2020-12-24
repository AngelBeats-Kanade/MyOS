#ifndef TASK_H
#define TASK_H

#include "FreeRTOS.h"
#include "main.h"

#define taskYIELD() portYIELD()

#define taskENTER_CRITICAL()                    portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR()  portSET_INTERRUPT_MASK_FROM_ISR()
#define taskEXIT_CRITICAL()                        portEXIT_CRITICAL()
#define taskEXIT_CRITICAL_FROM_ISR()      portCLEAR_INTERRUPT_MASK_FROM_ISR(x)

typedef void *TaskHandle_t;

#if (configSUPPORT_STATIC_ALLOCATION == 1)

TaskHandle_t xTaskCreateStatic(TaskFunction_t,
                               const char *const,
                               const uint32_t,
                               void *const,
                               StackType_t *const,
                               TCB_t *const);

#endif

static void prvInitialiseNewTask(TaskFunction_t,
                                 const char *const,
                                 const uint32_t,
                                 void *const,
                                 TaskHandle_t *const,
                                 TCB_t *);

void prvInitialiseTaskLists(void);

void vTaskStartScheduler(void);

void vTaskSwitchContext(void);

void xTaskIncrementTick(void);

void vTaskDelay(TickType_t const xTicksToDelay);

#endif //TASK_H