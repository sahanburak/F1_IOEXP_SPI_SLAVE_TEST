/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2019 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        io.c
 * @brief       
 *
 * @author      Burak Sahan
 * @date        Dec 10, 2020
 *
 * @ingroup     F1_CompactController_IOExp_Application
 * @{
 *****************************************************************************/

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "stm32f1xx_hal.h"
#include "io.h"
#include "main.h"
#include "spi.h"
#include <stdint.h>
#include <stm32f1xx_hal_def.h>
/*============================================================================*/
/* Forward declarations                                                       */
/*============================================================================*/
GPIO_TypeDef* DIRECT_OUTPUT_PORT_LIST[7]={
		DAC_20mA_SEL_GPIO_Port,
		DAC_40mA_SEL_GPIO_Port,
		DAC_100mA_SEL_GPIO_Port,
		LCD_RES_GPIO_Port,
		ADC_Current_INP_Ch1_GPIO_Port,
		ADC_Current_INP_Ch3_GPIO_Port,
		ADC_Current_INP_Ch5_GPIO_Port};

uint16_t DIRECT_OUTPUT_PIN_LIST[7]={
		DAC_20mA_SEL_Pin,
		DAC_40mA_SEL_Pin,
		DAC_100mA_SEL_Pin,
		LCD_RES_Pin,
		ADC_Current_INP_Ch1_Pin,
		ADC_Current_INP_Ch3_Pin,
		ADC_Current_INP_Ch5_Pin};

#define DIRECT_OUTPUT_COUNT sizeof(DIRECT_OUTPUT_PIN_LIST)/sizeof(uint16_t)
#define INDIRECT_OUTPUT_COUNT 8
/*============================================================================*/
/* Constants and macros                                                       */
/*============================================================================*/

/*============================================================================*/
/* Type definitions                                                           */
/*============================================================================*/

/*============================================================================*/
/* Global data                                                                */
/*============================================================================*/
tPDO g_PDO;
tPDI g_PDI;
tPDO prev_PDO;
tPDI prev_PDI;
uint8_t flag = 0;
/*============================================================================*/
/* Module global data                                                         */
/*============================================================================*/

/*============================================================================*/
/* Implementation of functions                                                */
/*============================================================================*/

/* SPI3 init function */
void MX_SPI3_ReInit(void)
{
	HAL_SPI_Abort(&hspi3);
	hspi3.Instance = SPI3;
	hspi3.Init.Mode = SPI_MODE_MASTER;
	hspi3.Init.Direction = SPI_DIRECTION_2LINES;
	hspi3.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi3.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi3.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi3.Init.NSS = SPI_NSS_SOFT;
	hspi3.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi3.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi3.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi3.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi3.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi3) != HAL_OK)
	{
		Error_Handler();
	}

}


void io_init(void)
{
	prev_PDI.dout = 0;
	io_update();
}

void io_do(uint8_t ch, uint8_t val)
{
	if (val )
	{
		g_PDI.dout |= (1<<ch);
	}
	else
	{
		g_PDI.dout &= (~(1<<ch));
	}
}

void Store_Outputs(){
	HAL_GPIO_WritePin(SHIFT_REG_CS_GPIO_Port, SHIFT_REG_CS_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SHIFT_REG_CS_GPIO_Port, SHIFT_REG_CS_Pin, GPIO_PIN_SET);
}

void Load_Inputs(){
	HAL_GPIO_WritePin(SHIFT_REG_LATCH_GPIO_Port, SHIFT_REG_LATCH_Pin, 0);
	HAL_GPIO_WritePin(SHIFT_REG_LATCH_GPIO_Port, SHIFT_REG_LATCH_Pin, 1);
}
uint32_t tempOut =0;
uint8_t io_update(void)
{
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t rxData[2] = {0};
	uint8_t txData[2] = {(uint8_t)(g_PDI.dout & 0xFF),(uint8_t)(g_PDI.dout & 0xFF)};

	Load_Inputs();
	HAL_SPI3_ReadWrite(IOEXP_SPI3_SLV,&txData[0], &rxData[0], 2);

	Store_Outputs();

	uint8_t direct_output = 0;
	for(direct_output=0;direct_output<DIRECT_OUTPUT_COUNT;direct_output++){
		HAL_GPIO_WritePin(DIRECT_OUTPUT_PORT_LIST[direct_output],DIRECT_OUTPUT_PIN_LIST[direct_output], (((g_PDI.dout>>8) & (1 << (direct_output))) >> (direct_output)));
	}
	memcpy(&g_PDO.din,&rxData[0],2);
	return status;
}


/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2019,  All Rights Reserved. Confidential.      */
/******************************************************************************/
