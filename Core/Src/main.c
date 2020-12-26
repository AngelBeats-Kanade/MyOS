/**
  ****************************************************************************
  * @file         : main.c
  * @brief        : Main program body
  ****************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 AngelBeats.
  * All rights reserved.</center></h2>
  *
  ****************************************************************************
  */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
/* Private includes -----------------------------------------------------------*/

#include "gpio.h"
#include "main.h"
#include "port.h"
#include "task.h"
#include "usart.h"

/* Private typedef ------------------------------------------------------------*/

/* Private define -------------------------------------------------------------*/

#define TASK1_STACK_SIZE             (unsigned short)128
#define TASK2_STACK_SIZE             (unsigned short)128

/* Private variables ----------------------------------------------------------*/

StackType_t Task1Stack[TASK1_STACK_SIZE];
StackType_t Task2Stack[TASK2_STACK_SIZE];
StackType_t IdleTaskStack[configMINIAL_STACK_SIZE];

extern ListItem_t pxReadyTaskLists[configMAX_PRIORITIES];

TCB_t Task1TCB;
TCB_t Task2TCB;
TCB_t IdleTaskTCB;

TaskHandle_t Task1_Handle;
TaskHandle_t Task2_Handle;
TaskHandle_t IdleTaskHandle;

/* Private function prototypes ----------------------------------------------*/

void SystemClock_Config(void);

void Task1_Entry(void *p_arg);

void Task2_Entry(void *p_arg);

/* Private user code ---------------------------------------------------------*/

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  HAL_Init();

  SystemClock_Config();

  MX_GPIO_Init();
  MX_USART1_UART_Init();

  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin | LED2_Pin, GPIO_PIN_RESET);

  printf("一切正常");

  prvInitialiseTaskLists();

  Task1_Handle = xTaskCreateStatic((TaskFunction_t) Task1_Entry,
                                   (char *) "Task1",
                                   (uint32_t) TASK1_STACK_SIZE,
                                   (void *) NULL,
                                   (UBaseType_t) 1,
                                   (StackType_t *) Task1Stack,
                                   (TCB_t *) &Task1TCB);

  Task2_Handle = xTaskCreateStatic((TaskFunction_t) Task2_Entry,
                                   (char *) "Task2",
                                   (uint32_t) TASK2_STACK_SIZE,
                                   (void *) NULL,
                                   (UBaseType_t) 2,
                                   (StackType_t *) Task2Stack,
                                   (TCB_t *) &Task2TCB);

  portDISABLE_INTERRUPTS();

  vTaskStartScheduler();

  for (;;)
  {
    /*啥事不干*/
  }
}

/**
  * @brief  Task1 function.
  * @param  p_arg: Pointer to Task Function.
  * @retval None
  */
void Task1_Entry(void *p_arg)
{
  for (;;)
  {
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    vTaskDelay(500);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    vTaskDelay(500);
  }
}

/**
  * @brief  Task2 function.
  * @param  p_arg: Pointer to Task Function.
  * @retval None
  */
void Task2_Entry(void *p_arg)
{
  for (;;)
  {
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    vTaskDelay(500);
    HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
    vTaskDelay(500);
  }
}

/**
  * @brief  Get the task memory.
  * @param  ppxIdleTaskTCBBuffer: Idle task TCB buffer.
  * @param  ppxIdleTaskStackBuffer: Idle task stack buffer.
  * @param  pulIdleTaskStackSize: Idle task stack size.
  * @retval None
  */
void vApplicationGetIdleTaskMemory(TCB_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
  *ppxIdleTaskTCBBuffer = &IdleTaskTCB;
  *ppxIdleTaskStackBuffer = IdleTaskStack;
  *pulIdleTaskStackSize = configMINIAL_STACK_SIZE;
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#pragma clang diagnostic pop