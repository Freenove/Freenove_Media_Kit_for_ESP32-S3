#include "driver_button.h"

Button::Button(int pin) : pin(pin) {
  // Initialize variables
  thresholdRange = 0;
  pinState = KEY_STATE_IDLE;
  lastPinState = KEY_STATE_IDLE;
  thisTimeButtonKeyValue = Volt_330;
  btnVolt = Volt_330;
}

void Button::init() {
  int thresholds[] = { 2800, 0, 700, 2000, 1350, 2660 };
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  set_voltage_thresholds(thresholds);
  set_threshold_range(50);
}

void Button::set_voltage_thresholds(const int thresholds[6]) {
  for (int i = 0; i < 6; i++) {
    voltageThresholds[i] = thresholds[i];
  }
}

void Button::set_threshold_range(int range) {
  thresholdRange = range;
}

void Button::key_scan() {
  int analogVolt = analogReadMilliVolts(pin);

  // Determine button state based on voltage value
  btnVolt = Volt_330;
  for (int i = 0; i < 6; i++) {
    if (analogVolt >= voltageThresholds[i] - thresholdRange && analogVolt <= voltageThresholds[i] + thresholdRange) {
      btnVolt = static_cast<ButtonVolt>(i);
      break;
    }
  }

  if (lastPinState != pinState && pinState != KEY_STATE_IDLE) {
    lastPinState = pinState;
  }

  switch (pinState) {
    case KEY_STATE_IDLE:
      if (btnVolt != Volt_330) {
        buttonTriggerTiming = millis();
        pinState = KEY_STATE_PRESSED_BOUNCE_TIME;
        thisTimeButtonKeyValue = btnVolt;
      }
      break;
    case KEY_STATE_PRESSED_BOUNCE_TIME:
      if (thisTimeButtonKeyValue == btnVolt) {
        if (millis() - buttonTriggerTiming > DEBOUNCE_TIME) {
          pinState = KEY_STATE_PRESSED;
        }
      } else {
        pinState = KEY_STATE_IDLE;
      }
      break;
    case KEY_STATE_PRESSED:
      if (thisTimeButtonKeyValue != btnVolt) {
        buttonFirstRelesseTiming = millis();
        pinState = KEY_STATE_RELEASE_BOUNCE_TIME;
      }
      break;
    case KEY_STATE_RELEASE_BOUNCE_TIME:
      if (thisTimeButtonKeyValue != btnVolt) {
        if (millis() - buttonFirstRelesseTiming > DEBOUNCE_TIME) {
          pinState = KEY_STATE_RELEASED;
        }
      }
      break;
    case KEY_STATE_RELEASED:
      pinState = KEY_STATE_IDLE;
      break;
  }
}

int Button::get_button_key_value() {
  return static_cast<int>(thisTimeButtonKeyValue);
}

int Button::get_button_state() {
  return static_cast<int>(pinState);
}