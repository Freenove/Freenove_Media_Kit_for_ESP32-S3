#ifndef __DRIVER_AUDIO_INPUT_H
#define __DRIVER_AUDIO_INPUT_H

#include "Arduino.h"
#include "ESP_I2S.h"

void audio_input_init(uint8_t sck, uint8_t ws, uint8_t din);
void audio_input_deinit(void);
uint8_t* audio_input_record_wav(uint32_t duration, size_t& wav_size);
void audio_input_print_buffer(uint8_t* buffer, size_t size);
size_t audio_input_read_iis_data(char* buffer, size_t size);
int audio_input_get_iis_data_available(void);
#endif