/******************** (C) COPYRIGHT 2014 STMicroelectronics ********************
* File Name          : stc31xx_I2cCustomReadWrite.c
* Author             : AMS - AAS application
* Version            : V2
* Date               : 2014/10
* Description        : gas gauge firmware for STC31xx
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.

* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*******************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "stc31xx_I2cCustomReadWrite.h"

/* Custom Includes --------*/
#include "i2c.h"
#include "FreeRTOS.h"


/* Local Define ------------------------------------------------------------------*/
#define STC3100_SLAVE_ADDRESS_8BIT       0xE0   /* STC3100 8-bit address byte */
#define STC3100_SLAVE_ADDRESS_7BIT       0x70   /* STC3100 7-bit address byte */

/* -----------------------------------------------------------------
The STC3100 routines use these I2C primitives 
to comunicate with device at address u8_I2C_address (see i2c.h):
I2c_CustomInit();
STC3100_Read(u8_NumberOfBytes, u8_RegAddress , pu8_RxBuffer);
STC3100_Write(u8_NumberOfBytes, u8_RegAddress , pu8_TxBuffer);
----------------------------------------------------------------- */

int I2c_CustomInit(void)
{
	return 0;
}

/*******************************************************************************
* Function Name  : STC3100_Write
* Description    : utility function to write several bytes to STC3100 registers
* Return         : error status
*******************************************************************************/

int STC3100_Write(unsigned char ByteCount, unsigned char RegisterAddr , unsigned char * TxBuffer)
{
	HAL_StatusTypeDef halRet = HAL_OK;
	uint8_t *buf;

	if(ByteCount == 0)
	{
		return(halRet = HAL_ERROR);
	}

	buf = pvPortMalloc((size_t)(ByteCount + 1));

	if(!buf)
	{
		return(halRet = HAL_ERROR);
	}

	buf[0] = RegisterAddr;

	for(int i = 0; i < ByteCount; i++)
	{
		buf[i+1] = TxBuffer[i];
	}

	halRet = HAL_I2C_Master_Transmit(&hi2c3, STC3100_SLAVE_ADDRESS_8BIT, buf, ByteCount+1, HAL_MAX_DELAY);

	if(halRet != HAL_OK)
	{
		return halRet;
	}

	vPortFree(buf);

	return halRet;
}

/*******************************************************************************
* Function Name  : STC3100_Read
* Description    : utility function to read several bytes from STC3100 registers
* Return         : error status
*******************************************************************************/

int STC3100_Read(unsigned char ByteCount, unsigned char RegisterAddr , unsigned char * RxBuffer)
{
	HAL_StatusTypeDef halRet = HAL_OK;

	if(!RxBuffer)
	{
		return( halRet = HAL_ERROR);
	}

	halRet = HAL_I2C_Master_Transmit(&hi2c3, STC3100_SLAVE_ADDRESS_8BIT, &RegisterAddr, 1, HAL_MAX_DELAY);

	if(halRet != HAL_OK)
	{
		return halRet;
	}

	halRet = HAL_I2C_Master_Receive(&hi2c3, STC3100_SLAVE_ADDRESS_8BIT, RxBuffer, ByteCount, HAL_MAX_DELAY);

	if(halRet != HAL_OK)
	{
		return halRet;
	}

	return halRet;
}

/* ---- end of I2C primitive interface --------------------------------------------- */

