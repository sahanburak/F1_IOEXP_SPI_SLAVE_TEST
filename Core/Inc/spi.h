/**
  ******************************************************************************
  * File Name          : SPI.h
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __spi_H
#define __spi_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
 /** Clock phase. */
 #define SPI_CPHA    (1 << 0)

 /** Clock polarity. */
 #define SPI_CPOL    (1 << 1)

 /** SPI mode definition. */
 #define SPI_MODE_0  0
 #define SPI_MODE_1  (SPI_CPHA)
 #define SPI_MODE_2  (SPI_CPOL)
 #define SPI_MODE_3  (SPI_CPOL | SPI_CPHA)


 #define IOEXP_SPI3_SLV		1
 #define DIGIPOT_SPI3_SLV	2
 #define IOEXP_MODE			SPI_MODE_0
 #define DIGIPOT_MODE		SPI_MODE_1
/* USER CODE END Includes */

extern SPI_HandleTypeDef hspi1;
extern SPI_HandleTypeDef hspi2;
extern SPI_HandleTypeDef hspi3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_SPI1_Init(void);
void MX_SPI2_Init(void);
void MX_SPI3_Init(void);

/* USER CODE BEGIN Prototypes */
void HAL_SPI3_SetMode(uint8_t mode);
void HAL_SPI3_SelectSlave(uint8_t slave);
void HAL_SPI3_DeselectSlave(uint8_t slave);
uint8_t HAL_SPI3_ReadWrite(uint8_t slave, uint8_t *txData, uint8_t *rxData,uint16_t Size);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ spi_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
