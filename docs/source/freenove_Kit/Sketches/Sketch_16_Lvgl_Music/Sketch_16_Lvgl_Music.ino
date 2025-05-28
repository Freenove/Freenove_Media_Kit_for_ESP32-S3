/**
 * Sketch_15_Lvgl_Music.ino
 * This sketch demonstrates the use of the LVGL library to create a graphical user interface (GUI) for music playback
 * on an ESP32 microcontroller. It initializes a display, sets up an SD card for music storage, and initializes audio output.
 * The GUI includes elements managed by LVGL, and music files are loaded from the SD card for playback.
 *
 * Author: Zhentao Lin
 * Date:   2025-04-08
 */
#include "music_ui.h"

#define SD_MMC_CMD 38  //Please do not modify it.
#define SD_MMC_CLK 39  //Please do not modify it.
#define SD_MMC_D0 40   //Please do not modify it.
#define I2S_BCLK 42    //Please do not modify it.
#define I2S_DOUT 1     //Please do not modify it.
#define I2S_LRC 41     //Please do not modify it.

Display screen;  // Create an instance of the Display class

void setup() {
  /* Prepare for possible serial debug */
  Serial.begin(115200);  // Initialize serial communication at 115200 baud rate

  /* Initialize SD card */
  sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  audio_output_init(I2S_BCLK, I2S_LRC, I2S_DOUT);

  /*** Initialize the screen ***/
  screen.init(TFT_DIRECTION);  // Initialize the display

  // Create a string to display LVGL version information
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  // Print LVGL version information to the serial monitor
  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  setup_scr_music(&guider_music_ui);
  lv_scr_load(guider_music_ui.music);

  // Print setup completion message to the serial monitor
  Serial.println("Setup done");
}

void loop() {
  screen.routine(); /* Let the GUI do its work */  // Handle routine display tasks
  delay(5);                                        // Add a small delay to prevent the loop from running too fast
}
