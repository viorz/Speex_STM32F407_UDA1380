/**
  ******************************************************************************
  * File Name          : uda1380.c
  * Description        : used for uda1380 init.
  ******************************************************************************
*/
#include "uda1380.h"

uint8_t UDA1380InitData[][3] =
{
	/*
	 *Enable all power for now
	 */
	{UDA1380_REG_PWRCTRL,     0xA5, 0x5F},
	
	/*
	 *CODEC ADC and DAC clock from WSPLL, all clocks enabled
	 */ 
	{UDA1380_REG_EVALCLK,     0x0F, 0x39},
	
	/*
	 *I2S bus data I/O formats, use digital mixer for output
	 *BCKO is slave
	 */
	{UDA1380_REG_I2S,         0x00, 0x00},

	/*
	 *Full mixer analog input gain
	 */
	{UDA1380_REG_ANAMIX,      0x00, 0x00},
	
	/* 
	 *Enable headphone short circuit protection
	 */
	{UDA1380_REG_HEADAMP,     0x02, 0x02},
	
	/*
	 *Full master volume
	 */
	{UDA1380_REG_MSTRVOL,     0x55, 0x55},
	
	/*
	 *Enable full mixer volume on both channels
	 */
	{UDA1380_REG_MIXVOL,      0x00, 0x00},
	
	/*
	 *Bass and treble boost set to flat
	 */
	{UDA1380_REG_MODEBBT,     0x00, 0x00},
	
	/*
	 *Disable mute and de-emphasis
	 */
	{UDA1380_REG_MSTRMUTE,    0x00, 0x00},
	
	/*
	 *Mixer off, other settings off
	 */
	{UDA1380_REG_MIXSDO,      0x00, 0x00},
	
	/*
	 *ADC decimator volume to max
	 */
	{UDA1380_REG_DECVOL,      0x00, 0x00},
	
	/*
	 *No PGA mute, full gain
	 */ 
	{UDA1380_REG_PGA,         0x00, 0x00},
	
	/*
	 *Select line in and MIC, max MIC gain
	 */
	{UDA1380_REG_ADC,         0x08, 0x0C},
	
	/*
	 *AGC
	 */
	{UDA1380_REG_AGC,         0x00, 0x00},
	
	/*
	 *Disable clocks to save power
     *{UDA1380_REG_EVALCLK,     0x00, 0x32},
     *disable power to input to save power
     *{UDA1380_REG_PWRCTRL,     0xA5, 0xC0},
	 */
	 
    /*
     *End of list
	 */
	{0xFF,                    0xFF, 0xFF}
};

uint32_t AudioTotalSize ;         /* This variable holds the total size of the audio file */
int32_t AudioRemSize;            /* This variable holds the remaining data in audio file */
uint16_t *CurrentPos;             /* This variable holds the current position of audio pointer */

/**
  * @brief UDA1380 Init
  * @param None
  * @retval None
  */
HAL_StatusTypeDef UDA1380_Init(void)
{
	uint8_t i=0;
	HAL_StatusTypeDef status;
	
	while (UDA1380InitData[i][0]!=0xff) 
	{
		status = HAL_I2C_Master_Transmit(&hi2c1, UDA1380_WRITE_ADDRESS, UDA1380InitData[i], 3, 1000);
		if(status)
		{
			return status;
		}
		i++;
	}
	
	return HAL_OK;
}

void UDA1380_set_mic(uint8_t data)
{
	uint8_t tx_data[3];
	tx_data[0] = UDA1380InitData[12][0];
	tx_data[1] = UDA1380InitData[12][1];
	tx_data[2] = data;
	HAL_I2C_Master_Transmit(&hi2c1, UDA1380_WRITE_ADDRESS, tx_data, 3, 10);
}
/*********************************************************************************************************
**End Of File
********************************************************************************************************/
