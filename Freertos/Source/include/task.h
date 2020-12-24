#ifndef TASK_H
#define TASK_H

#include "FreeRTOS.h"
#include "main.h"

#define taskYIELD() portYIELD()

#define taskENTER_CRITICAL()           portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR()  portSET_INTERRUPT_MASK_FROM_ISR()
#define taskEXIT_CRITICAL()            portEXIT_CRITICAL()
#define taskEXIT_CRITICAL_FROM_ISR()   portCLEAR_INTERRUPT_MASK_FROM_ISR(x)

#define tskIDLE_PRIORITY (UBaseType_t) 0U

typedef void *TaskHandle_t;

#if (configSUPPORT_STATIC_ALLOCATION == 1)

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               UBaseType_t uxPriority,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer);

#endif

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                 const char *const pcName,
                                 const uint32_t ulStackDepth,
                                 void *const pvParameters,
                                 UBaseType_t uxPriority,
                                 TaskHandle_t *const pxCreatedTask,
                                 TCB_t *pxNewTCB);

void prvInitialiseTaskLists(void);

void vTaskStartScheduler(void);

void vTaskSwitchContext(void);

void xTaskIncrementTick(void);

void vTaskDelay(TickType_t const xTicksToDelay);

#endif //TASK_H