/*
* Sketch_02_2_Button.ino
* This sketch uses the driver_button library to detect button press and release events on BUTTON_PIN.
* It prints the button state (pressed or released) to the serial monitor.
* 
* Author: [Zhentao Lin]
* Date:   [2025-04-07]
*/

#include "driver_button.h"

#define BUTTON_PIN 19  // GPIO pin connected to the button
Button button(BUTTON_PIN);

void setup() {
   // Initialize serial communication at 115200 bits per second
   Serial.begin(115200);
   // Initialize the button
   button.init();
}

void loop() {
   // Scan for button events
   button.key_scan();
   // Handle button events
   handleButtonEvents();
   // Delay to prevent excessive CPU usage
   delay(50);
}

void handleButtonEvents() {
   // Get the current state of the button
   int buttonState = button.get_button_state();
   // Get the key value of the button
   int buttonKeyValue = button.get_button_key_value();
   // Handle different button states
   switch (buttonState) {
       case Button::KEY_STATE_PRESSED:
           Serial.println(String(buttonKeyValue) + " Pressed.");
           break;
       case Button::KEY_STATE_RELEASED:
           Serial.println(String(buttonKeyValue) + " Released.");
           break;
       default:
           break;
   }
}