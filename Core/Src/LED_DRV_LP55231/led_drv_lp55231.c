/******************************************************************************
 *
 *   Copyright (C) Rota Teknik 2021 All Rights Reserved. Confidential
 *
 **************************************************************************//**
 * @file        led_drv_lp55231.c
 * @brief
 *
 * @author      Burak Sahan
 * @date        2021-08-12
 *
 * @ingroup     F1_CompactController_IOExp_Application
 * @{
 *****************************************************************************/

/*============================================================================*/
/* Includes                                                                   */
/*============================================================================*/
#include "LED_DRV_LP55231/led_drv_lp55231.h"
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
 * @brief  				Write Value to Specific Register
 *
 * @param 	reg_addr	Register Address
 * @param 	data		Register Value
 *
 *******************************************************************************/
void LP55231_WriteReg(uint8_t reg_addr, uint8_t data) {
	HAL_I2C_Mem_Write(&LP55231_I2C_PORT, LP55231_I2C_ADDR, reg_addr, 1, &data, 1, HAL_MAX_DELAY);
}

/***************************************************************************//**
 * @brief  				Write Buffer to Specific Register
 *
 * @param 	reg_addr	Register Address
 * @param 	buffer		Data Buffer
 * @param 	buff_size	Data Buffer Size
 *
 *******************************************************************************/
void LP55231_WriteData(uint8_t reg_addr, uint8_t* buffer, size_t buff_size) {
	HAL_I2C_Mem_Write(&LP55231_I2C_PORT, LP55231_I2C_ADDR, reg_addr, 1, buffer, buff_size, HAL_MAX_DELAY);
}

/***************************************************************************//**
 * @brief  				Read Value from Specific Register
 *
 * @param 	reg_addr	Register Address
 *
 *******************************************************************************/
uint8_t LP55231_ReadReg(uint8_t reg_addr) {
	uint8_t reg_data = 0;
	HAL_I2C_Mem_Read(&LP55231_I2C_PORT, LP55231_I2C_ADDR, reg_addr, 1, &reg_data, 1, HAL_MAX_DELAY);
}

/***************************************************************************//**
 * @brief  				Initialize the LP55231
 *
 *******************************************************************************/
void LP55231_Init(void){

	LP55231_Reset();
	HAL_Delay(500);
	LP55231_Enable();
	HAL_Delay(500);
	return;
}

/***************************************************************************//**
 * @brief  				Enable all led channels
 *
 *******************************************************************************/
void LP55231_Enable()
{
	/* Set enable bit */
	LP55231_WriteReg(REG_CNTRL1, 0x40 );

	/* enable internal clock & charge pump & write auto increment */
	LP55231_WriteReg(REG_MISC, 0x53);
}

/***************************************************************************//**
 * @brief  				Disable all led channels
 *
 *******************************************************************************/
void LP55231_Disable()
{
	uint8_t val;

	val = LP55231_ReadReg(REG_CNTRL1);
	val &= ~0x40;
	LP55231_WriteReg(REG_CNTRL1, val);
}

/***************************************************************************//**
 * @brief  				Reset the LPC55231
 *
 *******************************************************************************/
void LP55231_Reset()
{
	// force reset
	LP55231_WriteReg(REG_RESET, 0xff);
}

/***************************************************************************//**
 * @brief  				Set PWM Value to specific channel
 *
 * @param 	channel		Led output channel number. See led_drv_lp55231.h for more details about channels.
 * @param 	value		PWM value 0~255
 *
 * @return 	Error Code	If 0 is success, otherwise return error code. See led_drv_lp55231.h for more details about error codes.
 *******************************************************************************/
eLP55231ErrCode LP55231_SetChannelPWM(uint8_t channel, uint8_t value)
{
	if(channel >= LP55231_NumChannels)
	{
		return LP_ERR_INVALID_CHANNEL;
	}

	LP55231_WriteReg(REG_D1_PWM + channel, value);
	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Calculate led channel number depend on connection
 *
 * @param 	led			rgb led number
 * @param 	color		led color is based on eLEDColor. See led_drv_lp55231.h for more details about led color.s
 *
 * @return 	channel		return channel number depend on connection. See led_drv_lp55231.h for more details about led channel number.
 *
 *******************************************************************************/
eLP55231Channels LP55231_GetLedChannel(uint8_t led, eLEDColor color)
{
	if(led > (LP55231_NumChannels/3))
	{
		return -1;
	}
	eLP55231Channels ch = -1;
	switch(led){
	case 1:
		switch(color){
		case LED_COLOR_RED:
			ch = RGB_LED1_RED_CH;
			break;
		case LED_COLOR_GREEN:
			ch = RGB_LED1_GREEN_CH;
			break;
		case LED_COLOR_BLUE:
			ch = RGB_LED1_BLUE_CH;
			break;
		default:
			ch = -1;
			break;
		}
		break;
		case 2:
			switch(color){
			case LED_COLOR_RED:
				ch = RGB_LED2_RED_CH;
				break;
			case LED_COLOR_GREEN:
				ch = RGB_LED2_GREEN_CH;
				break;
			case LED_COLOR_BLUE:
				ch = RGB_LED2_BLUE_CH;
				break;
			default:
				ch = -1;
				break;
			}
			break;
			case 3:
				switch(color){
				case LED_COLOR_RED:
					ch = RGB_LED3_RED_CH;
					break;
				case LED_COLOR_GREEN:
					ch = RGB_LED3_GREEN_CH;
					break;
				case LED_COLOR_BLUE:
					ch = RGB_LED3_BLUE_CH;
					break;
				default:
					ch = -1;
					break;
				}
				break;
				default:
					ch = -1;
					break;
	}
	return ch;
}

/***************************************************************************//**
 * @brief  				Set led color
 *
 * @param 	led			rgb led number
 * @param 	argb		hexadecimal format color code with alpha(brightness).
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetColor(uint8_t led, uint32_t argb)
{
	if(led > (LP55231_NumChannels/3))
	{
		return LP_ERR_INVALID_CHANNEL;
	}

	uint8_t brightness = (argb & 0xFF000000) >> 24;
	uint8_t color[] = {(argb & 0x00FF0000) >> 16 , (argb & 0x0000FF00) >> 8 , (argb & 0x000000FF)};
	uint8_t ind = 0;
	while(ind<3){
		eLP55231Channels channel = LP55231_GetLedChannel(led, (LED_COLOR_RED+ind));
		if(color[ind]){
			LP55231_SetChannelPWM(channel,color[ind]*brightness/255);
		}else{
			LP55231_SetChannelPWM(channel,0);
		}
		ind++;
	}

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Master fader allows the user to control dimming of multiple LEDS with a single serial bus write.
 *
 * @param 	fader		fader number. It's limited with LP55231_NumFaders.
 * @param 	value		Target value for dimming.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetMasterFader(uint8_t fader, uint8_t value)
{
	if(fader >= LP55231_NumFaders)
	{
		return LP_ERR_INVALID_FADER;
	}

	LP55231_WriteReg(REG_MASTER_FADE_1 + fader, value);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Selects between linear and logarithmic PWM brightness adjustment
 *
 * @param 	channel		led channel number
 * @param 	enable		If 1 Enabled logarithmic, otherwise linear PWM brightness adjustment
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetLogBrightness(uint8_t channel, uint8_t enable)
{
	uint8_t regVal, bitVal;

	if(channel >= LP55231_NumChannels)
	{
		return LP_ERR_INVALID_CHANNEL;
	}

	regVal = LP55231_ReadReg(REG_D1_CTRL + channel);
	bitVal = enable?0x20:0x00;
	regVal &= ~0x20;
	regVal |= bitVal;
	LP55231_WriteReg(REG_D1_CTRL + channel, regVal);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Set Led drive current.
 *
 * @param 	channel		led channel number
 * @param 	value		Output current set value. Current is between 0 to 25.5mA. This value should be set with step size is 100 μA.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetDriveCurrent(uint8_t channel, uint8_t value)
{
	if(channel >= LP55231_NumChannels)
	{
		return LP_ERR_INVALID_CHANNEL;
	}

	LP55231_WriteReg(REG_D1_I_CTL + channel, value);
	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Master fader assignment to specific led channel
 *
 * @param 	channel		led channel number
 * @param 	fader		Master fader number. This value should be set as 0 to 3.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_AssignChannelToMasterFader(uint8_t channel, uint8_t fader)
{
	uint8_t regVal, bitVal;

	if(channel >= LP55231_NumChannels)
	{
		return LP_ERR_INVALID_CHANNEL;
	}
	else if(fader >= LP55231_NumFaders)
	{
		return LP_ERR_INVALID_FADER;
	}

	regVal = LP55231_ReadReg(REG_D1_CTRL + channel);
	bitVal = (fader + 1) & 0x03;
	bitVal <<= 6;
	regVal &= ~0xc0;
	regVal |= bitVal;
	LP55231_WriteReg(REG_D1_CTRL + channel, regVal);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Ratiometric dimming is similar to master fader, but when LEDs are driven by Execution engines
 *
 * @param 	channel		led channel number
 * @param 	value		Target value for dimming.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetRatiometricDimming(uint8_t channel, uint8_t value)
{
	uint8_t regVal;

	if(channel >= LP55231_NumChannels)
	{
		return LP_ERR_INVALID_CHANNEL;
	}

	if(channel == LP55231_NumChannels - 1)
	{
		regVal = LP55231_ReadReg(REG_RATIO_MSB);
		if(value)
		{
			regVal |= 0x01;
		}
		else
		{
			regVal &= ~0x01;
		}
		LP55231_WriteReg(REG_RATIO_MSB, regVal);
	}
	else
	{
		regVal = LP55231_ReadReg(REG_RATIO_LSB);
		if(value)
		{
			regVal |= (0x01 << channel);
		}
		else
		{
			regVal &= ~(0x01 << channel);
		}
		LP55231_WriteReg(REG_RATIO_LSB, regVal);
	}

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Load program
 *
 * @param 	prog		Program buffer pointer.
 * @param 	len			Program buffer lenght.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_LoadProgram(const uint16_t* prog, uint8_t len)
{
	uint8_t page;

	if(len >= LP55231_NumInstructions)
	{
		return LP_ERR_PROGRAM_LENGTH;
	}

	/* set up program write
	 * start in execution disabled mode (0b00)
	 * required to get into load mode.
	 * "Load program mode can be entered from the disabled mode only.  be
	 * entered from the disabled mode only."
	 */
	LP55231_WriteReg(REG_CNTRL2, 0x00);
	LP55231_WriteReg(REG_CNTRL2, 0x15);

	/* try to write program from example
	 * datasheet says MSB of each instruction is in earlier address
	 * TBD: could optimize with a sequence of byte writes, using auto increment
	 */

	/* use auto-increment of chip - enabled in MISC.
	 * If it gets turned off, this breaks.  TBD: set it explicitly?
	 */

	/* Write complete pages, setting page reg for each. */
	for(page = 0; page < (len/16); page++)
	{
		LP55231_WriteReg(REG_PROG_PAGE_SEL, page);

		for(uint8_t i = 0; i < 16; i++)
		{
			uint8_t buffer[2] = {0};
			buffer[0] = (prog[(i + (page*16))]>> 8) & 0xff;
			buffer[1] = prog[i + (page*16)] & 0xff;
			LP55231_WriteData((REG_PROG_MEM_BASE + (i*2)), buffer, sizeof(buffer));
		}
	}

	/* plus any incomplete pages */
	page = len/16;
	LP55231_WriteReg(REG_PROG_PAGE_SEL, page);
	for(uint8_t i = 0; i < (len%16); i++)
	{

		uint8_t buffer[2] = {0};
		buffer[0] = (prog[i + (page*16)]>> 8) & 0xff;
		buffer[1] = prog[i + (page*16)] & 0xff;
		LP55231_WriteData((REG_PROG_MEM_BASE + (i*2)), buffer, sizeof(buffer));
	}

	LP55231_WriteReg(REG_CNTRL2, 0x00);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Verify program
 *
 * @param 	prog		Program buffer pointer.
 * @param 	len			Program buffer lenght.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_VerifyProgram(const uint16_t* prog, uint8_t len)
{
	uint8_t page;

	if(len >= LP55231_NumInstructions)
	{
		return LP_ERR_PROGRAM_LENGTH;
	}

	LP55231_WriteReg(REG_CNTRL2, 0x00);	/* engines into disable mode - required for entry to program mode. */
	LP55231_WriteReg(REG_CNTRL2, 0x15); /* engines into program mode? */

	/* try to read  program from chip,
	 * datasheet says MSB of each instruction is in earlier address
	 * TBD: could optimize with a sequence of byte writes, using auto increment
	 */

	/* Auto-increment may not work for sequential reads... */
	for(page = 0; page < (len/16); page++)
	{
		LP55231_WriteReg(REG_PROG_PAGE_SEL, page);

		for(uint8_t i = 0; i < 16; i++)
		{
			uint16_t msb, lsb;
			uint8_t addr = (REG_PROG_MEM_BASE + (i*2));
			msb = LP55231_ReadReg(addr);
			lsb = LP55231_ReadReg(addr + 1);

			lsb |= (msb << 8);

			if(lsb != prog[i + (page*16)])
			{
				return LP_ERR_PROGRAM_VALIDATION;
			}
		}
	}

	/* plus any incomplete pages */
	page = len/16;
	LP55231_WriteReg(REG_PROG_PAGE_SEL, page);
	for(uint8_t i = 0; i < (len%16); i++)
	{
		uint16_t msb, lsb;
		uint8_t addr = (REG_PROG_MEM_BASE + (i*2));

		msb = LP55231_ReadReg(addr);
		lsb = LP55231_ReadReg(addr + 1);

		lsb |= (msb << 8);

		if(lsb != prog[i + (page*16)])
		{
			return LP_ERR_PROGRAM_VALIDATION;
		}
	}

	LP55231_WriteReg(REG_CNTRL2, 0x00);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Set Engine entry point
 *
 * @param 	engine		Set specific engine
 * @param 	addr		Engine entry point
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEngineEntryPoint(uint8_t engine, uint8_t addr)
{

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	LP55231_WriteReg(REG_PROG1_START + engine, addr);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Set Engine Program Counter for program start address.
 *
 * @param 	engine		Set specific engine
 * @param 	addr		Program start point
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEnginePC(uint8_t engine, uint8_t addr)
{
	uint8_t control_val, control2_val, temp;;

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	/* There are 6 pages of 16 instructions each (0..95) */
	if(addr >= LP55231_NumInstructions)
	{
		return LP_ERR_PROGRAM_PC;
	}

	/* In Ctl1 descriptions:
	 * 00 = hold: Hold causes the execution engine to finish the current instruction and then stop. Program counter
	 * (PC) can be read or written only in this mode.
	 */

	control_val = LP55231_ReadReg(REG_CNTRL1);
	control2_val = LP55231_ReadReg(REG_CNTRL2);

	temp = (control_val & ~(0x30 >> (engine * 2)));

	LP55231_WriteReg(REG_CNTRL2, (uint8_t)0x3ff); /* halt engines immediately. */
	LP55231_WriteReg(REG_CNTRL1, temp);   /* put engine in load mode */

	LP55231_WriteReg(REG_PC1 + engine, addr);

	/*restore prev mode? */
	LP55231_WriteReg(REG_CNTRL1, control_val);
	LP55231_WriteReg(REG_CNTRL2, control2_val);

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Get Engine Program Counter address
 *
 * @param 	engine		Specific engine
 *
 * @return 	pc_val		Program counter addres
 *
 *******************************************************************************/
uint8_t LP55231_GetEnginePC(uint8_t engine)
{
	uint8_t pc_val;

	if(engine >= LP55231_NumEngines)
	{
		return -1;
	}

	pc_val = LP55231_ReadReg(REG_PC1 + engine);

	return(pc_val);
}

/***************************************************************************//**
 * @brief  				Get Engine Mode
 *
 * @param 	engine		Specific engine
 *
 * @return 	mode		Engine mode
 *
 *******************************************************************************/
uint8_t LP55231_GetEngineMode(uint8_t engine)
{
	uint8_t val;

	if(engine >= LP55231_NumEngines)
	{
		return 0xff;
	}

	val = LP55231_ReadReg(REG_CNTRL1);
	val >>= (engine * 2);
	val &= 0x03;
	return(val);

}

/***************************************************************************//**
 * @brief  				Get Engine Map
 *
 * @param 	engine		Specific engine
 *
 * @return 	map			Engine map number
 *
 *******************************************************************************/
uint8_t LP55231_GetEngineMap(uint8_t engine)
{
	if(engine >= LP55231_NumEngines)
	{
		return 0xFF;
	}

	return(LP55231_ReadReg(REG_ENG1_MAP_LSB + engine));
}

/***************************************************************************//**
 * @brief  				Engine mode hold
 *
 * @param 	engine		Set specific engine
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEngineModeHold(uint8_t engine)
{
	uint8_t val;

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	/* Set the enghine to "free running" execution type
	 * bits to 0b00
	 */
	val = LP55231_ReadReg(REG_CNTRL1);
	val &= ~(0x30 >> (engine * 2));
	LP55231_WriteReg(REG_CNTRL1, val );

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Set Engine Mode Step
 *
 * @param 	engine		Set specific engine
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEngineModeStep(uint8_t engine)
{
	uint8_t val;

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	/* Set the enghine to "single step" execution type
	 * bits to 0b01
	 */
	val = LP55231_ReadReg(REG_CNTRL1);
	val &= ~(0x30 >> (engine * 2));
	val |= (0x10 >> (engine * 2));
	LP55231_WriteReg(REG_CNTRL1, val );

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Set Engine Mode once
 *
 * @param 	engine		Set specific engine
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEngineModeOnce(uint8_t engine)
{
	uint8_t val;

	/* This mode might not be the most useful.
	 * It executes the pointed instruction, then
	 * sets exec mode to hold, and resets the PC.
	 * It's an astringent form of step (which advances the PC, instead)
	 */

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	/* Set the enghine to "one shot" execution type
	 * Bits to 0b11
	 */
	val = LP55231_ReadReg(REG_CNTRL1);
	val |= (0x30 >> (engine * 2));

	LP55231_WriteReg(REG_CNTRL1, val );

	return LP_ERR_NONE;

}

/***************************************************************************//**
 * @brief  				Set Engine Mode Free
 *
 * @param 	engine		Set specific engine
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEngineModeFree(uint8_t engine)
{
	uint8_t val;

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	/* Set the enghine to "free running" execution type */
	val = LP55231_ReadReg(REG_CNTRL1);
	val &= ~(0x30 >> (engine * 2));
	val |= (0x20 >> (engine * 2));

	LP55231_WriteReg(REG_CNTRL1, val );

	return LP_ERR_NONE;
}

/***************************************************************************//**
 * @brief  				Set Engine to running mode
 *
 * @param 	engine		Set specific engine
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetEngineRunning(uint8_t engine)
{
	uint8_t val;

	if(engine >= LP55231_NumEngines)
	{
		return LP_ERR_INVALID_ENGINE;
	}

	/* This assumes that a suitable run mode in CNTRL1 was already selected.
	 * start execution by setting "run program" mode
	 */
	val = LP55231_ReadReg(REG_CNTRL2);
	val &= ~(0x30 >> (engine * 2));
	val |= (0x20>> (engine * 2));
	LP55231_WriteReg(REG_CNTRL2, val);

	return LP_ERR_NONE;
}


/***************************************************************************//**
 * @brief  				Clear interrupts when reading IRQ status register.
 *
 * @return 	value		IRQ status register value
 *
 *******************************************************************************/
uint8_t LP55231_ClearInterrupt()
{
	/* ToDo: make this more channel specific? */
	return( LP55231_ReadReg(REG_STATUS_IRQ) & 0x07);
}

/***************************************************************************//**
 * @brief  				Set Int pin mode as interrupt or GPO
 *
 * @param 	overrideOn	If 1 pin is set as GPO, otherwise pin is set as interrupt.
 *
 *******************************************************************************/
void LP55231_OverrideIntToGPO(uint8_t overrideOn )
{
	uint8_t regVal;

	if(overrideOn)
	{
		regVal = 0x04;
	}
	else
	{
		regVal = 0;
	}

	LP55231_WriteReg(REG_INT_GPIO, regVal);
}

/***************************************************************************//**
 * @brief  				Set Int pin state
 *
 * @param 	value		If 1 pin state is  high, otherwise pin state is low.
 *
 * @return 	error		return error code. See led_drv_lp55231.h for more details about error codes.
 *
 *******************************************************************************/
eLP55231ErrCode LP55231_SetIntGPOVal(uint8_t value)
{
	uint8_t regVal;

	regVal = LP55231_ReadReg(REG_INT_GPIO);

	if (!(regVal & 0x04))
	{
		return LP_ERR_GPIO_OFF;
	}

	if(value)
	{
		regVal |= 0x01;
	}
	else
	{
		regVal &= ~0x01;
	}

	LP55231_WriteReg(REG_INT_GPIO, regVal);

	return LP_ERR_NONE;
}


/***************************************************************************//**
 * @brief  				Temperature reading acquired from the internal temperature sensor
 *
 * @return 	temperature Internal temperature value is between -41 and 89 degree celsius.
 *
 *******************************************************************************/
int8_t LP55231_ReadDegC(void)
{
	uint8_t status;
	int8_t  temperature;

	LP55231_WriteReg(REG_TEMP_CTL, 0x04);

	do
	{
		status = LP55231_ReadReg(REG_TEMP_CTL);
	}while(status & 0x80);

	temperature = (int8_t)LP55231_ReadReg(REG_TEMP_READ);

	return temperature;
}

/***************************************************************************//**
 * @brief  				Reading specific led channel voltage value from the internal adc
 *
 * @param 	channel 	Specific channel for adc reading
 *
 * @return 	volts 		voltage value from adc channel.
 *
 *******************************************************************************/
float LP55231_ReadLEDADC(uint8_t channel)
{
	uint8_t reading;
	float volts;

	if(channel >= LP55231_NumChannels)
	{
		return 0.0;
	}

	reading = LP55231_ReadADCInternal(channel & 0x0f);

	volts = (reading * 0.03) - 1.478;
	return volts;
}

/***************************************************************************//**
 * @brief  				Reading Vout pin voltage value from the internal adc
 *
 * @return 	volts 		voltage value from adc channel.
 *
 *******************************************************************************/
float LP55231_ReadVoutADC(void)
{
	uint8_t reading;
	float volts;

	reading = LP55231_ReadADCInternal(0x0f);

	volts = (reading * 0.03) - 1.478;
	return volts;
}

/***************************************************************************//**
 * @brief  				Reading Vdd pin voltage value from the internal adc
 *
 * @return 	volts 		voltage value from adc channel.
 *
 *******************************************************************************/
float LP55231_ReadVddADC(void)
{
	uint8_t reading;
	float volts;

	reading = LP55231_ReadADCInternal(0x10);

	volts = (reading * 0.03) - 1.478;
	return volts;
}

/***************************************************************************//**
 * @brief  				Reading Interrupt pin voltage value from the internal adc
 *
 * @return 	volts 		voltage value from adc channel.
 *
 *******************************************************************************/
float LP55231_ReadIntADC(void)
{
	/* reads voltage at interrupt pin */

	uint8_t reading;
	float volts;

	reading = LP55231_ReadADCInternal(0x11);

	volts = (reading * 0.03) - 1.478;
	return volts;
}

/***************************************************************************//**
 * @brief  				Check led driver availability
 *
 *******************************************************************************/
void LP55231_WaitForBusy(void)
{
	uint8_t val;

	/* then wait to change modes */
	do
	{
		val = LP55231_ReadReg(REG_STATUS_IRQ) & 0x10; /* engine busy bit */
	}
	while(val);

}


/***************************************************************************//**
 * @brief  				Reading specific channel voltage value from the internal adc
 *
 * @param 	channel		adc channel number
 *
 * @return 	volts 		voltage value from internal adc.
 *
 *******************************************************************************/
uint8_t LP55231_ReadADCInternal(uint8_t channel)
{
	LP55231_WriteReg(REG_TEST_CTL, 0x80 |(channel & 0x1f));

	/* No reg bit to poll for completing - simply delay. */
	HAL_Delay(3);

	return(LP55231_ReadReg(REG_TEST_ADC));

}


/**@}*/
/******************************************************************************/
/*   Copyright (C) Rota Teknik 2021,  All Rights Reserved. Confidential.      */
/******************************************************************************/
