/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2021 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        sh1107_test.c
 * @brief       SH1107 LCD controller test functions
 *
 * @author      Burak Şahan
 * @date        2021-08-11
 *
 * @ingroup     F1_CompactController_IOExp_Application
 * @{
 *****************************************************************************/

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "SH1107/sh1107_test.h"
#include "SH1107/sh1107.h"
#include <string.h>
#include <stdio.h>
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

/***************************************************************************//**
* @brief Draws lines on the display at different angles.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/

void SH1107_TestLines(uint8_t color)
{
	uint8_t x1, y1, x2, y2;

	SH1107_Clear ();

	x1 = y1 = 0;
	y2    = SH1107_HEIGHT - 1;
	for (x2 = 0; x2 < SH1107_WIDTH; x2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		HAL_Delay(1);
		SH1107_UpdateScreen();
	}
	x2    = SH1107_WIDTH - 1;
	for (y2 = 0; y2 < SH1107_HEIGHT; y2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		HAL_Delay(1);
		SH1107_UpdateScreen();
	}

	HAL_Delay(10);


	SH1107_Clear ();

	x1    = SH1107_WIDTH - 1;
	y1    = 0;
	y2    = SH1107_HEIGHT - 1;
	for (x2 = 0; x2 < SH1107_WIDTH; x2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		SH1107_UpdateScreen();
		HAL_Delay(1);
	}
	x2    = 0;
	for (y2 = 0; y2 < SH1107_HEIGHT; y2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		SH1107_UpdateScreen();
		HAL_Delay(1);
	}

	HAL_Delay(10);


	SH1107_Clear ();

	x1    = 0;
	y1    = SH1107_HEIGHT - 1;
	y2    = 0;
	for (x2 = 0; x2 < SH1107_WIDTH; x2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		HAL_Delay(1);
		SH1107_UpdateScreen();
	}
	x2    = SH1107_WIDTH - 1;
	for (y2 = 0; y2 < SH1107_HEIGHT; y2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		HAL_Delay(1);
		SH1107_UpdateScreen();
	}

	HAL_Delay(10);


	SH1107_Clear ();

	x1    = SH1107_WIDTH - 1;
	y1    = SH1107_HEIGHT - 1;
	y2    = 0;
	for (x2 = 0; x2 < SH1107_WIDTH; x2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		HAL_Delay(1);
		SH1107_UpdateScreen();
	}
	x2    = 0;
	for (y2 = 0; y2 < SH1107_HEIGHT; y2 += 6)
	{
		SH1107_DrawLine(x1, y1, x2, y2, color);
		HAL_Delay(1);
		SH1107_UpdateScreen();
	}
}


/***************************************************************************//**
* @brief Draws rectangles on the display.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_TestRectangles (uint8_t color)
{
	uint8_t n, i, i2;

	SH1107_Clear ();

	n = min(SH1107_WIDTH, SH1107_HEIGHT);

	for (i = 2; i < n; i += 6)
	{
		i2 = i / 2;
		SH1107_DrawRectangle((SH1107_WIDTH/2) - i2, (SH1107_HEIGHT/2) - i2, i, i, color);
		SH1107_UpdateScreen();
		HAL_Delay (10);
	}
}

/***************************************************************************//**
* @brief Draws filled rectangles on the display.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_TestFilledRectangles (uint8_t color)
{
	uint8_t n, i, i2,
	cx = SH1107_WIDTH / 2 - 1,
	cy = SH1107_HEIGHT / 2 - 1;

	SH1107_Clear ();

	n = min(SH1107_WIDTH, SH1107_HEIGHT);
	for (i = n; i > 0; i -= 6)
	{
		i2    = i / 2;
		SH1107_DrawFilledRectangle(cx - i2, cy - i2, i, i, color);
		SH1107_UpdateScreen();
		HAL_Delay (10);
	}
}

/***************************************************************************//**
* @brief Draws filled circles on the display.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_TestFilledCircles(uint8_t radius, uint16_t color)
{
	uint8_t x, y, w = SH1107_WIDTH, h = SH1107_HEIGHT, r2 = radius * 2;

	SH1107_Clear ();

	for (x = radius; x < w; x += r2)
	{
		for (y = radius; y < h; y += r2)
		{
			SH1107_DrawFilledCircle(x, y, radius, color);
			SH1107_UpdateScreen();
			HAL_Delay (10);
		}
	}
}

/***************************************************************************//**
* @brief Draws circles on the display.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_TestCircles(uint8_t radius, uint16_t color)
{
	SH1107_Clear ();

	uint8_t x, y, r2 = radius * 2,
			w = SH1107_WIDTH + radius,
			h = SH1107_HEIGHT + radius;

	for (x = 0; x < w; x += r2)
	{
		for (y = 0; y < h; y += r2)
		{
			SH1107_DrawCircle(x, y, radius, color);
			SH1107_UpdateScreen();
			HAL_Delay (10);
		}
	}
}

/***************************************************************************//**
* @brief Draws triangles on the display.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_TestTriangles(uint8_t color)
{
	uint8_t n, i, cx = SH1107_WIDTH  / 2 - 1,
			cy = SH1107_HEIGHT / 2 - 1;

	SH1107_Clear ();

	n = min(cx, cy);
	for (i = 0; i < n; i += 5)
	{
		SH1107_DrawTriangle(cx , cy - i, cx - i, cy + i, cx + i, cy + i, color);
		SH1107_UpdateScreen();
		HAL_Delay (10);
	}

}

/***************************************************************************//**
* @brief Draws border on the display.
*
* @param color   - The color of the lines. Color is enumeration with SH1107_COLOR_t. See sh1107.h for more details about color.
*
*******************************************************************************/
void SH1107_TestBorder() {
	SH1107_Fill(Black);

	uint32_t start = HAL_GetTick();
	uint32_t end = start;
	uint8_t x = 0;
	uint8_t y = 0;
	do {
		SH1107_DrawPixel(x, y, Black);

		if((y == 0) && (x < 127))
			x++;
		else if((x == 127) && (y < 63))
			y++;
		else if((y == 63) && (x > 0))
			x--;
		else
			y--;

		SH1107_DrawPixel(x, y, White);
		SH1107_UpdateScreen();

		HAL_Delay(5);
		end = HAL_GetTick();
	} while((end - start) < 8000);

	HAL_Delay(1000);
}

/***************************************************************************//**
* @brief Writes string to display with different fonts.
*
*******************************************************************************/
void SH1107_TestFonts() {
	SH1107_Fill(Black);
	SH1107_SetCursor(2, 0);
	SH1107_WriteString("Font 16x26", &Font_16x26, White);
	SH1107_SetCursor(2, 26);
	SH1107_WriteString("Font 11x18", &Font_11x18, White);
	SH1107_SetCursor(2, 26+18);
	SH1107_WriteString("Font 7x10", &Font_7x10, White);
	SH1107_SetCursor(2, 26+18+10);
	SH1107_WriteString("Font 6x8", &Font_6x8, White);
	SH1107_UpdateScreen();
}

/***************************************************************************//**
* @brief Changed display frames for fps test.
*
*******************************************************************************/
void SH1107_TestFPS() {
	SH1107_Fill(White);

	uint32_t start = HAL_GetTick();
	uint32_t end = start;
	int fps = 0;
	char message[] = "ABCDEFGHIJK";

	SH1107_SetCursor(2,0);
	SH1107_WriteString("Testing...", &Font_11x18, Black);

	do {
		SH1107_SetCursor(2, 18);
		SH1107_WriteString(message, &Font_11x18, Black);
		SH1107_UpdateScreen();

		char ch = message[0];
		memmove(message, message+1, sizeof(message)-2);
		message[sizeof(message)-2] = ch;

		fps++;
		end = HAL_GetTick();
	} while((end - start) < 5000);

	HAL_Delay(1000);

	char buff[64];
	fps = (float)fps / ((end - start) / 1000.0);
	snprintf(buff, sizeof(buff), "~%d FPS", fps);

	SH1107_Fill(White);
	SH1107_SetCursor(2, 18);
	SH1107_WriteString(buff, &Font_11x18, Black);
	SH1107_UpdateScreen();
}


/***************************************************************************//**
* @brief Tests different drawing object.
*
*******************************************************************************/
void SH1107_TestAll(int duration){
	SH1107_TestLines(White);
	HAL_Delay(duration);
	SH1107_Clear();
	SH1107_TestLines(Black);
	HAL_Delay(duration);
	SH1107_Clear();

	SH1107_TestRectangles (White);
	HAL_Delay(duration);
	SH1107_Clear();
	SH1107_TestRectangles (Black);
	HAL_Delay(duration);
	SH1107_Clear();

	SH1107_TestFilledRectangles (White);
	HAL_Delay(duration);
	SH1107_Clear();
	SH1107_TestFilledRectangles (White);
	HAL_Delay(duration);
	SH1107_Clear();

	SH1107_TestFilledCircles(16, White);
	HAL_Delay(duration);
	SH1107_Clear();
	SH1107_TestFilledCircles(16, Black);
	HAL_Delay(duration);
	SH1107_Clear();


	SH1107_TestCircles(16,White);
	HAL_Delay(duration);
	SH1107_Clear();
	SH1107_TestCircles(16, Black);
	HAL_Delay(duration);
	SH1107_Clear();


	SH1107_TestTriangles(White);
	HAL_Delay(duration);
	SH1107_Clear();

	SH1107_TestFonts();
	HAL_Delay(duration);
	SH1107_Clear();

	SH1107_TestTriangles(Black);
	HAL_Delay(duration);
	SH1107_Clear();
}

/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2021,  All Rights Reserved. Confidential.      */
/******************************************************************************/
