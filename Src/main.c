/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usart.h"
#include "gpio.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "math.h"
#include "time.h"
#include "los_task.h"
//#include "los_api_task.h"
//#include "los_inspect_entry.h"

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static UINT32 g_uwTskHiID;
#define TSK_PRIOR_HI 8

static UINT32 Example_TaskHi(VOID)
{
    UINT32 uwRet = LOS_OK;

    printf("Enter TaskHi Handler.\r\n");

    /* task delay 5 ticks, task will be suspend */
		while(1)
		{
			uwRet = LOS_TaskDelay(2);
			printf("this is a test thread\r\n");
//			HAL_Delay(2000);
			if (uwRet != LOS_OK)
			{
					printf("Delay Task Failed.\r\n");
					return LOS_NOK;
			}
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		}
    

    /* task resumed */
//    dprintf("TaskHi LOS_TaskDelay Done.\r\n");

    /* suspend self */
//    uwRet = LOS_TaskSuspend(g_uwTskHiID);
//    if (uwRet != LOS_OK)
//    {
//        dprintf("Suspend TaskHi Failed.\r\n");
//        uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK, LOS_INSPECT_STU_ERROR);
//        if (LOS_OK != uwRet)
//        {
//            dprintf("Set Inspect Status Err.\r\n");
//        }
//        return LOS_NOK;
//    }

//    dprintf("TaskHi LOS_TaskResume Success.\r\n");

//    uwRet = LOS_InspectStatusSetByID(LOS_INSPECT_TASK, LOS_INSPECT_STU_SUCCESS);
//    if (LOS_OK != uwRet)
//    {
//        dprintf("Set Inspect Status Err.\r\n");
//    }

//    /* delete self */
//    if(LOS_OK != LOS_TaskDelete(g_uwTskHiID))
//    {
//        dprintf("TaskHi delete failed .\r\n");
//        return LOS_NOK;
//    }

    return LOS_OK;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	GPIO_TypeDef* GPIO_PORT[] = {LED1_GPIO_Port};
	const uint16_t GPIO_PIN[] = {LED1_Pin};
	UINT32 uwRet = LOS_OK;
  TSK_INIT_PARAM_S stInitParam;
	
	
  /* USER CODE END 1 */
  
	
  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	
	printf("begin test uart\r\n");
	LOS_TaskLock();
	
	stInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
	stInitParam.usTaskPrio = TSK_PRIOR_HI;
	stInitParam.pcName = "HIGH_NAME";
	stInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
	/* create high prio task */
	uwRet = LOS_TaskCreate(&g_uwTskHiID, &stInitParam);
	if (uwRet != LOS_OK)
	{
			LOS_TaskUnlock();

			printf("Example_TaskHi create Failed!\r\n");
			return LOS_NOK;
	}
	printf("Example_TaskHi create Success!\r\n");
	
	/* unlock task schedue */
  LOS_TaskUnlock();
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    HAL_GPIO_TogglePin(GPIO_PORT[0], GPIO_PIN[0]);
		HAL_Delay(2000);
		/* USER CODE END WHILE */
		
    /* USER CODE BEGIN 3 */
		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, 10);
	return ch;
}
/* USER CODE END 4 */

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

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
