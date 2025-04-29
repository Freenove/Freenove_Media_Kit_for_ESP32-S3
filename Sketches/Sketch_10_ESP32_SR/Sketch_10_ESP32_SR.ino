/**
 * Sketch_10_ESP32_SR.ino
 * This sketch uses the ESP_SR library to recognize specific voice commands
 *        and control an LED light on an ESP32 microcontroller. The voice commands
 *        are processed using the I2S bus for audio input. The recognized commands
 *        include "Turn on the light," "Switch on the light," "Turn off the light,"
 *        "Switch off the light," and "Go dark."
 *
 * Author: Zhentao Lin
 * Date:   2025-04-08
 */

#include "ESP_I2S.h"
#include "ESP_SR.h"

// Define I2S pins for audio input
#define I2S_PIN_BCK 3
#define I2S_PIN_WS 14
#define I2S_PIN_DIN 46

// Define pin for the light
#define LIGHT_PIN 2

// Create an I2S object
I2SClass i2s;

// Generated using the following command:
// python3 tools/gen_sr_commands.py "Turn on the light,Switch on the light;Turn off the light,Switch off the light,Go dark;Start fan;Stop fan"
enum {
  SR_CMD_TURN_ON_THE_LIGHT,
  SR_CMD_TURN_OFF_THE_LIGHT,
};
static const sr_cmd_t sr_commands[] = {
  { 0, "Turn on the light", "TkN nN jc LiT" },      // Command ID 0: Turn on the light
  { 0, "Switch on the light", "SWgp nN jc LiT" },   // Command ID 0: Switch on the light
  { 1, "Turn off the light", "TkN eF jc LiT" },     // Command ID 1: Turn off the light
  { 1, "Switch off the light", "SWgp eF jc LiT" },  // Command ID 1: Switch off the light
  { 1, "Go dark", "Gb DnRK" },                      // Command ID 1: Go dark
};

/**
 * @brief Callback function for voice recognition events
 * @param event Type of event (e.g., wakeword detected, command recognized)
 * @param command_id ID of the recognized command
 * @param phrase_id ID of the recognized phrase within the command
 */
void onSrEvent(sr_event_t event, int command_id, int phrase_id) {
  switch (event) {
    case SR_EVENT_WAKEWORD:
      Serial.println("WakeWord Detected!");  // Wakeword detected
      break;
    case SR_EVENT_WAKEWORD_CHANNEL:
      Serial.printf("WakeWord Channel %d Verified!\n", command_id);  // Specific wakeword channel verified
      ESP_SR.setMode(SR_MODE_COMMAND);                               // Switch to command detection mode
      break;
    case SR_EVENT_TIMEOUT:
      Serial.println("Timeout Detected!");  // Timeout occurred
      ESP_SR.setMode(SR_MODE_WAKEWORD);     // Switch back to wakeword detection mode
      break;
    case SR_EVENT_COMMAND:
      Serial.printf("Command %d Detected! %s\n", command_id, sr_commands[phrase_id].str);  // Command recognized
      switch (command_id) {
        case SR_CMD_TURN_ON_THE_LIGHT:
          digitalWrite(LIGHT_PIN, HIGH);  // Turn on the light
          break;
        case SR_CMD_TURN_OFF_THE_LIGHT:
          digitalWrite(LIGHT_PIN, LOW);  // Turn off the light
          break;
        default:
          Serial.println("Unknown Command!");  // Unknown command received
          break;
      }
      ESP_SR.setMode(SR_MODE_COMMAND);  // Allow for more commands to be given before timeout
      break;
    default:
      Serial.println("Unknown Event!");  // Unknown event received
      break;
  }
}

/**
 * @brief Setup function to initialize hardware and libraries
 */
void setup() {
  Serial.begin(115200);  // Initialize serial communication for debugging

  pinMode(LIGHT_PIN, OUTPUT);    // Set light pin as output
  digitalWrite(LIGHT_PIN, LOW);  // Ensure light is off initially

  // Configure I2S for audio input
  i2s.setPins(I2S_PIN_BCK, I2S_PIN_WS, -1, I2S_PIN_DIN);
  i2s.setTimeout(1000);
  i2s.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_STEREO);

  // Initialize voice recognition library with commands and event callback
  ESP_SR.onEvent(onSrEvent);
  ESP_SR.begin(i2s, sr_commands, sizeof(sr_commands) / sizeof(sr_cmd_t), SR_CHANNELS_STEREO, SR_MODE_WAKEWORD);
}

/**
 * @brief Main loop function (empty in this sketch)
 */
void loop() {}