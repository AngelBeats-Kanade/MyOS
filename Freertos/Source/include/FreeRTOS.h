#ifndef FREERTOS_H
#define FREERTOS_H

#include "FreeRTOSConfig.h"
#include "list.h"
#include "port.h"
#include "projdefs.h"

#ifndef configUSE_TIME_SLICING
#define configUSE_TIME_SLICING 1
#endif

typedef struct tsTaskControlBlock {
  volatile StackType_t *pxTopOfStack;
  ListItem_t xStateListItem;
  StackType_t *pxStack;
  char pcTaskName[configMAX_TASK_NAME_LEN];
  TickType_t xTicksToDelay;
  UBaseType_t uxPriority;
} tskTCB;

typedef tskTCB TCB_t;

#endif //FREERTOS_H