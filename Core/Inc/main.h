#ifndef MAIN_H
#define MAIN_H

void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
	                                                          StackType_t **ppxIdleTaskStackBuffer,
	                                                          uint32_t *pulIdleTaskStackSize);

#endif
