/*
* Sketch_01_LedPixel_RGBW.ino
* This sketch demonstrates how to control an RGBW LED using the Freenove_WS2812_Lib_for_ESP32 library.
* It cycles through a set of predefined colors on a single LED connected to pin 48.
* 
* Author: [Zhentao Lin]
* Date:   [2025-04-07]
*/

#include "Freenove_WS2812_Lib_for_ESP32.h"

#define LEDS_COUNT 1  // Number of LEDs in the strip
#define LEDS_PIN 48   // GPIO pin connected to the LED strip
#define CHANNEL 0     // PWM channel for LED control

// Initialize the LED strip with the specified parameters
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);

// Array of colors to cycle through: Red, Green, Blue, White, Off
uint8_t m_color[5][3] = { { 255, 0, 0 }, { 0, 255, 0 }, { 0, 0, 255 }, { 255, 255, 255 }, { 0, 0, 0 } };
int delayval = 100;  // Delay between color changes in milliseconds

// Setup function runs once when the program starts
void setup() {
  strip.begin();            // Initialize the LED strip
  strip.setBrightness(10);  // Set the brightness of the LED strip (0-100)
}

// Loop function runs continuously after setup
void loop() {
  for (int j = 0; j < 5; j++) {                                               // Loop through each color
    for (int i = 0; i < LEDS_COUNT; i++) {                                    // Loop through each LED (only one in this case)
      strip.setLedColorData(i, m_color[j][0], m_color[j][1], m_color[j][2]);  // Set the color of the LED
      strip.show();                                                           // Update the LED strip with the new color
      delay(delayval);                                                        // Wait for a short period before changing the color again
    }
    delay(500);  // Wait for half a second before moving to the next color
  }
}