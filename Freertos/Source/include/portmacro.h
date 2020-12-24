#ifndef PORTMACRO_H
#define PORTMACRO_H

#include <stdint.h>
#include <stddef.h>
#include "FreeRTOSConfig.h"

#define portCHAR               char
#define portFLOAT              float
#define portDOUBLE           double
#define portLONG               long
#define portSHORT             short
#define portSTACK_TYPE    uint32_t
#define portBASE_TYPE       long

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if (configUSE_16_BIT_TICKS == 1)
typedef uint16_t TickType_t;
#define portMAX_DELAY (TickType_t) 0xffff
#else
typedef uint32_t TickType_t;
#define portMAX_DELAY (TickType_t) 0xffffffffUL
#endif

#define portNVIC_INT_CTRL_REG   (*(volatile uint32_t *)0xe000ed04)
#define portNVIC_PENDSVSET_BIT (1UL << 28UL)
#define portSY_FULL_READ_WRITE 15
#define portYIELD()                                                            \
{                                                                                          \
  portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
  __asm volatile("dsb" ::: "memory");                                    \
  __asm volatile("isb");                                                        \
}

#define portINLINE __inline

#ifndef portFORCE_INLINE
#define portFORCE_INLINE inline __attribute__(( always_inline ))
#endif

#define portDISABLE_INTERRUPTS()                           vPortRaiseBASEPRI()
#define portSET_INTERRUPT_MASK_FROM_ISR()       ulPortRaiseBASEPRI()
#define portENABLE_INTERRUPTS()                            vPortSetBASEPRI(0)
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortSetBASEPRI(x)
#define portENTER_CRITICAL()                                   vPortEnterCritical()
#define portEXIT_CRITICAL()                                       vPortExitCritical()

#define portTASK_FUNCTION(vFunction, pvParameters) void vFunction(void *pvParameters)

portFORCE_INLINE static void vPortRaiseBASEPRI(void)
{
  uint32_t ulNewBASEPRI;
  __asm volatile
  (
  "mov %0, %1       \n"
  "msr basepri, %0 \n"
  "isb                      \n"
  "dsb                     \n"
  : "=r"(ulNewBASEPRI)
  : "i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
  : "memory"
  );
}

portFORCE_INLINE static uint32_t ulPortRaiseBASEPRI(void)
{
  uint32_t ulReturn, ulNewBASEPRI;
  __asm volatile
  (
  "mrs %0, basepri \n"
  "mov %1, %2       \n"
  "msr basepri, %1 \n"
  "isb                      \n"
  "dsb                     \n"
  : "=r"(ulReturn), "=r"(ulNewBASEPRI)
  : "i"(configMAX_SYSCALL_INTERRUPT_PRIORITY)
  : "memory"
  );
  return ulReturn;
}

portFORCE_INLINE static void vPortSetBASEPRI(uint32_t ulBASEPRI)
{
  __asm volatile
  (
  "msr basepri, %0"::"r" (ulBASEPRI) : "memory"
  );
}

portFORCE_INLINE static void vPortClearBASEPRIFromISR(void)
{
  __asm volatile
  (
  "msr basepri, %0"::"r"(0)
  );
}

#endif //PORTMACRO_H
