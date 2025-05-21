/**
 * Sketch_12_LVGL_LedPixel.ino
 * This sketch demonstrates the use of the LVGL library along with WS2812 LED pixels to create a graphical user interface (GUI)
 * on an ESP32 microcontroller. It initializes a display, sets up WS2812 LEDs, and loads a specific UI screen using LVGL.
 * The GUI includes elements managed by LVGL, and the WS2812 LEDs are controlled via a custom UI setup.
 *
 * Author: Zhentao Lin
 * Date:   2025-04-08
 */
#include "display.h"
#include <lvgl.h>
#include "ws2812_ui.h"

Display screen; // Create an instance of the Display class

void setup()
{
  /* Prepare for possible serial debug */
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate

  /*** Initialize the screen ***/
  screen.init(TFT_DIRECTION); // Initialize the display

  // Create a string to display LVGL version information
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

  // Print LVGL version information to the serial monitor
  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  // Setup the WS2812 UI screen
  setup_scr_ws2812(&guider_ws2812_ui);
  // Load the WS2812 UI screen
  lv_scr_load(guider_ws2812_ui.ws2812);

  // Print setup completion message to the serial monitor
  Serial.println("Setup done");
}

void loop()
{
  screen.routine(); /* Let the GUI do its work */ // Handle routine display tasks
  delay(5);                                       // Add a small delay to prevent the loop from running too fast
}