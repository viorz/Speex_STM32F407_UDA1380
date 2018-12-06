#include "codec.h"


/*SPEEX*/
int quality = 0, complexity=10, vbr=0, enh=1;/* SPEEX PARAMETERS, MUST REMAINED UNCHANGED */
SpeexBits bits;/* Holds bits so they can be read and written by the Speex routines */
void *enc_state, *dec_state;/* Holds the states of the encoder & the decoder */
SpeexPreprocessState *preprocess_state;

extern int16_t mono_data_for_encode[2][FRAME_SIZE];
extern char data_encoded[2][ENCODED_FRAME_SIZE];
extern char data_for_decode[2][ENCODED_FRAME_SIZE];
extern int16_t mono_data_decoded[2][FRAME_SIZE];

extern uint8_t flag_uart_tx;
extern uint8_t clkNtx;



/*speex echo canceller and encoder*/
void encode_data()
{
	if(flag_uart_tx==1)
	{
		speex_preprocess_run(preprocess_state, (spx_int16_t*)mono_data_for_encode[0]);
		speex_bits_reset(&bits);
		speex_encode_int(enc_state, (spx_int16_t*)mono_data_for_encode[0], &bits);
		speex_bits_write(&bits, (char *)data_encoded[0], ENCODED_FRAME_SIZE);
	}
	if(flag_uart_tx==2)
	{
		speex_preprocess_run(preprocess_state, (spx_int16_t*)mono_data_for_encode[1]);
		speex_bits_reset(&bits);
		speex_encode_int(enc_state, (spx_int16_t*)mono_data_for_encode[1], &bits);
		speex_bits_write(&bits, (char *)data_encoded[1], ENCODED_FRAME_SIZE);
	}
}

/*speex decoder*/
void decode_data()
{
    if(clkNtx==1)
    {
        speex_bits_read_from(&bits, (char *)data_for_decode[0], ENCODED_FRAME_SIZE);
        speex_decode_int(dec_state, &bits, (spx_int16_t*)mono_data_decoded[0]);
    }
    if(clkNtx==2)
    {
        speex_bits_read_from(&bits, (char *)data_for_decode[1], ENCODED_FRAME_SIZE);
        speex_decode_int(dec_state, &bits, (spx_int16_t*)mono_data_decoded[1]);
        clkNtx=0;
    }
    clkNtx++;
}


void Speex_codec_Init(void)
{
    /* Speex encoding initializations */ 
    speex_bits_init(&bits);
    enc_state = speex_encoder_init(&speex_nb_mode);
    speex_encoder_ctl(enc_state, SPEEX_SET_VBR, &vbr);
    speex_encoder_ctl(enc_state,SPEEX_SET_QUALITY,&quality);
    speex_encoder_ctl(enc_state, SPEEX_SET_COMPLEXITY, &complexity);
    speex_encoder_ctl(enc_state, SPEEX_SET_ENH, &enh);
        
    /* speex decoding intilalization */
    dec_state = speex_decoder_init(&speex_nb_mode);
    speex_decoder_ctl(dec_state, SPEEX_SET_ENH, &enh);
        
    /* speex preprocessor intilalization */
    preprocess_state = speex_preprocess_state_init(FRAME_SIZE, SAMPLING_RATE);
}
