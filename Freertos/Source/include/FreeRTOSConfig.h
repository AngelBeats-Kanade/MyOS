#ifndef FREERTOSCONFIG_H
#define FREERTOSCONFIG_H

#include "stm32f1xx.h"

#define configUSE_16_BIT_TICKS               0
#define configMAX_TASK_NAME_LEN              16
#define configSUPPORT_STATIC_ALLOCATION      1
#define configMAX_PRIORITIES                 5
#define configKERNEL_INTERRUPT_PRIORITY      255
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 191
#define configUSE_PREEMPTION                 1

#define configMINIAL_STACK_SIZE ((unsigned short)128)
#define configCPU_CLOCK_HZ      SystemCoreClock
#define configTICK_RATE_HZ      ((TickType_t) 1000)

#define xPortPendSVHandler   PendSV_Handler
#define vPortSVCHandler      SVC_Handler

#endif //FREERTOSCONFIG_H
