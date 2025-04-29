#ifndef __MAIN_UI_H
#define __MAIN_UI_H

#include "public.h"

typedef struct lvgl_main {
  lv_obj_t *main;
  lv_obj_t *main_label_logo;
  lv_obj_t *main_btn_ws2812;
  lv_obj_t *main_btn_camera;
  lv_obj_t *main_btn_picture;
  lv_obj_t *main_btn_music;
  lv_obj_t *main_btn_recorder;
} lvgl_main_ui;  // main ui struct

extern lvgl_main_ui guider_main_ui;     // main ui structure
void setup_scr_main(lvgl_main_ui *ui);  // Parameter configuration function on the main screen

#endif