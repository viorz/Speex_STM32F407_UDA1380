#ifndef __PROGRAMM_VOCODER_H__
#define __PROGRAMM_VOCODER_H__

#include "main.h"
#include "i2s.h"
#include "gpio.h"
#include "uda1380.h"
#include "codec.h"
#include "function.h"

void start_init(void);
void run_vocoder(void);

#endif
