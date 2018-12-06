#include "programm_vocoder.h"

__IO uint16_t audio_data_mic[lengt_audio];  //from UDA1380
__IO uint16_t audio_data_head[lengt_audio];	//for tx to UDA1380

/*flag for action*/
uint8_t flag_complet_rx_uart=0;
uint8_t flag_uart_tx=0;
uint8_t flag_i2s_rx=0;
uint8_t flag_i2s_tx=0;
uint8_t key=0;

uint8_t mic=0;

/*init perif and codec*/
void start_init(void)
{
    HAL_I2S_Receive_DMA(&hi2s3, (uint16_t*)audio_data_mic, sizeof(audio_data_mic)/2);
    HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t*)audio_data_head, sizeof(audio_data_head)/2);
    
    if(UDA1380_Init() == HAL_OK)
    {
        
    }
    else
    {
        UDA1380_Init();
    }
    
    Speex_codec_Init();	
    start_uart();
    
    /*for see Hard fault*/
    SCB->SHCSR |= SCB_SHCSR_BUSFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;
}

/*main programm*/
void run_vocoder(void)
{
    /*if compleat half data from UDA1380 encode this and transmit to uart*/
    if(flag_i2s_rx)
    {
        HAL_GPIO_WritePin(GPIOD, Led_blue_Pin, GPIO_PIN_SET); 
        make_mono();
        encode_data();
        uart_tx();
        HAL_GPIO_WritePin(GPIOD, Led_blue_Pin, GPIO_PIN_RESET); 
    }
    
    /*if data rx from uart doing decode data*/
    if(flag_complet_rx_uart)
    {
        HAL_GPIO_WritePin(GPIOD, Led_green_Pin, GPIO_PIN_SET);
        flag_complet_rx_uart=0;
        copy_rx_data();
        decode_data();
        HAL_GPIO_WritePin(GPIOD, Led_green_Pin, GPIO_PIN_RESET);
    }
    
    /*if compleat tx to UDA1380 half data, copy to this half decode data*/
    if(flag_i2s_tx)
    {
        HAL_GPIO_WritePin(GPIOD, Led_green_Pin, GPIO_PIN_SET);
        copy_data_tx_i2s();
        HAL_GPIO_WritePin(GPIOD, Led_green_Pin, GPIO_PIN_RESET);
    }
    
    /*if press key change source betwen microphone and linein*/
    if(key)
    {
        key=0;
        uint8_t set_mic;
        if(mic)
        {
            set_mic=0x0C;
            mic=0;
            HAL_GPIO_WritePin(GPIOD, Led_orange_Pin, GPIO_PIN_RESET);
        }
        else
        {
            set_mic=0x02;
            mic=1;
            HAL_GPIO_WritePin(GPIOD, Led_orange_Pin, GPIO_PIN_SET);
        }
        UDA1380_set_mic(set_mic);
    }
}
