/*
* Sketch_04_Simple_Tone.ino
* This sketch generates simple tones using the ESP_I2S library for I2S audio output.
* It plays a sequence of musical notes in a loop.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include <ESP_I2S.h>  // Include ESP_I2S library for I2S audio output

#define I2S_BCLK 42
#define I2S_DOUT 1
#define I2S_LRC 41

// Define note frequencies in Hz for "Do Re Mi Fa So La Ti"
const int frequencies[] = { 261, 293, 329, 349, 392, 440, 493 };
const int amplitude = 500;    // Amplitude of the square wave, controls volume
const int sampleRate = 8000;  // Sampling rate, number of samples per second in Hz

// I2S configuration parameters
i2s_data_bit_width_t bps = I2S_DATA_BIT_WIDTH_16BIT;  // 16 bits per sample
i2s_mode_t mode = I2S_MODE_STD;                       // Use standard I2S mode
i2s_slot_mode_t slot = I2S_SLOT_MODE_STEREO;          // Use stereo mode (left and right channels)

int currentNote = 0;  // Index of the current note being played, starts at 0 (Do)
int halfWavelength;   // Half wavelength of the square wave, controls the flipping point

int32_t sample = amplitude;  // Current sample value, initialized to amplitude
int count = 0;               // Sample counter, tracks progress of the current note

I2SClass i2s;  // Create I2S object for controlling I2S output

void setup() {
  Serial.begin(115200);                      // Initialize serial communication at 115200 baud rate
  Serial.println("I2S simple tone");         // Print debug message
  i2s.setPins(I2S_BCLK, I2S_LRC, I2S_DOUT);  // Set I2S pins (BCLK, LRCK, DATA)

  // Initialize I2S
  if (!i2s.begin(mode, sampleRate, bps, slot)) {
    Serial.println("Failed to initialize I2S!");  // Print error message if initialization fails
    while (1)
      ;  // Stop program execution
  }

  // Calculate initial half wavelength for generating the square wave
  halfWavelength = (sampleRate / frequencies[currentNote]);
}

void loop() {
  // If the sample counter reaches half wavelength, flip the sample value to generate a square wave
  if (count % halfWavelength == 0) {
    sample = -1 * sample;
  }

  // Write the current sample value to I2S, output to right and left channels
  i2s.write(sample);  // Right channel
  i2s.write(sample);  // Left channel

  // Increment the sample counter
  count++;

  // If the current note finishes playing (1 second), switch to the next note
  if (count >= sampleRate) {  // sampleRate represents the number of samples in 1 second
    count = 0;                // Reset the sample counter
    currentNote++;            // Switch to the next note

    // If all notes have been played (currentNote >= 7), pause for 1 second and reset the note index
    if (currentNote >= 7) {
      delay(1000);      // Pause for 1 second
      currentNote = 0;  // Reset the note index to start over
    }

    // Update the half wavelength to match the new note frequency
    halfWavelength = (sampleRate / frequencies[currentNote]);
  }
}