/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2019 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        ad529x.c
 * @brief       
 *
 * @author      Burak Sahan
 * @date        Feb 18, 2021
 *
 * @ingroup     F1_IOEXP_SPI_SLAVE_TEST
 * @{
 *****************************************************************************/

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "ad529x.h"
/*============================================================================*/
/* Forward declarations                                                       */
/*============================================================================*/

/*============================================================================*/
/* Constants and macros                                                       */
/*============================================================================*/

/*============================================================================*/
/* Type definitions                                                           */
/*============================================================================*/

/*============================================================================*/
/* Global data                                                                */
/*============================================================================*/

/*============================================================================*/
/* Module global data                                                         */
/*============================================================================*/

/*============================================================================*/
/* Implementation of functions                                                */
/*============================================================================*/
HAL_StatusTypeDef SPI_Write(int id, uint8_t *pData, uint16_t Size){
	HAL_StatusTypeDef status = HAL_ERROR;
	uint8_t rData[2];
	HAL_SPI3_ReadWrite(DIGIPOT_SPI3_SLV,pData,&rData[0],Size);
	/*
	AD529x_CS_LOW();
	status = HAL_SPI_Transmit(&hspi3, pData, Size, 10000);
	HAL_Delay(20);
	AD529x_CS_HIGH();*/
	return status;
}


void AD529x_SetRegister(unsigned char command, unsigned short data)
{
	unsigned char dataBuf[2] = {0, 0};
	data &= 0x3FF;
	command &= 0x0F;
	dataBuf[0] = (command << 2) + ((data & 0x0300) >> 8);
	dataBuf[1] = (data & 0x00FF);
	SPI_Write(AD529x_SLAVE_ID, dataBuf, 2);

}


void AD529x_SetResistor(uint32_t res)
{
	uint32_t value = ((MAX_RES_VALUE-res)  * 1024 ) / MAX_RES_VALUE;
	if(res == 0){
		value = 1023;
	}
	//dbprintf("*** Write Pot Value : %d (%d ohm)\n\r",value,res);
	AD529x_SetRegister(AD529x_WRITE_RDAC, value);

}


char AD529x_Init()
{
	char status = -1;

	//AD527x_RESET_OUT;
	//AD527x_RESET_HIGH;

	/* setup AD527x */
	/* Calibration mode set to normal mode. RDAC register write proction closed. */
	AD529x_SetRegister(AD529x_WRITE_CONTENT,(AD529x_C2 | AD529x_C1));

	return status;
}


/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2019,  All Rights Reserved. Confidential.      */
/******************************************************************************/
