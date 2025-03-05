/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for myTask02 */
osThreadId_t myTask02Handle;
const osThreadAttr_t myTask02_attributes = {
  .name = "myTask02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myQueue01 */
osMessageQueueId_t myQueue01Handle;
const osMessageQueueAttr_t myQueue01_attributes = {
  .name = "myQueue01"
};
/* Definitions for myTimer01 */
osTimerId_t myTimer01Handle;
const osTimerAttr_t myTimer01_attributes = {
  .name = "myTimer01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartTask02(void *argument);
void Callback01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* USER CODE BEGIN PREPOSTSLEEP */
__weak void PreSleepProcessing(uint32_t ulExpectedIdleTime)
{
/* place for user code */
}

__weak void PostSleepProcessing(uint32_t ulExpectedIdleTime)
{
/* place for user code */
}
/* USER CODE END PREPOSTSLEEP */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of myTimer01 */
  myTimer01Handle = osTimerNew(Callback01, osTimerPeriodic, NULL, &myTimer01_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of myQueue01 */
  myQueue01Handle = osMessageQueueNew (16, sizeof(uint16_t), &myQueue01_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of myTask02 */
  myTask02Handle = osThreadNew(StartTask02, NULL, &myTask02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	uint16_t value = 0;
	uint8_t pri;

  /* Infinite loop */
	for(;;) {
			osDelay(100*portTICK_PERIOD_MS);
		}
	while(1) {
		if(osMessageQueueGet(myQueue01Handle, &value, &pri, osWaitForever) == osOK) {
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
			while(value) {
				HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
				osDelay(5/portTICK_PERIOD_MS);
				HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
				osDelay(5*portTICK_PERIOD_MS);
				value--;
			}
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
		}
	}
  /*for(;;)
  {
	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
	osDelay(20*portTICK_PERIOD_MS);
	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
	osDelay(20*portTICK_PERIOD_MS);

    msg_stat = osMessageQueueGet(myQueue01Handle, &value, &pri, osWaitForever);
    if(msg_stat == osOK) {
    	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
    	while(value) {
    		HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
    		osDelay(5*portTICK_PERIOD_MS);
    		HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_SET);
    		value--;
    	}
    	HAL_GPIO_WritePin(DBG1_GPIO_Port, DBG1_Pin, GPIO_PIN_RESET);
    }
  }*/
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_StartTask02 */
/**
* @brief Function implementing the myTask02 thread.
* @param argument: Not used
* @retval None
*/
#include "main.h"
/* USER CODE END Header_StartTask02 */
void StartTask02(void *argument)
{
  /* USER CODE BEGIN StartTask02 */
	static uint16_t muahaha = 0;
  /* Infinite loop */
	for(;;) {
		osDelay(500/portTICK_PERIOD_MS);
		/*muahaha = 1;
		osMessageQueuePut(myQueue01Handle, &muahaha, 0, osWaitForever);
		muahaha = 2;
		osMessageQueuePut(myQueue01Handle, &muahaha, 0, osWaitForever);
		muahaha = 3;
		osMessageQueuePut(myQueue01Handle, &muahaha, 0, osWaitForever);
		muahaha = 5;
		osMessageQueuePut(myQueue01Handle, &muahaha, 0, osWaitForever);*/
	}/*
	for(;;) {
		HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
		osDelay(100*portTICK_PERIOD_MS);
		HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_SET);

		osMessageQueuePut(myQueue01Handle, &muahaha, 0, osWaitForever);

		muahaha++;
		if(muahaha == 5) {
			muahaha = 0;
		}
		HAL_GPIO_WritePin(DBG0_GPIO_Port, DBG0_Pin, GPIO_PIN_RESET);
	}*/
  /* USER CODE END StartTask02 */
}

/* Callback01 function */
void Callback01(void *argument)
{
  /* USER CODE BEGIN Callback01 */

  /* USER CODE END Callback01 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

