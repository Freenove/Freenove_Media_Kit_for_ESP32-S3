/*
* Sketch_10_3_Record_And_Play.ino
* This sketch records audio data from an audio input using the I2S bus and saves it as a WAV file on an SD card.
* It also plays back the recorded audio files using the same I2S bus.
* The recording and playback are controlled by a button press.
* 
* Author: Zhentao Lin
* Date:   2025-08-07
*/
#include "driver_audio_input.h"
#include "driver_audio_output.h"
#include "driver_button.h"
#include "driver_sdmmc.h"
#include <esp_heap_caps.h>

// Define the folder path for recording files
#define RECORDER_FOLDER "/recorder"
// Define the pin number for the button (do not modify)
#define BUTTON_PIN 19         
// Define the pin numbers for SDMMC interface (do not modify)
#define SD_MMC_CMD 38         
#define SD_MMC_CLK 39         
#define SD_MMC_D0 40          
// Define the pin numbers for audio input (do not modify)
#define AUDIO_INPUT_SCK 3     
#define AUDIO_INPUT_WS 14     
#define AUDIO_INPUT_DIN 46    
// Define the pin numbers for audio output (do not modify)
#define AUDIO_OUTPUT_BCLK 42  
#define AUDIO_OUTPUT_LRC 41   
#define AUDIO_OUTPUT_DOUT 1   

// Define the size of PSRAM in bytes
#define MOLLOC_SIZE (1024 * 1024)

// Create a button object with the specified pin
Button button(BUTTON_PIN);

// Flag to indicate the status of the recorder task (0=stopped, 1=running)
int recorder_task_flag = 0;       
// Flag to indicate the status of the player task (0=stopped, 1=running)
int player_task_flag = 0;        
// Save wav data
uint8_t *wav_buffer;

// Setup function to initialize the hardware and software components
void setup() {
  // Initialize the serial communication at 115200 baud rate
  Serial.begin(115200);
  // Wait for the serial port to be ready
  while (!Serial) {
    delay(10);
  }
  // Initialize the button
  button.init();

  // Initialize the I2S bus for audio input
  audio_input_init(AUDIO_INPUT_SCK, AUDIO_INPUT_WS, AUDIO_INPUT_DIN);
  // Initialize the I2S bus for audio output
  i2s_output_init(AUDIO_OUTPUT_BCLK, AUDIO_OUTPUT_LRC, AUDIO_OUTPUT_DOUT);

  // Initialize the SD card
  sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  // Remove the existing recorder folder if it exists
  remove_dir(RECORDER_FOLDER);
  // Create a new recorder folder
  create_dir(RECORDER_FOLDER);
}

// Main loop function that runs continuously
void loop() {
  // Scan the button state
  button.key_scan();
  // Handle button events
  handle_button_events();
  // Delay for 10 milliseconds
  delay(10);
}

// Function to handle button events
void handle_button_events() {
  // Get the current state of the button
  int button_state = button.get_button_state();
  // Get the key value associated with the button press
  int button_key_value = button.get_button_key_value();
  // Switch case based on the button key value
  switch (button_key_value) {
    case 1:
      // If the button is pressed, start the recorder task
      if (button_state == Button::KEY_STATE_PRESSED) {
        start_recorder_task();
      } 
      // If the button is released, stop the recorder task
      else if (button_state == Button::KEY_STATE_RELEASED) {
        stop_recorder_task();
      }
      break;
    case 2:
      // If the button is pressed, start the player task
      if (button_state == Button::KEY_STATE_PRESSED) {
        start_player_task();
      }
      break;
    case 3:
      // If the button is pressed, stop the player task
      if (button_state == Button::KEY_STATE_PRESSED) {
        stop_player_task();
      }
    case 4:
    case 5:
    default:
      // Default case for other button key values
      break;
  }
}

/* Start recording task */
void start_recorder_task(void) {
  // Check if the recorder task is not already running
  if (recorder_task_flag == 0) {
    // Set the recorder task flag to running
    recorder_task_flag = 1;
    // Create a new task for recording sound
    xTaskCreate(loop_task_sound_recorder, "loop_task_sound_recorder", 4096, NULL, 1, NULL);
  }
}

/* Stop recording task */
void stop_recorder_task(void) {
  // Check if the recorder task is running
  if (recorder_task_flag == 1) {
    // Set the recorder task flag to stopped
    recorder_task_flag = 0;
    // Print a message indicating the deletion of the recorder task
    Serial.println("loop_task_sound_recorder deleted!");
  }
}

/* Check if recording task is active */
int is_recorder_task_running(void) {
  // Return the status of the recorder task
  return recorder_task_flag;
}

/* Main recording task loop */
void loop_task_sound_recorder(void *pvParameters) {
  // Print a message indicating the start of the recording task
  Serial.println("loop_task_sound_recorder start...");
  // Initialize the total size of recorded data
  int total_size = 0;
  // Allocate memory in PSRAM for storing audio data
  if(wav_buffer!=NULL)
  {
    // Free the allocated memory in PSRAM
    heap_caps_free(wav_buffer);
    wav_buffer = NULL;
  }
  wav_buffer = (uint8_t *)heap_caps_malloc(MOLLOC_SIZE, MALLOC_CAP_SPIRAM);

  // Get the index for the next recording file
  int wav_index = read_file_num(RECORDER_FOLDER);
  // Generate the file name for the new recording
  String file_name = String(RECORDER_FOLDER) + "/recording_" + String(wav_index) + ".wav";
  Serial.printf("file_name:%s\r\n", file_name.c_str());

  // Loop while the recorder task is running
  while (recorder_task_flag == 1) {
    // Get the available IIS data size
    int iis_buffer_size = audio_input_get_iis_data_available();
    // Loop while there is IIS data available
    while (iis_buffer_size > 0) {
      // Check if the buffer is full
      if ((total_size + 512) >= MOLLOC_SIZE) {
        // Stop the recorder task if the buffer is full
        recorder_task_flag = 0;
        break;
      }
      // Read IIS data into the buffer
      int real_size = audio_input_read_iis_data((char*)wav_buffer + total_size, 512);
      // Update the total size of recorded data
      total_size += real_size;
      // Decrease the available IIS data size
      iis_buffer_size -= real_size;
    }
  }
  // Write the WAV header to the file
  bool state = write_wav_header(file_name.c_str(), total_size);
  Serial.printf("Write wav header state2:%d\r\n", state);

  // Append the recorded data to the file
  append_file(file_name.c_str(), wav_buffer, total_size);
  
  Serial.printf("write wav size:%d\r\n", total_size);
  // Print a message indicating the end of the recording task
  Serial.println("loop_task_sound_recorder stop...");
  // Delete the current task
  vTaskDelete(NULL);
}

/* Start player task */
void start_player_task(void) {
  // Check if the player task is not already running
  if (player_task_flag == 0) {
    // Set the player task flag to running
    player_task_flag = 1;
    // Create a new task for playing sound
    xTaskCreate(loop_task_play_handle, "loop_task_play_handle", 4096, NULL, 1, NULL);
  }
}

/* Stop player task */
void stop_player_task(void) {
  // Check if the player task is running
  if (player_task_flag == 1) {
    // Set the player task flag to stopped
    player_task_flag = 0;
    // Print a message indicating the deletion of the player task
    Serial.println("loop_task_play_handle deleted!");
  }
}

/* Check if player task is active */
int is_player_task_running(void) {
  // Return the status of the player task
  return player_task_flag;
}

/* Main player task loop */
void loop_task_play_handle(void *pvParameters) {
  // Print a message indicating the start of the player task
  Serial.println("loop_task_play_handle start...");
  // Loop while the player task is running
  while (player_task_flag == 1) {
      // Get the number of recorded files
      int file_count = read_file_num(RECORDER_FOLDER);
      // Generate the file name for the last recorded file
      String file_name = String(RECORDER_FOLDER) + String("/") + String(get_file_name_by_index(RECORDER_FOLDER, (file_count - 1)));
      size_t length = read_file_size(file_name.c_str());
      Serial.printf("file_name:%s, size:%d\r\n", file_name.c_str(), length);
      if(wav_buffer!=NULL)
      {
        // Free the allocated memory in PSRAM
        heap_caps_free(wav_buffer);
        wav_buffer = NULL;
      }
      wav_buffer = (uint8_t *)heap_caps_malloc(MOLLOC_SIZE, MALLOC_CAP_SPIRAM);
      read_file(file_name.c_str(), wav_buffer, length);
      i2s_output_wav(wav_buffer, length);
      // for(size_t i=0;i<44;i++)
      // {
      //   Serial.printf("0x%x ",wav_buffer[i]);
      //   if(i%4==3)
      //     Serial.println();
      // }
      player_task_flag=0;
  }
  // Print a message indicating the end of the player task
  Serial.println("loop_task_play_handle stop...");
  // Delete the current task
  vTaskDelete(NULL);
}

















