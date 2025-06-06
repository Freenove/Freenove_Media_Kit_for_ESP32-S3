/**
 * Sketch_16_Lvgl_Sound_Recorder.ino
 * This sketch demonstrates the use of the LVGL library to create a graphical user interface (GUI) for sound recording
 * on an ESP32 microcontroller. It initializes a display, sets up audio input and output, and initializes an SD card for storing recordings.
 * The GUI includes elements managed by LVGL, and sound recordings can be made and saved to the SD card.
 *
 * Author: Zhentao Lin
 * Date:   2025-04-08
 */
#include "recorder_ui.h"

#define SD_MMC_CMD 38         // Please do not modify it.
#define SD_MMC_CLK 39         // Please do not modify it.
#define SD_MMC_D0 40          // Please do not modify it.
#define AUDIO_INPUT_SCK 3     // Please do not modify it.
#define AUDIO_INPUT_WS 14     // Please do not modify it.
#define AUDIO_INPUT_DIN 46    // Please do not modify it.
#define AUDIO_OUTPUT_BCLK 42  // Please do not modify it.
#define AUDIO_OUTPUT_LRC 41   // Please do not modify it.
#define AUDIO_OUTPUT_DOUT 1   // Please do not modify it.

Display screen;  // Create an instance of the Display class

void setup() {
  /* Prepare for possible serial debug */
  Serial.begin(115200);
  while (!Serial) {
    delay(10);
  }

  audio_input_init(AUDIO_INPUT_SCK, AUDIO_INPUT_WS, AUDIO_INPUT_DIN);
  audio_output_init(AUDIO_OUTPUT_BCLK, AUDIO_OUTPUT_LRC, AUDIO_OUTPUT_DOUT);
  sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  audio_output_set_volume(21);

  // Create recording directory if not exists
  create_dir(RECORDER_FOLDER);

  /*** Initialize the screen ***/
  screen.init(TFT_DIRECTION);  // Initialize the display

  // Create a string to display LVGL version information
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  // Print LVGL version information to the serial monitor
  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  setup_scr_sound_recorder(&guider_recorder_ui);
  lv_scr_load(guider_recorder_ui.recorder);

  // Print setup completion message to the serial monitor
  Serial.println("Setup done");
}

void loop() {
  screen.routine(); /* Let the GUI do its work */  // Handle routine display tasks
  delay(5);                                        // Add a small delay to prevent the loop from running too fast
}
