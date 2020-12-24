/**
   ****************************************************************************
   * @file           : main.c
   * @brief        : Main program body
   ****************************************************************************
   * @attention
   *
   * <h2><center>&copy; Copyright (c) 2020 AngelBeats.
   * All rights reserved.</center></h2>
   *
   ****************************************************************************
   */
/* Private includes -----------------------------------------------------------*/

#include "FreeRTOS.h"
#include "port.h"
#include "task.h"
#include "main.h"

/* Private typedef ------------------------------------------------------------*/

/* Private define -------------------------------------------------------------*/

#define TASK1_STACK_SIZE             (unsigned short)128
#define TASK2_STACK_SIZE             (unsigned short)128

/* Private variables ----------------------------------------------------------*/

StackType_t Task1Stack[TASK1_STACK_SIZE];
StackType_t Task2Stack[TASK2_STACK_SIZE];
StackType_t IdleTaskStack[configMINIAL_STACK_SIZE];

struct xLIST List_Test;
struct xLIST_ITEM List_Item1;
struct xLIST_ITEM List_Item2;
struct xLIST_ITEM List_Item3;

portCHAR flag0, flag1, flag2;

extern ListItem_t pxReadyTaskLists[configMAX_PRIORITIES];

TCB_t Task1TCB;
TCB_t Task2TCB;
TCB_t IdleTaskTCB;

TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t IdleTaskHandle;

/* Private function prototypes ----------------------------------------------*/

void delay(uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

/* Private user code ---------------------------------------------------------*/

/**
   * @brief  The application entry point.
   * @retval int
   */
int main(void)
{
  prvInitialiseTaskLists();

  Task1_Handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry,
                                                          (char *)"Task1",
                                                          (uint32_t)TASK1_STACK_SIZE,
                                                          (void *)NULL,
                                                          (StackType_t *)Task1Stack,
                                                          (TCB_t *)&Task1TCB);
  vListInsertEnd(&(pxReadyTaskLists[1]), &(((TCB_t *)(&Task1TCB))->xStateListItem));

  Task2_Handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry,
                                                          (char *)"Task2",
                                                          (uint32_t)TASK2_STACK_SIZE,
                                                          (void *)NULL,
                                                          (StackType_t *)Task2Stack,
                                                          (TCB_t *)&Task2TCB);
  vListInsertEnd(&(pxReadyTaskLists[2]), &(((TCB_t *)(&Task2TCB))->xStateListItem));

  vTaskStartScheduler();

  for (;;)
  {
    /*啥事不干*/
  }
}

/**
   * @brief  Soft delay.
   * @param count: determine how long it delay.
   * @retval None
   */
void delay(uint32_t count)
{
  for (; count != 0; count--)
    ;
}

/**
   * @brief  Task1 function.
   * @param p_arg: Pointer to Task Function.
   * @retval None
   */
void Task1_Entry(void *p_arg)
{
  for (;;) 
  {
    flag1 = 1;
    vTaskDelay(2);
    flag2 = 0;
    vTaskDelay(2);

    taskYIELD();
  }
}

/**
   * @brief  Task2 function.
   * @param p_arg: Pointer to Task Function.
   * @retval None
   */
void Task2_Entry(void* p_arg)
{
  for (;;) 
  {
    flag2 = 1;
    vTaskDelay(2);
    flag2 = 0;
    vTaskDelay(2);

    taskYIELD();
  }
}

void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
                                                             StackType_t **ppxIdleTaskStackBuffer,
                                                             uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
  *ppxIdleTaskStackBuffer = IdleTaskStack;
  *pulIdleTaskStackSize = configMINIAL_STACK_SIZE;
}