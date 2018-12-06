#include "function.h"

/*variables*/
__IO char data_encoded[2][ENCODED_FRAME_SIZE];
__IO char data_for_decode[2][ENCODED_FRAME_SIZE];
int16_t mono_data_for_encode[2][FRAME_SIZE];
int16_t mono_data_decoded[2][FRAME_SIZE];
uint8_t RX_uart[ENCODED_FRAME_SIZE];
uint8_t TX_uart[ENCODED_FRAME_SIZE];

uint8_t clkNtx=1;

/*EXTERN DATA*/
/*flag for action*/
extern uint8_t flag_complet_rx_uart;
extern uint8_t flag_uart_tx;
extern uint8_t flag_i2s_rx;
extern uint8_t flag_i2s_tx;
extern uint8_t key;

extern uint16_t audio_data_mic[lengt_audio];
extern uint16_t audio_data_head[lengt_audio];




/*copy rx data from uart for decode*/
void copy_rx_data(void)
{
	if(clkNtx==1)
	{
		for(uint8_t i=0; i<(sizeof(data_for_decode[0])); i++)
		{
			data_for_decode[0][i]=RX_uart[i];
		}
	}
	else if(clkNtx==2)
	{
		for(uint16_t i=0; i<(sizeof(data_for_decode[1])); i++)
		{
			data_for_decode[1][i]=RX_uart[i];
		}
	}
}


/*give only right chanel*/
void make_mono(void)
{
	if(flag_i2s_rx==1)
	{
		flag_uart_tx=1;
		for(uint16_t i=0; i<(sizeof(mono_data_for_encode[0])/2); i++)
		{
			mono_data_for_encode[0][i]=audio_data_mic[i*2];
		}
	}
	if(flag_i2s_rx==2)
	{
		flag_uart_tx=2;
		for(uint16_t i=0; i<(sizeof(mono_data_for_encode[1])/2); i++)
		{
			mono_data_for_encode[1][i]=audio_data_mic[i*2+(half_lengt_audio)];
		}
	}
	flag_i2s_rx=0;
}


/*prepare data for tx to uart*/
void uart_tx(void)
{
	if(flag_uart_tx==1)
	{
		flag_uart_tx=0;
		for(uint8_t i=0; i<(sizeof(data_encoded[0])); i++)
		{
			TX_uart[i]=data_encoded[0][i];
		}
	}
	if(flag_uart_tx==2)
	{
		flag_uart_tx=0;
		for(uint8_t i=0; i<(sizeof(data_encoded[1])); i++)
		{
			TX_uart[i]=data_encoded[1][i];
		}
	}
	HAL_UART_Transmit_DMA(&huart2, TX_uart, sizeof(TX_uart));
}


/*prepare audio out data*/
void copy_data_tx_i2s(void)
{
	if(flag_i2s_tx==1)
	{
		for(uint16_t i=0; i<(FRAME_SIZE); i++)
		{
			audio_data_head[i*2+1]=mono_data_decoded[0][i];
		}
	}
	if(flag_i2s_tx==2)
	{
		for(uint16_t i=0; i<(FRAME_SIZE); i++)
		{
			audio_data_head[i*2+1+(half_lengt_audio)]=mono_data_decoded[1][i];
		}
	}
	flag_i2s_tx=0;
}


/*----------------------------HANDLERS INTERRUPTS----------------------------*/

/*start circular DMA rx uart*/
void start_uart(void)
{
	HAL_UART_Receive_DMA(&huart3, RX_uart, sizeof(RX_uart));
}
/*interrupt for completed callbacks from uart rx*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	flag_complet_rx_uart=1;
}
/*interrupt for half completed callbacks from tx i2s (UDA1380)*/
void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	flag_i2s_tx=1;
}
void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	flag_i2s_tx=2;
}
/*interrupt for half completed callbacks from rx i2s (UDA1380)*/
void HAL_I2S_RxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	flag_i2s_rx=1;
}
void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	flag_i2s_rx=2;
}
/*interrupt for changing the sound source*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	key=1;
}