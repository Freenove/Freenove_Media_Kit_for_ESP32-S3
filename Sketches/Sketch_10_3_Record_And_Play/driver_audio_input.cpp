#include "driver_audio_input.h"

static I2SClass i2s;

void audio_input_init(uint8_t sck, uint8_t ws, uint8_t din) {
    i2s.setPins(sck, ws, -1, din);
    if (!i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_RIGHT)) {
        Serial.println("Failed to initialize I2S bus!");
        return;
    }
    Serial.println("I2S bus initialized.");
}

void audio_input_deinit(void)
{ 
    i2s.end(); 
}

uint8_t* audio_input_record_wav(uint32_t duration, size_t& wav_size) {
    return i2s.recordWAV(duration, &wav_size);
}

void audio_input_print_buffer(uint8_t* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        Serial.print(buffer[i]);
        Serial.print(" ");
    }
    Serial.println();
}

size_t audio_input_read_iis_data(char* buffer, size_t size) {
    return i2s.readBytes(buffer, size);
}

int audio_input_get_iis_data_available(void) {
    return i2s.available();
}