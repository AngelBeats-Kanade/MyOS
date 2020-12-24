#include "task.h"

TCB_t *volatile pxCurrentTCB = NULL;
List_t pxReadyTaskLists[configMAX_PRIORITIES];
static volatile UBaseType_t uxCurrentNumberOfTasks = (UBaseType_t)0U;
static TaskHandle_t xIdleTaskHandle = NULL;
static volatile TickType_t xTickCount = (TickType_t)0U;

extern portCHAR flag0;
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t IdleTaskTCB;

static portTASK_FUNCTION(prvIdleTask, pvParameters);

TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
	                                                const char* const pcName,
	                                                const uint32_t ulStackDepth,
	                                                void* const pvParameters,
	                                                StackType_t* const puxStackBuffer,
	                                                TCB_t* const pxTaskBuffer)
{
  TCB_t *pxNewTCB;
  TaskHandle_t xReturn;

  if ((pxTaskBuffer != NULL) && (puxStackBuffer != NULL))
  {
    pxNewTCB = (TCB_t *)pxTaskBuffer;
    pxNewTCB->pxStack = (StackType_t *)puxStackBuffer;
    prvInitialiseNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters,
                         &xReturn, pxNewTCB);
  } else {
    xReturn = NULL;
  }

  return xReturn;
}

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                                    const char *const pcName,
                                                    const uint32_t ulStackDepth,
                                                    void *const pvParameters,
                                                    TaskHandle_t *const pxCreatedTask,
                                                    TCB_t *pxNewTCB) 
{
  StackType_t *pxTopOfStack;
  UBaseType_t x;

  pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);
  pxTopOfStack = (StackType_t *)(((uint32_t)pxTopOfStack) & (~((uint32_t)0x0007)));
  
  for (x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; x++)
  {
    pxNewTCB->pcTaskName[x] = pcName[x];
    if (pcName[x] == 0x00)
      break;
  }
  pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN - 1] = '\0';
  vListInitialiseItem(&(pxNewTCB->xStateListItem));
  listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem), pxNewTCB);
  pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack, pxTaskCode, pvParameters);

  if ((void *)pxCreatedTask != NULL)
    *pxCreatedTask = (TaskHandle_t)pxNewTCB;
}

void prvInitialiseTaskLists(void)
{ 
  for ( UBaseType_t uxPriority = (UBaseType_t)0U;
          uxPriority < (UBaseType_t)configMAX_PRIORITIES;
          uxPriority++)
  {
    vListInitialise(&(pxReadyTaskLists[uxPriority]));
  }
}

void vTaskStartScheduler(void)
{ 
  TCB_t *pxIdleTaskTCBBuffer = NULL;
  StackType_t *pxIdleTaskStackBuffer = NULL;
  uint32_t ulIdleTaskStackSize;

  vApplicationGetIdleTaskMemory(&pxIdleTaskTCBBuffer,
                                                       &pxIdleTaskStackBuffer,
                                                       &ulIdleTaskStackSize);

  xIdleTaskHandle = xTaskCreateStatic((TaskFunction_t)prvIdleTask,
                                                              (char *)"IDLE",
                                                              (uint32_t)ulIdleTaskStackSize,
                                                              (void *)NULL,
                                                              (StackType_t *)pxIdleTaskStackBuffer,
                                                              (TCB_t *)pxIdleTaskTCBBuffer);

  vListInsertEnd(&pxReadyTaskLists[0],
                         &(((TCB_t *)pxIdleTaskTCBBuffer)->xStateListItem));

  pxCurrentTCB = &Task1TCB;

  xTickCount = (TickType_t)0U;

  if (xPortStartScheduler() != pdFALSE)
  {
    flag0 = 1;
  }
}

void vTaskSwitchContext(void)
{
  if (pxCurrentTCB == &IdleTaskTCB)
  {
    if (Task1TCB.xTicksToDelay == 0) {
      pxCurrentTCB = &Task1TCB;
    } else if (Task2TCB.xTicksToDelay == 0) {
      pxCurrentTCB = &Task2TCB;
    } else {
      return;
    }
  } else {
    if (pxCurrentTCB == &Task1TCB)
    {
      if (Task2TCB.xTicksToDelay == 0) 
        pxCurrentTCB == &Task2TCB;
      else if (pxCurrentTCB->xTicksToDelay != 0)
        pxCurrentTCB = &IdleTaskTCB;
      else 
        return;
    } else if (pxCurrentTCB == &Task2TCB) {
      if (Task2TCB.xTicksToDelay == 0)
        pxCurrentTCB = &Task1TCB;
      else if (pxCurrentTCB->xTicksToDelay != 0)
        pxCurrentTCB = &IdleTaskTCB;
      else
        return;
    }
  }
}

static portTASK_FUNCTION(prvIdleTask, pvParameters) 
{
  (void)pvParameters;

  for (;;) {}
}

void vTaskDelay(TickType_t const xTicksToDelay)
{ 
  TCB_t *pxTCB = pxCurrentTCB;
  pxTCB->xTicksToDelay = xTicksToDelay;
  taskYIELD();
}

void xTaskIncrementTick(void)
{ 
  TCB_t *pxTCB = NULL;
  BaseType_t i = 0;

  TickType_t const xConstTickCount = xTickCount + 1;
  xTickCount = xConstTickCount;

  for (i = 0; i < configMAX_PRIORITIES; i++)
  {
    pxTCB = (TCB_t *)listGET_OWNER_OF_HEAD_ENTRY(&pxReadyTaskLists[i]);
    if (pxTCB->xTicksToDelay > 0)
      pxTCB->xTicksToDelay--;
  }
  portYIELD();
}