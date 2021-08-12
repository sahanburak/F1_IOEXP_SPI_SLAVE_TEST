/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2021 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        sh1107.h
 * @brief       SH1107 LCD controller functions declarations
 *
 * @author      Burak Şahan
 * @date        2021-08-11
 *
 * @ingroup     F1_CompactController_IOExp_Application
 * @{
 *****************************************************************************/
#ifndef INC_SH1107_SH1107_H_
#define INC_SH1107_SH1107_H_

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include <stddef.h>
#include <SH1107/sh1107_fonts.h>
#include "stm32f1xx_hal.h"
#include "main.h"
/*============================================================================*/
/* Forward declarations                                                       */
/*============================================================================*/

#ifndef SH1107_I2C_PORT
#define SH1107_I2C_PORT		hi2c1
#endif

#ifndef SH1107_I2C_ADDR
#define SH1107_I2C_ADDR        (0x3C << 1)
#endif

#ifndef SH1107_Reset_Port
#define SH1107_Reset_Port      LCD_RES_GPIO_Port
#endif
#ifndef SH1107_Reset_Pin
#define SH1107_Reset_Pin       LCD_RES_Pin
#endif

extern I2C_HandleTypeDef SH1107_I2C_PORT;


/* SH1107 OLED height in pixels */
#ifndef SH1107_HEIGHT
#define SH1107_HEIGHT          128
#endif

/* SH1107 width in pixels */
#ifndef SH1107_WIDTH
#define SH1107_WIDTH           64
#endif

#ifndef SH1107_PAGE
#define SH1107_PAGE	16
#endif

#define SH1107_ADDR_MODE 				0x20
#define SH1107_ADDR_COLUMN 				0x21
#define SH1107_ADDR_PAGE 				0x22
#define SH1107_SET_START_LINE 			0xDC//0x40
#define SH1107_SET_CONTRAST 			0x81
#define SH1107_CHARGE_DCDC_PUMP 		0x8D
#define SH1107_SET_REMAP_R_TO_L 		0xA0
#define SH1107_SET_REMAP_L_TO_R 		0xA1
#define SH1107_RAM_ON 					0xA4
#define SH1107_RAM_OFF 					0xA5
#define SH1107_INVERT_OFF 				0xA6
#define SH1107_INVERT_ON 				0xA7
#define SH1107_SET_MULTIPLEX_RATIO 		0xA8
#define SH1107_DISPLAY_OFF 				0xAE
#define SH1107_DISPLAY_ON 				0xAF
#define SH1107_SET_REMAP_D_TO_T 		0xC0
#define SH1107_SET_REMAP_T_TO_D 		0xC8
#define SH1107_SET_DISPLAY_OFFSET 		0xD3
#define SH1107_SET_DISPLAY_CLOCK 		0xD5
#define SH1107_SET_PRECHARGE_PERIOD 	0xD9
#define SH1107_SET_COM_PINS 			0xDA
#define SH1107_SET_VCOM_DESELECT 		0xDB

#define SH1107_SET_DCDC_SETTINGS 		0xAD


#define SH1107_RIGHT_HORIZONTAL_SCROLL              0x26
#define SH1107_LEFT_HORIZONTAL_SCROLL               0x27
#define SH1107_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SH1107_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A
#define SH1107_DEACTIVATE_SCROLL                    0x2E // Stop scroll
#define SH1107_ACTIVATE_SCROLL                      0x2F // Start scroll
#define SH1107_SET_VERTICAL_SCROLL_AREA             0xA3 // Set scroll range

#define min(a, b) (((a) < (b)) ? (a) : (b))
#define ABS(x)   ((x) > 0 ? (x) : -(x))
/*============================================================================*/
/* Constants and macros                                                       */
/*============================================================================*/

/*============================================================================*/
/* Type definitions                                                           */
/*============================================================================*/

/* Enumeration for screen colors */
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SH1107_COLOR_t;

/* Struct to store transformations */
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SH1107_t;

/*============================================================================*/
/* Global data                                                                */
/*============================================================================*/

/*============================================================================*/
/* Declarations                                                               */
/*============================================================================*/

void SH1107_Reset(void);
void SH1107_On(void);
void SH1107_Off(void);
void SH1107_WriteCommand(uint8_t byte);
void SH1107_WriteData(uint8_t* buffer, size_t buff_size);
void SH1107_ChangePageAddress(unsigned char add);
void SH1107_ChangeColumnAddress(unsigned char add);
void SH1107_Init(void);
void SH1107_Fill(SH1107_COLOR_t color);
void SH1107_UpdateScreen(void);
char SH1107_WriteChar(char ch, FontDef_t* Font, SH1107_COLOR_t color);
char SH1107_WriteString(char* str, FontDef_t* Font, SH1107_COLOR_t color);
void SH1107_SetCursor(uint8_t x, uint8_t y);
void SH1107_DrawPixel(uint8_t x, uint8_t y, SH1107_COLOR_t color);
void SH1107_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SH1107_COLOR_t c);
void SH1107_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1107_COLOR_t c);
void SH1107_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1107_COLOR_t c);
void SH1107_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1107_COLOR_t color);
void SH1107_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1107_COLOR_t color);
void SH1107_DrawCircle(int16_t x0, int16_t y0, int16_t r, SH1107_COLOR_t c);
void SH1107_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SH1107_COLOR_t c);
void SH1107_Clear (void);
void SH1107_ON(void);
void SH1107_OFF(void);
void SH1107_ScrollRight(uint8_t start_row, uint8_t end_row);
void SH1107_ScrollLeft(uint8_t start_row, uint8_t end_row);
void SH1107_Scrolldiagright(uint8_t start_row, uint8_t end_row);
void SH1107_Scrolldiagleft(uint8_t start_row, uint8_t end_row);
void SH1107_Stopscroll(void);
void SH1107_InvertDisplay(uint8_t state);
void SH1107_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color);
void SH1107_ToggleInvert(void);
#endif /* INC_SH1107_SH1107_H_ */
/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2021,  All Rights Reserved. Confidential.      */
/******************************************************************************/
