/*
* Sketch_02_1_Button_Value.ino
* This sketch reads the analog value from a button connected to BUTTON_PIN and prints it to the serial monitor.
* The ADC resolution is set to 12 bits (0-4095) with an attenuation of 11 dB.
* 
* Author: [Zhentao Lin]
* Date:   [2025-04-07]
*/

#define BUTTON_PIN 19  // GPIO pin connected to the button

void setup() {
  // Initialize serial communication at 115200 bits per second
  Serial.begin(115200);

  // Set the ADC resolution to 12 bits (0-4095)
  analogReadResolution(12);
  // Set the ADC attenuation to 11 dB
  analogSetAttenuation(ADC_11db);
}

void loop() {
  // Read the analog/millivolts value for pin 20
  int analogValue = analogReadMilliVolts(BUTTON_PIN);
  // Print the ADC value to the serial monitor
  Serial.printf("ADC value = %d\n", analogValue);

  // Delay in between reads for clear read from serial
  delay(100);
}