/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2019 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        ssi.c
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
#include "spi.h"
#include <string.h>
#include "ssi.h"
#include "usart.h"
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
uint32_t count = 0;
extern uint8_t gSPI_Tx_Buf[2048];
uint32_t ssi_read()
{

	uint8_t ssiData[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};
	uint32_t data;
	HAL_StatusTypeDef ret = HAL_SPI_Receive(&hspi2,ssiData,5,1000);
	if(ret != HAL_OK){
		return 0xFFFFFFFF;
	}

	if(ssiData[0] == 0xFF && ssiData[1] == 0xFF && ssiData[2] == 0xFF && ssiData[3] == 0xFF  && ssiData[4] == 0xFF ){
		return 0xFFFFFFFF;
	}else{
		/*data = (ssiData[1]);
		data = (data << 8) | (ssiData[2]);
		data = (data << 8) | (ssiData[3]);
		data = (data << 8) | (ssiData[4]);*/
		data = (ssiData[4]) | (ssiData[3] << 8)| (ssiData[2] << 16)| (ssiData[1] << 24);
		//memcpy(&data,&ssiData[0],4);
	}
	//data = count;
	dbprintf("data: %08X",data);
	//HAL_Delay(1);
	return data;
}

uint32_t inversegrayCode(uint32_t n)
{
	uint32_t inv = 0;

	// Taking xor until n becomes zero
	for (; n; n = n >> 1)
		inv ^= n;

	return inv;
}

volatile uint8_t gTranducerErr = 0;
volatile uint8_t gTranducerErrCount = 0;

uint32_t ssi_read__()
{
	uint8_t ssiData[5] = {0xFF,0xFF,0xFF,0xFF,0xFF};
	uint32_t data;
	HAL_StatusTypeDef ret = HAL_SPI_Receive(&hspi2,ssiData,5,1000);
	if(ret != HAL_OK){
		return 0xFFFFFFFF;
	}
	if(ssiData[0] == 0xFF && ssiData[1] == 0xFF && ssiData[2] == 0xFF && ssiData[3] == 0xFF  && ssiData[4] == 0xFF ){
		gTranducerErrCount += 1;
		return 0xFFFFFFFF;
	}else{
		if ((gTranducerErrCount > 0) & (gTranducerErrCount <= 1000))
			gTranducerErrCount -= 1;
	}


	data = ssiData[1] & 0x7F;
	data = (data << 8) | (ssiData[2]);
	data = (data << 8) | (ssiData[3]);

	data = (data << 1) | (ssiData[4] & 0x01);


	data = inversegrayCode(data);

	return data;
}

/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2019,  All Rights Reserved. Confidential.      */
/******************************************************************************/
