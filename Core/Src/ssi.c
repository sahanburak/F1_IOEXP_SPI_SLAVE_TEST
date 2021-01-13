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

uint32_t ssi_read()
{
	uint8_t ssiData[4] = {0xFF,0xFF,0xFF,0xFF};
	uint32_t data;

	HAL_SPI_Receive(&hspi2,ssiData,4,1000);

	if(ssiData[0] == 0xFF && ssiData[1] == 0xFF && ssiData[2] == 0xFF && ssiData[3] == 0xFF ){
		data = 0xFFFFFFFF;
	}else{

		data = (data << 8) | (ssiData[1]);
		data = (data << 8) | (ssiData[2]);

		data = (data << 1) | (ssiData[3] & 0x01);

	}
	return data;
}

/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2019,  All Rights Reserved. Confidential.      */
/******************************************************************************/