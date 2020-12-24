#ifndef PORT_H
#define PORT_H

#include "projdefs.h"

StackType_t *pxPortInitialiseStack(StackType_t *, TaskFunction_t, void *);
BaseType_t xPortStartScheduler(void);

#endif //PORT_H