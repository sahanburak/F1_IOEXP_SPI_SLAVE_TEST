/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2021 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        sh1107_fonts.h
 * @brief       
 *
 * @author      Burak Sahan
 * @date        2021-08-11
 *
 * @ingroup     F1_CompactController_IOExp_Application
 * @{
 *****************************************************************************/
#ifndef SRC_SH1107_SH1107_FONTS_H_
#define SRC_SH1107_SH1107_FONTS_H_
/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include <stdint.h>
/*============================================================================*/
/* Forward declarations                                                       */
/*============================================================================*/

/*============================================================================*/
/* Constants and macros                                                       */
/*============================================================================*/

/*============================================================================*/
/* Type definitions                                                           */
/*============================================================================*/

/**
 * @brief  Font structure used on my LCD libraries
 */
typedef struct {
	uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef_t;

/**
 * @brief  String length and height
 */
typedef struct {
	uint16_t Length;      /*!< String length in units of pixels */
	uint16_t Height;      /*!< String height in units of pixels */
} FONTS_SIZE_t;
/*============================================================================*/
/* Global data                                                                */
/*============================================================================*/
extern FontDef_t Font_6x8;
extern FontDef_t Font_7x10;
extern FontDef_t Font_11x18;
extern FontDef_t Font_16x26;
/*============================================================================*/
/* Declarations                                                               */
/*============================================================================*/



#endif /* SRC_SH1107_SH1107_FONTS_H_ */
/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2021,  All Rights Reserved. Confidential.      */
/******************************************************************************/
