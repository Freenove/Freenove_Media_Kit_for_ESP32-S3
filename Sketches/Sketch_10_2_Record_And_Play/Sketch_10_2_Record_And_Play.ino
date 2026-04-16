/*
* Sketch_10_2_Record_And_Play.ino
* This sketch records audio data from an audio input using the I2S bus and saves it as a WAV file on an SD card.
* It also plays back the recorded audio files using the same I2S bus.
* The recording and playback are controlled by a button press.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include "driver_audio_input.h"
#include "driver_audio_output.h"
#include "driver_sdmmc.h"

#define RECORDER_FOLDER "/recorder"
#define BUTTON_PIN 19         // Please do not modify it.
#define SD_MMC_CMD 38         // Please do not modify it.
#define SD_MMC_CLK 39         // Please do not modify it.
#define SD_MMC_D0 40          // Please do not modify it.
#define AUDIO_INPUT_SCK 3     // Please do not modify it.
#define AUDIO_INPUT_WS 14     // Please do not modify it.
#define AUDIO_INPUT_DIN 46    // Please do not modify it.
#define AUDIO_OUTPUT_BCLK 42  // Please do not modify it.
#define AUDIO_OUTPUT_LRC 41   // Please do not modify it.
#define AUDIO_OUTPUT_DOUT 1   // Please do not modify it.

int recorder_task_flag = 0;  // Task status flag (0=stopped, 1=running)
uint8_t *wav_buffer;
size_t wav_size;

void setup() {
  // Initialize the serial port
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize I2S bus for audio input and output
  audio_input_init(AUDIO_INPUT_SCK, AUDIO_INPUT_WS, AUDIO_INPUT_DIN);
  i2s_output_init(AUDIO_OUTPUT_BCLK, AUDIO_OUTPUT_LRC, AUDIO_OUTPUT_DOUT);
  //audio_output_set_volume(5);  // Set volume to maximum (0...21)

  // Initialize SD card for storing audio files
  sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  remove_dir(RECORDER_FOLDER); 
  create_dir(RECORDER_FOLDER);  // Create a new recorder folder if it doesn't exist
}

void loop() {
  if (analogRead(BUTTON_PIN) <= 100) {
    if(recorder_task_flag==0){
      if(wav_buffer!=NULL)
      {
        free(wav_buffer); // Free the allocated memory for the audio buffer
        wav_buffer = NULL;
      }
      int file_count = read_file_num(RECORDER_FOLDER);
      String file_name = String(RECORDER_FOLDER) + "/recorder_" + String(file_count) + ".wav";
      Serial.println(file_name);
      wav_buffer = audio_input_record_wav(3, &wav_size);    // Record 3 seconds of audio
      write_file(file_name.c_str(), wav_buffer, wav_size);  // Save the recorded audio to the SD card
      Serial.println("Writing audio data to file..." + String(wav_size));
      recorder_task_flag = 1;                               // Stop the recording task
    }
    else if(recorder_task_flag==1)
    {
      Serial.printf("wav_buffer:%d, wav_size:%d\r\n", sizeof(wav_buffer), wav_size);
      i2s_output_wav(wav_buffer, wav_size);
      recorder_task_flag = 0; 
    }
  }
}
