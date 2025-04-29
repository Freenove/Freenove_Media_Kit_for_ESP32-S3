/**
 * Sketch_11_LVGL.ino
 * This sketch demonstrates the use of the LVGL library to create a simple graphical user interface (GUI)
 *        on an ESP32 microcontroller. It initializes a display, creates labels and buttons, and handles button
 *        events using LVGL. The GUI includes a label displaying the LVGL version and two buttons labeled "Left"
 *        and "Right". Button events are logged via the serial monitor.
 *
 * Author: Zhentao Lin
 * Date:   2025-04-08
 */

#include "display.h"
#include <lvgl.h>

#define TFT_BL_PIN 20
Display screen;

void setup() {
  /* Prepare for possible serial debug */
  Serial.begin(115200);

  /*** Initialize screen ***/
  screen.init(TFT_DIRECTION);

  // Create a string to display the LVGL version
  String LVGL_Arduino = "Hello Arduino! ";
  LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);
  Serial.println("I am LVGL_Arduino");

  // Create a label on the active screen and set its text
  lv_obj_t *label = lv_label_create(lv_scr_act());
  lv_label_set_text(label, LVGL_Arduino.c_str());
  lv_obj_align(label, LV_ALIGN_CENTER, 0, -30);

  // Create a left button on the active screen and set its size and position
  lv_obj_t *btn_left = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn_left, 60, 30);
  lv_obj_align(btn_left, LV_ALIGN_CENTER, -50, 30);

  // Create a right button on the active screen and set its size and position
  lv_obj_t *btn_right = lv_btn_create(lv_scr_act());
  lv_obj_set_size(btn_right, 60, 30);
  lv_obj_align(btn_right, LV_ALIGN_CENTER, 50, 30);

  // Create a label for the left button and set its text
  lv_obj_t *label_btn_left = lv_label_create(btn_left);
  lv_label_set_text(label_btn_left, "Left");
  lv_obj_center(label_btn_left);

  // Create a label for the right button and set its text
  lv_obj_t *label_btn_right = lv_label_create(btn_right);
  lv_label_set_text(label_btn_right, "Right");
  lv_obj_center(label_btn_right);

  // Create a group to manage the buttons
  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, btn_left);
  lv_group_add_obj(group, btn_right);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  // Add event callbacks to the buttons
  lv_obj_add_event_cb(btn_left, btn_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(btn_right, btn_event_cb, LV_EVENT_ALL, NULL);

  Serial.println("Setup done");
}

void loop() {
  screen.routine(); /* Let the GUI do its work */
  delay(5);
}

/**
  * @brief Callback function for button events
  * @param e Pointer to the event data
  */
void btn_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(e);
    if (c == LV_KEY_ENTER) {
      Serial.printf("LV_KEY_ENTER\r\n");
    } else if (c == LV_KEY_DOWN) {
      Serial.printf("LV_KEY_DOWN\r\n");
    } else if (c == LV_KEY_UP) {
      Serial.printf("LV_KEY_UP\r\n");
    } else if (c == LV_KEY_LEFT) {
      Serial.printf("LV_KEY_LEFT\r\n");
    } else if (c == LV_KEY_RIGHT) {
      Serial.printf("LV_KEY_RIGHT\r\n");
    }
  }
  if (code == LV_EVENT_RELEASED) {
    Serial.printf("LV_EVENT_RELEASED\r\n");
  }
}