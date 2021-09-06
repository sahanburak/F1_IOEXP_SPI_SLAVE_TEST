/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "rt_bus_proto.h"
#include "io.h"
#include "ad529x.h"
#include "ssi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define SPI_BUF_SIZE 2048
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern SPI_HandleTypeDef hspi1;
extern DMA_HandleTypeDef hdma_spi1_rx;
extern DMA_HandleTypeDef hdma_spi1_tx;
/*uint8_t rData[SPI_BUF_SIZE];
uint8_t tData[SPI_BUF_SIZE];*/
uint16_t currentDMA = SPI_TX_BUF_SIZE;
uint16_t prevDMA = SPI_TX_BUF_SIZE;
uint8_t gRunMode = RT_RunMode_None;
uint8_t gStayInBootloader = 0;
extern uint8_t gSPI_Tx_Buf[SPI_TX_BUF_SIZE];
extern uint8_t gSPI_Rx_Buf[SPI_RX_BUF_SIZE];
uint8_t g_comms_mode = COMMS_MODE_PDIO;
extern tPDO g_PDO;
extern tPDI g_PDI;

int counter =0;
uint8_t currentStateB;
uint8_t  previousStateB;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void SPI_DMA_Reset();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_SPI3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	AD529x_Init();
	SH1107_Init();
	LP55231_Init();
	memset(gSPI_Tx_Buf,0,SPI_TX_BUF_SIZE);
	//HAL_SPI_TransmitReceive_DMA(&hspi1,gSPI_Tx_Buf, gSPI_Rx_Buf, SPI_TX_BUF_SIZE);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	dbprintf("Rota SPI Test Application");
#if 0
	uint8_t output_count = 0;
	while(1){
		io_update();
		g_PDI.dout = ~g_PDI.dout;
		HAL_GPIO_TogglePin(INT_LED1_GPIO_Port, INT_LED1_Pin);
		//HAL_Delay(1000);
	}

	while(1){
		//set_output(output);
		io_update_old(output_count++);
		if(output_count >= (12*8)){
			output_count = 0;
			g_PDI.dout = ~g_PDI.dout;
		}
		//HAL_Delay(2000);
		//dbprintf("Output Value: %d",output);
		HAL_GPIO_TogglePin(INT_LED1_GPIO_Port, INT_LED1_Pin);
	}

#endif

#if 0
	uint32_t time = 200;
	/* Led Driver Test */
	while(1){
		for(int i =1 ;i<4;i++){
			LP55231_SetColor(i,0x50FF0000);
			HAL_Delay(time);
		}
		for(int i =1 ;i<4;i++){
			LP55231_SetColor(i,0x5000FF00);
			HAL_Delay(time);
		}

		for(int i =1 ;i<4;i++){
			LP55231_SetColor(i,0x500000FF);
			HAL_Delay(time);
		}

		for(int i =1 ;i<4;i++){
			LP55231_SetColor(i,0x5033FFFF);
			HAL_Delay(time);
		}

		for(int i =1 ;i<4;i++){
			LP55231_SetColor(i,0x50FF3399);
			HAL_Delay(time);
		}
		SH1107_WriteLine("Rota Teknik  Rota Teknik  Rota Teknik   Rota Teknik", &Font_11x18, White);
		/*if(HAL_GetTick() >= next)
		{
			next += 1000;

			dbprintf("Illuminating: %d",current);

			LP55231_SetChannelPWM(previous, 0);
			LP55231_SetChannelPWM(current, 50);

			previous = current;
			current++;
			if(current >= LP55231_NumChannels)
			{
				current = 0;
			}
		}*/


	}
#endif

#if 0
	/*Encoder Test*/
	previousStateB = HAL_GPIO_ReadPin(Encoder_Phase_B_GPIO_Port, Encoder_Phase_B_Pin);
	//SH1107_WriteString("Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345Deneme12345", &Font_11x18, White);
	//SH1107_WriteString("Deneme1234Deneme1234Deneme1234Deneme1234Deneme1234", &Font_11x18, White);
	/*SH1107_WriteLine("Deneme12345", &Font_11x18, White);
	SH1107_WriteLine("DenemeDeneme", &Font_11x18, White);*/
	while(1){
		currentStateB = HAL_GPIO_ReadPin(Encoder_Phase_B_GPIO_Port, Encoder_Phase_B_Pin);
		if(currentStateB != previousStateB){
			if(HAL_GPIO_ReadPin(Encoder_Phase_A_GPIO_Port, Encoder_Phase_A_Pin) != currentStateB){
				counter--;
				dbprintf("Rotate CCW\t value: %d",counter);
			}else{
				counter++;
				dbprintf("Rotate CW\t value: %d",counter);
			}
		}
		previousStateB = currentStateB;
	}
#endif
#if 0
	/* Digital pot test */
	int resVal = 0;
	int tempOut = 0;
	while(1){
		/*0io_update();
		if(tempOut != g_PDI.dout){
			dbprintf("Output : 0x%08X", g_PDI.dout);
			tempOut = g_PDI.dout;
		}
		g_PDI.dout = ~g_PDI.dout;
		HAL_Delay(1000);*/
		//resVal += 100;
		resVal =4000;
		AD529x_SetResistor(resVal);
		if(resVal > MAX_RES_VALUE)
			resVal = 0;
		HAL_Delay(20);
	}
#endif
#if 1
	float resVal = 0.0f;
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		g_PDO.ssi = ssi_read();
		memcpy(&resVal, &g_PDI.resistor,sizeof(float));
		resVal *= 1000;
		if(resVal > MAX_RES_VALUE)
			resVal = MAX_RES_VALUE;
		AD529x_SetResistor(resVal);
		if(g_comms_mode == COMMS_MODE_PDIO){
			rt_get_io_values();
		}/*else{
			//rt_bus_proto_bl_dt();
		}*/
	}
#endif
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
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
	case STM_MODE_SEL_Pin:
		if(HAL_GPIO_ReadPin(STM_MODE_SEL_GPIO_Port, STM_MODE_SEL_Pin)){
			g_comms_mode = COMMS_MODE_PDIO;
			dbprintf("PDIO Mode");
		}else{
			g_comms_mode = COMMS_MODE_CONF;
			dbprintf("CONF Mode");
		}
		break;

	case EXT_BTN1_Pin:
		dbprintf("BTN1 Pressed.");
		break;
	case EXT_BTN2_Pin:
		dbprintf("BTN2 Pressed.");
		break;
	case Encoder_SW_Pin:
		dbprintf("Encoder Button Pressed.");
		break;
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	dbprintf("%s",__func__);
	SPI_DMA_Reset();
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
	dbprintf("%s",__func__);
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
