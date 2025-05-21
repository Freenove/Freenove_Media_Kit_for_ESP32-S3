#ifndef __MUSIC_H
#define __MUSIC_H

#include "lvgl.h"
#include "Arduino.h"
#include "stdlib.h"
#include "string.h"

#include "display.h"
#include "driver_sdmmc.h"
#include "driver_audio_output.h"

#define MUSIC_FOLDER     "/music"

typedef struct lvgl_music
{
	lv_obj_t *music;
	lv_obj_t *music_btn_left;
  lv_obj_t *music_btn_left_label;
	lv_obj_t *music_btn_play;
  lv_obj_t *music_btn_play_label;
  lv_obj_t *music_btn_stop;
  lv_obj_t *music_btn_stop_label;
	lv_obj_t *music_btn_right;
  lv_obj_t *music_btn_right_label;
	lv_obj_t *music_mp3_label;
  lv_obj_t *music_slider_valume;
  lv_obj_t *music_slider_label;
  lv_obj_t *music_bar_time;
}lvgl_music_ui;

extern lvgl_music_ui guider_music_ui;//music ui structure 
void setup_scr_music(lvgl_music_ui *ui);//Parameter configuration function on the music screen

void music_set_label_text(const char *text);     //Set the label display content
void loopTask_music(void *pvParameters);   //music player task thread
void start_music_task(void);               //Create music task thread
void stop_music_task(void);                //Close the music thread
int music_task_is_running(void);           //Check whether the thread is running

#endif





