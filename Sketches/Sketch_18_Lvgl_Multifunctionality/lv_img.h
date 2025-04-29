#ifndef __LV_IMG_H
#define __LV_IMG_H

#define SELECT_IMG_SIZE     (80)
#define SELECT_IMG_CAMERA   SELECT_IMG_SIZE
#define SELECT_IMG_MUSIC    SELECT_IMG_SIZE
#define SELECT_IMG_PICTURE  SELECT_IMG_SIZE
#define SELECT_IMG_RECORDER SELECT_IMG_SIZE
#define SELECT_IMG_WS2812   SELECT_IMG_SIZE

extern lv_img_dsc_t img_camera;
extern lv_img_dsc_t img_music;
extern lv_img_dsc_t img_picture;
extern lv_img_dsc_t img_recorder;
extern lv_img_dsc_t img_ws2812;

void lv_img_camera_init(void);
void lv_img_music_init(void);
void lv_img_picture_init(void);
void lv_img_recorder_init(void);
void lv_img_ws2812_init(void);


#endif



