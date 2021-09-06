/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2019 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        ad529x.h
 * @brief       
 *
 * @author      Burak Sahan
 * @date        Feb 18, 2021
 *
 * @ingroup     F1_IOEXP_SPI_SLAVE_TEST
 * @{
 *****************************************************************************/
#ifndef INC_AD529X_H_
#define INC_AD529X_H_
/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "main.h"
#include "spi.h"
#include <stdint.h>
#include <stm32f1xx_hal_def.h>
/*============================================================================*/
/* Forward declarations                                                       */
/*============================================================================*/

/*============================================================================*/
/* Constants and macros                                                       */
/*============================================================================*/

/*============================================================================*/
/* Type definitions                                                           */
/*============================================================================*/
#define MAX_BITS_NUMBER  			10

/* Commands */
/* AD529x Commands */
#define AD529x_NOP					0x00
#define AD529x_WRITE_RDAC			0x01
#define AD529x_READ_RDAC			0x02
#define AD529x_RESET_RDAC			0x04
#define AD529x_WRITE_CONTENT		0x06
#define AD529x_READ_CONTROL			0x07
#define AD529x_SHUTDOWN				0x09

/* AD529x Control Register Bits */
#define AD529x_C3					(1 << 3)
#define AD529x_C2					(1 << 2)
#define AD529x_C1					(1 << 1)
#define AD529x_C0					(1 << 0)

/* AD529x GPIO */
/* RESET - GPIO1 */
#define AD529x_RESET_OUT        	GPIO1_PIN_OUT
#define AD529x_RESET_LOW        	GPIO1_LOW
#define AD529x_RESET_HIGH       	GPIO1_HIGH

#define AD529x_SLAVE_ID        		1

#define MAX_RES_VALUE 200000
/*============================================================================*/
/* Global data                                                                */
/*============================================================================*/

/*============================================================================*/
/* Declarations                                                               */
/*============================================================================*/

#define AD529x_CS_HIGH()		HAL_GPIO_WritePin(LVDT_POT_CS_GPIO_Port, LVDT_POT_CS_Pin, GPIO_PIN_SET);
#define AD529x_CS_LOW()			HAL_GPIO_WritePin(LVDT_POT_CS_GPIO_Port, LVDT_POT_CS_Pin, GPIO_PIN_RESET);

void AD529x_SetRegister(unsigned char command, unsigned short data);
void AD529x_SetResistor(uint32_t res);
char AD529x_Init();
#endif /* INC_AD529X_H_ */
/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2019,  All Rights Reserved. Confidential.      */
/******************************************************************************/
