/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2021 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        SH1107.c
 * @brief       
 *
 * @author      Burak Sahan
 * @date        2021-08-11
 *
 * @ingroup     F1_CompactController_IOExp_Application
 * @{
 *****************************************************************************/

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "stm32f1xx_hal.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <SH1107/sh1107.h>
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
static uint8_t SH1107_Buffer[SH1107_WIDTH * SH1107_HEIGHT / 8];
static SH1107_t SH1107;

/*============================================================================*/
/* Module global data                                                         */
/*============================================================================*/

/*============================================================================*/
/* Implementation of functions                                                */
/*============================================================================*/

/***************************************************************************//**
* @brief Reset SH1107 with reset pin.
*
*******************************************************************************/
void SH1107_Reset(void) {
	SH1107_Off();
	HAL_Delay(200);
	SH1107_On();
}

/***************************************************************************//**
* @brief SH1107 on with reset pin.
*
*******************************************************************************/
void SH1107_On(void) {
	HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_SET);
}

/***************************************************************************//**
* @brief SH1107 off with reset pin.
*
*******************************************************************************/
void SH1107_Off(void) {
	HAL_GPIO_WritePin(LCD_RES_GPIO_Port, LCD_RES_Pin, GPIO_PIN_RESET);
}

/***************************************************************************//**
* @brief  			Send a byte to the command register
*
* @param 	byte	data in byte format for writing
*
*******************************************************************************/
void SH1107_WriteCommand(uint8_t byte) {
	HAL_I2C_Mem_Write(&SH1107_I2C_PORT, SH1107_I2C_ADDR, 0x00, 1, &byte, 1, HAL_MAX_DELAY);
}

/***************************************************************************//**
* @brief  				Send data to display
*
* @param 	buffer		data in pointer format for writing
* @param 	buff_size	data size in size_t format for writing
*
*******************************************************************************/
void SH1107_WriteData(uint8_t* buffer, size_t buff_size) {
	HAL_I2C_Mem_Write(&SH1107_I2C_PORT, SH1107_I2C_ADDR, 0x40, 1, buffer, buff_size, HAL_MAX_DELAY);
}

/***************************************************************************//**
* @brief  				Changed the Page address for pixel drive
*
* @param 	add			page address in unsigned char format between 0~SH1107_PAGE
*
*******************************************************************************/
void SH1107_ChangePageAddress(unsigned char add)
{
	add=0xb0|add;
	HAL_I2C_Mem_Write(&SH1107_I2C_PORT, SH1107_I2C_ADDR, 0x80, 1, &add, 1, HAL_MAX_DELAY);
	return;
}

/***************************************************************************//**
* @brief  				Changed the Column address for pixel drive
*
* @param 	add			Column address in unsigned char format between 0~SH1107_HEIGHT
*
*******************************************************************************/
void SH1107_ChangeColumnAddress(unsigned char add)
{
	uint8_t byte =(0x10|(add>>4));
	SH1107_WriteCommand(byte);

	byte =(0x0f&add);
	SH1107_WriteCommand(byte);

	return;
}

/***************************************************************************//**
* @brief  				Initialize the oled screen
*
*******************************************************************************/
void SH1107_Init(void) {

	/* Reset OLED */
	SH1107_Reset();
	/* Wait for the screen to boot */
	HAL_Delay(100);

	SH1107_On();
	HAL_Delay(100);

	/* Init OLED */
	SH1107_WriteCommand(SH1107_DISPLAY_OFF); 			/* display off */

	SH1107_WriteCommand(SH1107_SET_DISPLAY_CLOCK); 		/* set display clock divide ratio/oscillator frequency */
	SH1107_WriteCommand(0x51); 							/* set divide ratio F0 */

	SH1107_WriteCommand(SH1107_ADDR_MODE); 				/* Set Memory Addressing Mode */
	SH1107_WriteCommand(0x00); 							/* 00b,Horizontal Addressing Mode;
														 * 01b,Vertical Addressing Mode;
														 * 10b,Page Addressing Mode (RESET);
														 * 11b,Invalid */

	SH1107_WriteCommand(SH1107_SET_MULTIPLEX_RATIO); 	/* set multiplex ratio(1 to 64) - CHECK */
	SH1107_WriteCommand(0x3F);

	SH1107_WriteCommand(SH1107_SET_START_LINE); 		/* set start line address - CHECK */
	SH1107_WriteCommand(0x00);

	SH1107_WriteCommand(SH1107_SET_DISPLAY_OFFSET); 	/* set display offset - CHECK */
	SH1107_WriteCommand(0x60); 							/* not offset */

	SH1107_WriteCommand(SH1107_SET_DCDC_SETTINGS); 		/* Set DC-DC Setting: */
	SH1107_WriteCommand(0x80); 							/* Set external VPP */

#ifdef SH1107_INVERSE_COLOR
	SH1107_WriteCommand(SH1107_INVERT_ON); 				/* set inverse color */
#else
	SH1107_WriteCommand(SH1107_INVERT_OFF); 			/* set normal color */
#endif

	SH1107_WriteCommand(SH1107_RAM_ON); 				/* 0xa4,Output follows RAM content;
														 * 0xa5,Output ignores RAM content */

#ifdef SH1107_MIRROR_HORIZ
	SH1107_WriteCommand(SH1107_SET_REMAP_L_TO_R); 		/* Mirror horizontally */
#else
	SH1107_WriteCommand(SH1107_SET_REMAP_R_TO_L); 		/* set segment re-map 0 to 127 - CHECK A1 */
#endif

#ifdef SH1107_MIRROR_VERT
	SH1107_WriteCommand(SH1107_SET_REMAP_T_TO_D); 		/* Mirror vertically */
#else
	SH1107_WriteCommand(SH1107_SET_REMAP_D_TO_T); 		/* Set COM Output Scan Direction */
#endif

	SH1107_WriteCommand(SH1107_SET_CONTRAST); 			/* set contrast control register - CHECK */
	SH1107_WriteCommand(0xFF);

	SH1107_WriteCommand(SH1107_SET_PRECHARGE_PERIOD); 	/* set pre-charge period */
	SH1107_WriteCommand(0x22);

	SH1107_WriteCommand(SH1107_SET_VCOM_DESELECT); 		/* set vcomh */
	SH1107_WriteCommand(0x20); 							/* 0x20,0.77xVcc */

	SH1107_WriteCommand(SH1107_DISPLAY_ON); 			/* turn on SH1107 panel */

	/* Clear screen */
	SH1107_Fill(Black);

	/* Flush buffer to screen */
	SH1107_UpdateScreen();

	/* Set default values for screen object */
	SH1107.CurrentX = 0;
	SH1107.CurrentY = 0;

	SH1107.Initialized = 1;

}

/***************************************************************************//**
* @brief  			Fill the whole screen with the given color
*
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_Fill(SH1107_COLOR_t color) {
	/* Set memory */
	memset(SH1107_Buffer, (color == Black) ? 0x00 : 0xFF, sizeof(SH1107_Buffer));
}

/***************************************************************************//**
* @brief  			Write the screen buffer with changed to the screen
*
*******************************************************************************/
void SH1107_UpdateScreen(void) {

	unsigned char i;
	for(i=0;i<SH1107_PAGE;i++)
	{
		SH1107_ChangePageAddress(i);
		SH1107_ChangeColumnAddress(0x00);
		SH1107_WriteData(&SH1107_Buffer[i*SH1107_WIDTH], SH1107_WIDTH);
	}
	return;
}

/***************************************************************************//**
* @brief  			Draw 1 char to the screen buffer
*
* @param	ch		Character in char format for writing
* @param	Font	Font in FontDef_t type for font size. Font is enumeration with FontDef_t. See sh1107_fonts.h for more details about fonts.
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
char SH1107_WriteChar(char ch, FontDef_t* Font, SH1107_COLOR_t color) {
	uint32_t i, b, j;

	/* Check if character is valid */
	if (ch < 32 || ch > 126)
		return 0;

	/* Check remaining space on current line */
	if (SH1107_WIDTH < (SH1107.CurrentX + Font->FontWidth) ||
			SH1107_HEIGHT < (SH1107.CurrentY + Font->FontHeight))
	{
		// Not enough space on current line
		return 0;
	}

	/* Use the font to write */
	for(i = 0; i < Font->FontHeight; i++) {
		b = Font->data[(ch - 32) * Font->FontHeight + i];
		for(j = 0; j < Font->FontWidth; j++) {
			if((b << j) & 0x8000)  {
				SH1107_DrawPixel(SH1107.CurrentX + j, (SH1107.CurrentY + i), (SH1107_COLOR_t) color);
			} else {
				SH1107_DrawPixel(SH1107.CurrentX + j, (SH1107.CurrentY + i), (SH1107_COLOR_t)!color);
			}
		}
	}

	/* The current space is now taken */
	SH1107.CurrentX += Font->FontWidth;

	/* Return written char for validation */
	return ch;
}

/***************************************************************************//**
* @brief  			Write full string to screenbuffer
*
* @param	str		str in char pointer format for writing
* @param	Font	Font in FontDef_t type for font size. Font is enumeration with FontDef_t. See sh1107_fonts.h for more details about fonts.
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
char SH1107_WriteString(char* str, FontDef_t* Font, SH1107_COLOR_t color) {
	/* Write until null-byte */
	while (*str) {
		if (SH1107_WriteChar(*str, Font, color) != *str) {
			/* Char could not be written */
			return *str;
		}

		/* Next char */
		str++;
	}
	SH1107_UpdateScreen();
	/* Everything ok */
	return *str;
}

/***************************************************************************//**
* @brief  			Position the cursor
*
* @param	x		x in uint8_t format for row coordinate
* @param	y		y in uint8_t format for column coordinate
*
*******************************************************************************/
void SH1107_SetCursor(uint8_t x, uint8_t y) {
	SH1107.CurrentX = x;
	SH1107.CurrentY = y;
}

/***************************************************************************//**
* @brief  			Draw one pixel in the screen buffer
*
* @param	x		x in uint8_t format for row coordinate
* @param	y		y in uint8_t format for column coordinate
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawPixel(uint8_t x, uint8_t y, SH1107_COLOR_t color) {
	if(x >= SH1107_WIDTH || y >= SH1107_HEIGHT) {
		/* Don't write outside the buffer */
		return;
	}

	/* Check if pixel should be inverted */
	if(SH1107.Inverted) {
		color = (SH1107_COLOR_t)!color;
	}

	/* Draw in the right color */
	if(color == White) {
		SH1107_Buffer[x + (y / 8) * SH1107_WIDTH] |= 1 << (y % 8);
	} else {
		SH1107_Buffer[x + (y / 8) * SH1107_WIDTH] &= ~(1 << (y % 8));
	}
}

/***************************************************************************//**
* @brief  			Draw line in the screen buffer
*
* @param	x0		x0 in uint16_t format for start of row coordinate
* @param	x1		x1 in uint16_t format for end of row coordinate
* @param	y0		y0 in uint16_t format for start of column coordinate
* @param	y1		y1 in uint16_t format for end of column coordinate
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, SH1107_COLOR_t c) {
	int16_t dx, dy, sx, sy, err, e2, i, tmp;

	/* Check for overflow */
	if (x0 >= SH1107_WIDTH) {
		x0 = SH1107_WIDTH - 1;
	}
	if (x1 >= SH1107_WIDTH) {
		x1 = SH1107_WIDTH - 1;
	}
	if (y0 >= SH1107_HEIGHT) {
		y0 = SH1107_HEIGHT - 1;
	}
	if (y1 >= SH1107_HEIGHT) {
		y1 = SH1107_HEIGHT - 1;
	}

	dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	sx = (x0 < x1) ? 1 : -1;
	sy = (y0 < y1) ? 1 : -1;
	err = ((dx > dy) ? dx : -dy) / 2;

	if (dx == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Vertical line */
		for (i = y0; i <= y1; i++) {
			SH1107_DrawPixel(x0, i, c);
		}

		/* Return from function */
		return;
	}

	if (dy == 0) {
		if (y1 < y0) {
			tmp = y1;
			y1 = y0;
			y0 = tmp;
		}

		if (x1 < x0) {
			tmp = x1;
			x1 = x0;
			x0 = tmp;
		}

		/* Horizontal line */
		for (i = x0; i <= x1; i++) {
			SH1107_DrawPixel(i, y0, c);
		}

		/* Return from function */
		return;
	}

	while (1) {
		SH1107_DrawPixel(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y0 += sy;
		}
	}
}

/***************************************************************************//**
* @brief  			Draw Rectangle in the screen buffer
*
* @param	x		x in uint16_t format for start of row coordinate
* @param	y		y in uint16_t format for start of column coordinate
* @param	w		w in uint16_t format for rectangle width
* @param	h		h in uint16_t format for rectangle height
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1107_COLOR_t c) {
	/* Check input parameters */
	if (
			x >= SH1107_WIDTH ||
			y >= SH1107_HEIGHT
	) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SH1107_WIDTH) {
		w = SH1107_WIDTH - x;
	}
	if ((y + h) >= SH1107_HEIGHT) {
		h = SH1107_HEIGHT - y;
	}

	/* Draw 4 lines */
	SH1107_DrawLine(x, y, x + w, y, c);         /* Top line */
	SH1107_DrawLine(x, y + h, x + w, y + h, c); /* Bottom line */
	SH1107_DrawLine(x, y, x, y + h, c);         /* Left line */
	SH1107_DrawLine(x + w, y, x + w, y + h, c); /* Right line */
}

/***************************************************************************//**
* @brief  			Draw Filled Rectangle in the screen buffer
*
* @param	x		x in uint16_t format for start of row coordinate
* @param	y		y in uint16_t format for start of column coordinate
* @param	w		w in uint16_t format for rectangle width
* @param	h		h in uint16_t format for rectangle height
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawFilledRectangle(uint16_t x, uint16_t y, uint16_t w, uint16_t h, SH1107_COLOR_t c) {
	uint8_t i;

	/* Check input parameters */
	if (
			x >= SH1107_WIDTH ||
			y >= SH1107_HEIGHT
	) {
		/* Return error */
		return;
	}

	/* Check width and height */
	if ((x + w) >= SH1107_WIDTH) {
		w = SH1107_WIDTH - x;
	}
	if ((y + h) >= SH1107_HEIGHT) {
		h = SH1107_HEIGHT - y;
	}

	/* Draw lines */
	for (i = 0; i <= h; i++) {
		/* Draw lines */
		SH1107_DrawLine(x, y + i, x + w, y + i, c);
	}
}

/***************************************************************************//**
* @brief  			Draw Triangle in the screen buffer
*
* @param	x1		x1 in uint16_t format for first corner row coordinate
* @param	y1		y1 in uint16_t format for first corner column coordinate
* @param	x2		x2 in uint16_t format for second corner row coordinate
* @param	y2		y2 in uint16_t format for second corner column coordinate
* @param	x3		x3 in uint16_t format for third corner row coordinate
* @param	y3		y3 in uint16_t format for third corner column coordinate
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1107_COLOR_t color) {
	/* Draw lines */
	SH1107_DrawLine(x1, y1, x2, y2, color);
	SH1107_DrawLine(x2, y2, x3, y3, color);
	SH1107_DrawLine(x3, y3, x1, y1, color);
}

/***************************************************************************//**
* @brief  			Draw Filled Triangle in the screen buffer
*
* @param	x1		x1 in uint16_t format for first corner row coordinate
* @param	y1		y1 in uint16_t format for first corner column coordinate
* @param	x2		x2 in uint16_t format for second corner row coordinate
* @param	y2		y2 in uint16_t format for second corner column coordinate
* @param	x3		x3 in uint16_t format for third corner row coordinate
* @param	y3		y3 in uint16_t format for third corner column coordinate
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawFilledTriangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t x3, uint16_t y3, SH1107_COLOR_t color) {
	int16_t deltax = 0, deltay = 0, x = 0, y = 0, xinc1 = 0, xinc2 = 0,
			yinc1 = 0, yinc2 = 0, den = 0, num = 0, numadd = 0, numpixels = 0,
			curpixel = 0;

	deltax = ABS(x2 - x1);
	deltay = ABS(y2 - y1);
	x = x1;
	y = y1;

	if (x2 >= x1) {
		xinc1 = 1;
		xinc2 = 1;
	} else {
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1) {
		yinc1 = 1;
		yinc2 = 1;
	} else {
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay){
		xinc1 = 0;
		yinc2 = 0;
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;
	} else {
		xinc2 = 0;
		yinc1 = 0;
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++) {
		SH1107_DrawLine(x, y, x3, y3, color);

		num += numadd;
		if (num >= den) {
			num -= den;
			x += xinc1;
			y += yinc1;
		}
		x += xinc2;
		y += yinc2;
	}
}

/***************************************************************************//**
* @brief  			Draw Circle in the screen buffer
*
* @param	x0		x0 in int16_t format for start position of row coordinate
* @param	y0		y0 in int16_t format for start position of column coordinate
* @param	r		r in int16_t format for circle radius
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawCircle(int16_t x0, int16_t y0, int16_t r, SH1107_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SH1107_DrawPixel(x0, y0 + r, c);
	SH1107_DrawPixel(x0, y0 - r, c);
	SH1107_DrawPixel(x0 + r, y0, c);
	SH1107_DrawPixel(x0 - r, y0, c);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SH1107_DrawPixel(x0 + x, y0 + y, c);
		SH1107_DrawPixel(x0 - x, y0 + y, c);
		SH1107_DrawPixel(x0 + x, y0 - y, c);
		SH1107_DrawPixel(x0 - x, y0 - y, c);

		SH1107_DrawPixel(x0 + y, y0 + x, c);
		SH1107_DrawPixel(x0 - y, y0 + x, c);
		SH1107_DrawPixel(x0 + y, y0 - x, c);
		SH1107_DrawPixel(x0 - y, y0 - x, c);
	}
}

/***************************************************************************//**
* @brief  			Draw Filled Circle in the screen buffer
*
* @param	x0		x0 in int16_t format for start position of row coordinate
* @param	y0		y0 in int16_t format for start position of column coordinate
* @param	r		r in int16_t format for circle radius
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_DrawFilledCircle(int16_t x0, int16_t y0, int16_t r, SH1107_COLOR_t c) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	SH1107_DrawPixel(x0, y0 + r, c);
	SH1107_DrawPixel(x0, y0 - r, c);
	SH1107_DrawPixel(x0 + r, y0, c);
	SH1107_DrawPixel(x0 - r, y0, c);
	SH1107_DrawLine(x0 - r, y0, x0 + r, y0, c);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		SH1107_DrawLine(x0 - x, y0 + y, x0 + x, y0 + y, c);
		SH1107_DrawLine(x0 + x, y0 - y, x0 - x, y0 - y, c);

		SH1107_DrawLine(x0 + y, y0 + x, x0 - y, y0 + x, c);
		SH1107_DrawLine(x0 + y, y0 - x, x0 - y, y0 - x, c);
	}
}

/***************************************************************************//**
* @brief  			Clear all screen
*
*******************************************************************************/
void SH1107_Clear (void)
{
	SH1107_Fill (0);
	SH1107_UpdateScreen();
}

/***************************************************************************//**
* @brief  			SH1107 screen controller on.
*
*******************************************************************************/
void SH1107_ON(void) {
	SH1107_WriteCommand(0x8D);
	SH1107_WriteCommand(0x14);
	SH1107_WriteCommand(0xAF);
}

/***************************************************************************//**
* @brief  			SH1107 screen controller off.
*
*******************************************************************************/
void SH1107_OFF(void) {
	SH1107_WriteCommand(0x8D);
	SH1107_WriteCommand(0x10);
	SH1107_WriteCommand(0xAE);
}

/***************************************************************************//**
* @brief  				Screen scroll to right.
*
* @param	start_row	start_row in uint8_t format. Starting row coordinate for scrolling
* @param	end_row		end_row in uint8_t format. Ending row coordinate for scrolling
*
*******************************************************************************/
void SH1107_ScrollRight(uint8_t start_row, uint8_t end_row)
{
	SH1107_WriteCommand (SH1107_RIGHT_HORIZONTAL_SCROLL);  // send 0x26
	SH1107_WriteCommand (0x00);  // send dummy
	SH1107_WriteCommand(start_row);  // start page address
	SH1107_WriteCommand(0X00);  // time interval 5 frames
	SH1107_WriteCommand(end_row);  // end page address
	SH1107_WriteCommand(0X00);
	SH1107_WriteCommand(0XFF);
	SH1107_WriteCommand (SH1107_ACTIVATE_SCROLL); // start scroll
}

/***************************************************************************//**
* @brief  				Screen scroll to left.
*
* @param	start_row	start_row in uint8_t format. Starting row coordinate for scrolling
* @param	end_row		end_row in uint8_t format. Ending row coordinate for scrolling
*
*******************************************************************************/
void SH1107_ScrollLeft(uint8_t start_row, uint8_t end_row)
{
	SH1107_WriteCommand (SH1107_LEFT_HORIZONTAL_SCROLL);  // send 0x26
	SH1107_WriteCommand (0x00);  // send dummy
	SH1107_WriteCommand(start_row);  // start page address
	SH1107_WriteCommand(0X00);  // time interval 5 frames
	SH1107_WriteCommand(end_row);  // end page address
	SH1107_WriteCommand(0X00);
	SH1107_WriteCommand(0XFF);
	SH1107_WriteCommand (SH1107_ACTIVATE_SCROLL); // start scroll
}

/***************************************************************************//**
* @brief  				Screen scroll to diagonal right.
*
* @param	start_row	start_row in uint8_t format. Starting row coordinate for scrolling
* @param	end_row		end_row in uint8_t format. Ending row coordinate for scrolling
*
*******************************************************************************/
void SH1107_Scrolldiagright(uint8_t start_row, uint8_t end_row)
{
	SH1107_WriteCommand(SH1107_SET_VERTICAL_SCROLL_AREA);  // sect the area
	SH1107_WriteCommand (0x00);   // write dummy
	SH1107_WriteCommand(SH1107_HEIGHT);

	SH1107_WriteCommand(SH1107_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL);
	SH1107_WriteCommand (0x00);
	SH1107_WriteCommand(start_row);
	SH1107_WriteCommand(0X00);
	SH1107_WriteCommand(end_row);
	SH1107_WriteCommand (0x01);
	SH1107_WriteCommand (SH1107_ACTIVATE_SCROLL);
}


/***************************************************************************//**
* @brief  				Screen scroll to diagonal left.
*
* @param	start_row	start_row in uint8_t format. Starting row coordinate for scrolling
* @param	end_row		end_row in uint8_t format. Ending row coordinate for scrolling
*
*******************************************************************************/
void SH1107_Scrolldiagleft(uint8_t start_row, uint8_t end_row)
{
	SH1107_WriteCommand(SH1107_SET_VERTICAL_SCROLL_AREA);  // sect the area
	SH1107_WriteCommand (0x00);   // write dummy
	SH1107_WriteCommand(SH1107_HEIGHT);

	SH1107_WriteCommand(SH1107_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL);
	SH1107_WriteCommand (0x00);
	SH1107_WriteCommand(start_row);
	SH1107_WriteCommand(0X00);
	SH1107_WriteCommand(end_row);
	SH1107_WriteCommand (0x01);
	SH1107_WriteCommand (SH1107_ACTIVATE_SCROLL);
}

/***************************************************************************//**
* @brief  				Screen scroll stop
*
*******************************************************************************/
void SH1107_Stopscroll(void)
{
	SH1107_WriteCommand(SH1107_DEACTIVATE_SCROLL);
}

/***************************************************************************//**
* @brief  				Invert screen color
*
* @param	state		state in uint8_t format. If state is 0 non-inverting display, otherwise inverting display.
*
*******************************************************************************/
void SH1107_InvertDisplay(uint8_t state)
{
	if (state) SH1107_WriteCommand (SH1107_INVERT_ON);

	else SH1107_WriteCommand (SH1107_INVERT_OFF);

}

/***************************************************************************//**
* @brief  				Draw bitmap to screen
*
* @param	x			x in int16_t format for start position of row coordinate
* @param	y			y in int16_t format for start position of column coordinate
* @param	bitmap		bitmap in unsigned char pointer format for bitmap data buffer.
* @param	w			w in int16_t format for bitmap width.
* @param	h			h in int16_t format for bitmap height.
* @param	color	The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*
*******************************************************************************/
void SH1107_DrawBitmap(int16_t x, int16_t y, const unsigned char* bitmap, int16_t w, int16_t h, uint16_t color)
{

	int16_t byteWidth = (w + 7) / 8; // Bitmap scanline pad = whole byte
	uint8_t byte = 0;

	for(int16_t j=0; j<h; j++, y++)
	{
		for(int16_t i=0; i<w; i++)
		{
			if(i & 7)
			{
				byte <<= 1;
			}
			else
			{
				byte = (*(const unsigned char *)(&bitmap[j * byteWidth + i / 8]));
			}
			if(byte & 0x80) SH1107_DrawPixel(x+i, y, color);
		}
	}
}

/***************************************************************************//**
* @brief  				Invert mode toggle
*
*******************************************************************************/
void SH1107_ToggleInvert(void) {
	uint16_t i;

	/* Toggle invert */
	SH1107.Inverted = !SH1107.Inverted;

	/* Do memory toggle */
	for (i = 0; i < sizeof(SH1107_Buffer); i++) {
		SH1107_Buffer[i] = ~SH1107_Buffer[i];
	}
}

/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2021,  All Rights Reserved. Confidential.      */
/******************************************************************************/
