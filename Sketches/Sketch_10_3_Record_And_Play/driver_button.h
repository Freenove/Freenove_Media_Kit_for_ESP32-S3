#ifndef __BUTTON_H
#define __BUTTON_H

#include <Arduino.h>

class Button {
public:
  Button(int pin);
  void init();
  void key_scan();
  void set_voltage_thresholds(const int thresholds[6]);
  void set_threshold_range(int range);
  int get_button_key_value();
  int get_button_state();

  typedef enum {
    KEY_STATE_IDLE,
    KEY_STATE_PRESSED_BOUNCE_TIME,
    KEY_STATE_PRESSED,
    KEY_STATE_RELEASE_BOUNCE_TIME,
    KEY_STATE_RELEASED
  } ButtonState;

  typedef enum {
    Volt_330,
    Volt_000,
    Volt_066,
    Volt_132,
    Volt_198,
    Volt_264
  } ButtonVolt;

  const static int DEBOUNCE_TIME = 50;

private:
  int pin;
  ButtonVolt thisTimeButtonKeyValue;
  ButtonVolt btnVolt;
  ButtonState pinState;
  ButtonState lastPinState;

  uint32_t buttonTriggerTiming;
  uint32_t buttonFirstRelesseTiming;
  int voltageThresholds[6];
  int thresholdRange;
};

#endif