#ifndef FREERTOS_H
#define FREERTOS_H

#include "FreeRTOSConfig.h"
#include "list.h"
#include "port.h"
#include "projdefs.h"

typedef struct tsTaskControlBlock {
  volatile StackType_t *pxTopOfStack;
  ListItem_t xStateListItem;
  StackType_t *pxStack;
  char pcTaskName[configMAX_TASK_NAME_LEN];
  TickType_t xTicksToDelay;
} tskTCB;

typedef tskTCB TCB_t;

#endif //FREERTOS_H