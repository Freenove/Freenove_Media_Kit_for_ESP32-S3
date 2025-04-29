/*
* Sketch_09_2_Record_And_Play.ino
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

void setup() {
  // Initialize the serial port
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Initialize I2S bus for audio input and output
  audio_input_init(AUDIO_INPUT_SCK, AUDIO_INPUT_WS, AUDIO_INPUT_DIN);
  audio_output_init(AUDIO_OUTPUT_BCLK, AUDIO_OUTPUT_LRC, AUDIO_OUTPUT_DOUT);
  audio_output_set_volume(21);  // Set volume to maximum (0...21)

  // Initialize SD card for storing audio files
  sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  create_dir(RECORDER_FOLDER);  // Create a new recorder folder if it doesn't exist
}

void loop() {
  if (analogRead(BUTTON_PIN) <= 100) {
    start_recorder_task();  // Start recording if button is pressed
  } else {
    if (recorder_task_is_running())
      stop_recorder_task();  // Stop recording if button is released
    audio_output_loop();     // Loop to play audio if available
    if (!audio_output_is_running()) {
      int file_count = read_file_num(RECORDER_FOLDER) - 1;
      if (file_count >= 0) {
        String file_name = String(RECORDER_FOLDER) + "/recorder_" + String(file_count) + ".wav";
        Serial.println(file_name);
        audio_output_load_music(file_name.c_str());  // Load and play the last recorded file
      }
    }
  }
}

/* Start recording task */
void start_recorder_task(void) {
  if (recorder_task_flag == 0) {
    recorder_task_flag = 1;
    xTaskCreate(loopTask_sound_recorder, "loopTask_sound_recorder", 4096, NULL, 1, NULL);  // Create a new task for recording
  }
}

/* Stop recording task */
void stop_recorder_task(void) {
  if (recorder_task_flag == 1) {
    recorder_task_flag = 0;
    Serial.println("loopTask_sound_recorder deleted!");  // Indicate that the recording task is stopped
  }
}

/* Check if recording task is active */
int recorder_task_is_running(void) {
  return recorder_task_flag;  // Return the status of the recording task
}

/* Main recording task loop */
void loopTask_sound_recorder(void *pvParameters) {
  Serial.println("loopTask_sound_recorder start...");
  while (recorder_task_flag == 1) {
    size_t wav_size;
    uint8_t *wav_buffer = audio_input_record_wav(5, wav_size);  // Record 5 seconds of audio
    int file_count = read_file_num(RECORDER_FOLDER);
    String file_name = String(RECORDER_FOLDER) + "/recorder_" + String(file_count) + ".wav";
    Serial.println(file_name);
    write_file(file_name.c_str(), wav_buffer, wav_size);  // Save the recorded audio to the SD card
    free(wav_buffer);                                     // Free the allocated memory for the audio buffer
    recorder_task_flag = 0;                               // Stop the recording task
  }
  Serial.println("loopTask_sound_recorder stop...");
  vTaskDelete(NULL);  // Delete the recording task
}