/*
* Sketch_09_1_Record_To_WAV.ino
* This sketch records audio data from an audio input using the I2S bus and saves it as a WAV file on an SD card.
* It initializes the I2S bus, records audio for 5 seconds, and writes the audio data to the SD card.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include "driver_audio_input.h"
#include "driver_sdmmc.h"

#define RECORDER_FOLDER "/recorder"

#define SD_MMC_CMD      38  // Please do not modify it.
#define SD_MMC_CLK      39  // Please do not modify it.
#define SD_MMC_D0       40  // Please do not modify it.
#define AUDIO_INPUT_SCK  3  // Please do not modify it.
#define AUDIO_INPUT_WS  14  // Please do not modify it.
#define AUDIO_INPUT_DIN 46  // Please do not modify it.

void setup() {
   // Initialize the serial port
   Serial.begin(115200);
   while (!Serial) {
       delay(10);
   }

   // Initialize I2S bus for audio input
   audio_input_init(AUDIO_INPUT_SCK, AUDIO_INPUT_WS, AUDIO_INPUT_DIN);

   // Initialize SD card for storing audio files
   sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
   remove_dir(RECORDER_FOLDER);  // Remove the existing recorder folder if it exists
   create_dir(RECORDER_FOLDER);   // Create a new recorder folder

   Serial.println("Recording 5 seconds of audio data...");

   // Record 5 seconds of audio data and store it in a buffer
   size_t wav_size;
   uint8_t* wav_buffer = audio_input_record_wav(5, wav_size);

   // Print the first 1024 bytes of the recorded audio buffer for debugging
   audio_input_print_buffer(wav_buffer, 1024);

   // Construct the file name for the recorded audio file
   String file_name = String(RECORDER_FOLDER) + "/recorder_0.wav";
   Serial.println(file_name);

   // Write the recorded audio data to the SD card
   write_file(file_name.c_str(), wav_buffer, wav_size);
   Serial.println("Application complete.");
}

void loop() {
   // Main loop can be used for additional tasks if needed
}