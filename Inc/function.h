#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#include "main.h"
#include "usart.h"
#include "codec.h"
#include "programm_vocoder.h"

void start_uart(void);
void copy_rx_data(void);
void uart_tx(void);
void make_mono(void);
void copy_data_tx_i2s(void);

#endif
