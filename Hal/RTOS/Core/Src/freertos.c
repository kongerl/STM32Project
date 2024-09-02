/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "bsp.h"
#define LOG_TAG    "RTOS"
#include <elog.h>
#include "dht11.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

typedef struct htsensor_t{
  uint8_t humi;     //温度
  uint8_t temp;   //湿度
}htsensor_t;
htsensor_t g_htsensor;
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
/* Definitions for Task_Dht11 */
osThreadId_t Task_Dht11Handle;
const osThreadAttr_t Task_Dht11_attributes = {
  .name = "Task_Dht11",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED */
osThreadId_t Task_LEDHandle;
const osThreadAttr_t Task_LED_attributes = {
  .name = "Task_LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for Task_Info */
osThreadId_t Task_InfoHandle;
const osThreadAttr_t Task_Info_attributes = {
  .name = "Task_Info",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void App_Task_Dht11(void *argument);
void App_Task_LED(void *argument);
void App_Task_Info(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Dht11 */
  Task_Dht11Handle = osThreadNew(App_Task_Dht11, NULL, &Task_Dht11_attributes);

  /* creation of Task_LED */
  Task_LEDHandle = osThreadNew(App_Task_LED, NULL, &Task_LED_attributes);

  /* creation of Task_Info */
  Task_InfoHandle = osThreadNew(App_Task_Info, NULL, &Task_Info_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_App_Task_Dht11 */
/**
  * @brief  Function implementing the Task_Dht11 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_App_Task_Dht11 */
void App_Task_Dht11(void *argument)
{
  /* USER CODE BEGIN App_Task_Dht11 */

  TickType_t pxPreviousWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {

    if(DHT11_Read_Data(&g_htsensor.temp,&g_htsensor.humi) == 0)
    {
      log_i("ReadOk,hum = %d,Temp = %d\n",g_htsensor.humi,g_htsensor.temp);
    }
    else
    {
      log_e("ReadError!\n");
    }

    vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(1000)); // 500ms delay

  }
  /* USER CODE END App_Task_Dht11 */
}

/* USER CODE BEGIN Header_App_Task_LED */
/**
* @brief Function implementing the Task1_LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_App_Task_LED */
void App_Task_LED(void *argument)
{
  /* USER CODE BEGIN App_Task_LED */
  TickType_t pxPreviousWakeTime = xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
    LED_Toggle();
    vTaskDelayUntil(&pxPreviousWakeTime, pdMS_TO_TICKS(500)); // 500ms delay
    //osDelay(500);‘
    
  }
  /* USER CODE END App_Task_LED */
}

/* USER CODE BEGIN Header_App_Task_Info */
/**
* @brief Function implementing the Task_Info thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_App_Task_Info */
void App_Task_Info(void *argument)
{
  /* USER CODE BEGIN App_Task_Info */

  //----------------------------------------------------------------
  //获取当前的系统时钟计数值（Tick数）
  TickType_t pxPreviousWakeTime = xTaskGetTickCount();

  //----------------------------------------------------------------
  //获取特定任务的信息
  TaskHandle_t taskHandle = Task_Dht11Handle;
  TaskStatus_t taskInfo;
  BaseType_t getFreeStackSpace = pdTRUE;
  eTaskState taskState = eInvalid;
  vTaskGetInfo(taskHandle, &taskInfo, getFreeStackSpace, taskState);

  taskENTER_CRITICAL();//进入临界代码段

  //----------------------------------------------------------------
  //打印特定任务信息
  log_a("Task_Info");
  log_i("Task Name :            %s", taskInfo.pcTaskName);
  log_i("task Number:           %d", &taskInfo.xTaskNumber);
  log_i("Task State:            %d", taskInfo.eCurrentState);
  log_i("Task Priority:         %d", taskInfo.uxCurrentPriority);
  log_i("Stack High Water Mark: %d(Word)", taskInfo.usStackHighWaterMark);

  //----------------------------------------------------------------
  //获取空闲任务和特定任务的堆栈高水位值
  taskHandle = xTaskGetIdleTaskHandle();
  log_a("StackHighWaterMark:");
  log_i("Idle task =        %d",uxTaskGetStackHighWaterMark(taskHandle));
  log_i("Task_Dht11Handle = %d",uxTaskGetStackHighWaterMark(Task_Dht11Handle));
  log_i("Task_LEDHandle =   %d",uxTaskGetStackHighWaterMark(Task_LEDHandle));
  log_i("Task_InfoHandle =  %d",uxTaskGetStackHighWaterMark(Task_InfoHandle));

  //----------------------------------------------------------------
  //输出内核信息和任务列表
  log_a("Kernel Info:");
  log_i("uxTaskGetNumberOfTasks = %d", uxTaskGetNumberOfTasks());

  char pcWriteBuffer[300];
  vTaskList(pcWriteBuffer);
  log_i("Task List:%s",pcWriteBuffer);


  taskEXIT_CRITICAL();//退出临界代码段


  /* Infinite loop */
  //for(;;)
  //{
   // osDelay(1);
    for(uint8_t i = 0; i < 10; ++i)
    {
      LED_Toggle();
      osDelay(100);
    }
  //}

  log_a("Task_uart is deleted!");
  vTaskDelete(NULL);//为NULL是删除当前任务

  /* USER CODE END App_Task_Info */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

