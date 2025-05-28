#ifndef __PICTURE_UI_H
#define __PICTURE_UI_H

#include "public.h"

typedef struct lvgl_picture
{
	lv_obj_t *picture;
	lv_obj_t *picture_left;
	lv_obj_t *picture_right;
	lv_obj_t *picture_show;
  lv_group_t *picture_show_group;                 // Group for managing sliders
}lvgl_picture_ui;

extern lvgl_picture_ui guider_picture_ui;    //picture ui structure 

void setup_scr_picture(lvgl_picture_ui *ui); //Parameter configuration function on the picture screen
void picture_display(const char *name);

void create_picture_task(void);               //Create picture task thread
void stop_picture_task(void);                 //Close the picture thread
void loopTask_picture(void *pvParameters);    //picture task thread






#endif
