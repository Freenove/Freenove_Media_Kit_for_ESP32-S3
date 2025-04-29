#ifndef __CAMERA_UI_H
#define __CAMERA_UI_H

#include "lvgl.h"
#include "Arduino.h"
#include "esp_camera.h"
#include "driver_sdmmc.h"
#include "display.h"
#define CAMERA_MODEL_ESP32S3_EYE  // Has PSRAM
#include "camera_pins.h"

#define CAMERA_FOLDER "/picture"

typedef struct lvgl_camera
{
	lv_obj_t *camera;
	lv_obj_t *camera_video;
}lvgl_camera_ui;

extern lv_img_dsc_t photo_show;              //apply an lvgl image variable
extern lvgl_camera_ui guider_camera_ui;      //camera ui structure 

void create_camera_task(void);               //Create camera task thread
void stop_camera_task(void);                 //Close the camera thread
void loopTask_camera(void *pvParameters);    //camera task thread
void ui_set_photo_show(void);                //Initialize an lvgl image variable

void camera_init(int state);                  //Camera initialization
void setup_scr_camera(lvgl_camera_ui *ui);   //Parameter configuration function on the camera screen

#endif
