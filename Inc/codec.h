#ifndef __CODEC_H
#define __CODEC_H

#include "main.h"
#include <speex/speex.h>
#include <speex/speex_preprocess.h>
//#include "config.h"

#define FRAME_SIZE              160                 //frame for 20ms 
#define ENCODED_FRAME_SIZE      5                   //frome for 2Kb/s (if you want change it, you must change quality)
#define SAMPLING_RATE           8000                //sampling rate need for preprocessor

#define lengt_audio             FRAME_SIZE*4        //length audio geting from I2S
#define half_lengt_audio        FRAME_SIZE*2        //left + right chanel

void encode_data(void);
void decode_data(void);
void Speex_codec_Init(void);

#endif

