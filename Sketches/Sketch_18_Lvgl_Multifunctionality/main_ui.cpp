
#include "main_ui.h"

lvgl_main_ui guider_main_ui;  // main ui structure

static void main_btn_ws2812_event_cb(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("WS2812 Btn Enter!");  // Debug output
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Jumping to ws2812 screen!");
    if (!lv_obj_is_valid(guider_ws2812_ui.ws2812))
      setup_scr_ws2812(&guider_ws2812_ui);
    lv_scr_load(guider_ws2812_ui.ws2812);
    lv_obj_del(guider_main_ui.main);
  }
}

static void main_btn_camera_event_cb(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Camera Btn Enter!");  // Debug output
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Jump to Camera Screen!");
    if (!lv_obj_is_valid(guider_camera_ui.camera))
      setup_scr_camera(&guider_camera_ui);
    lv_scr_load(guider_camera_ui.camera);
    lv_obj_del(guider_main_ui.main);
  }
}

static void main_btn_picture_event_cb(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Picture Btn Enter!");  // Debug output
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Jump to Picture Screen!");
    if (!lv_obj_is_valid(guider_picture_ui.picture))
      setup_scr_picture(&guider_picture_ui);
    lv_scr_load(guider_picture_ui.picture);
    lv_obj_del(guider_main_ui.main);
  }
}

static void main_btn_music_event_cb(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Music Btn Enter!");  // Debug output
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Jump to Music Screen!");
    if (!lv_obj_is_valid(guider_music_ui.music))
      setup_scr_music(&guider_music_ui);
    lv_scr_load(guider_music_ui.music);
    lv_obj_del(guider_main_ui.main);
  }
}

static void main_btn_recorder_event_cb(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Recorder Btn Enter!");  // Debug output
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Recorder Btn Release!");
      if (!lv_obj_is_valid(guider_recorder_ui.recorder))
        setup_scr_sound_recorder(&guider_recorder_ui);
      lv_scr_load(guider_recorder_ui.recorder);
      lv_obj_del(guider_main_ui.main);
  }
}

// Parameter configuration function on the main screen
void setup_scr_main(lvgl_main_ui *ui) {
  // Write codes main
  ui->main = lv_obj_create(NULL);
  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->main, &bg_style, LV_PART_MAIN);

  ui->main_label_logo = lv_label_create(ui->main);
  lv_obj_set_size(ui->main_label_logo, 100, 20);
  lv_obj_set_style_text_align(ui->main_label_logo, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text(ui->main_label_logo, "Freenove");
  // lv_obj_set_style_text_font(ui->main_label_logo, &lv_font_montserrat_20, 0);
  lv_obj_align(ui->main_label_logo, LV_ALIGN_TOP_MID, 0, 10);

  ui->main_btn_ws2812 = lv_btn_create(ui->main);
  lv_obj_set_size(ui->main_btn_ws2812, 70, 30);
  lv_obj_align(ui->main_btn_ws2812, LV_ALIGN_LEFT_MID, 10, -10);
  lv_obj_t *label_ws2812 = lv_label_create(ui->main_btn_ws2812);
  lv_label_set_text(label_ws2812, "WS2812");
  lv_obj_align(label_ws2812, LV_ALIGN_CENTER, 0, 0);

  ui->main_btn_camera = lv_btn_create(ui->main);
  lv_obj_set_size(ui->main_btn_camera, 70, 30);
  lv_obj_align_to(ui->main_btn_camera, ui->main_btn_ws2812, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_t *label_camera = lv_label_create(ui->main_btn_camera);
  lv_label_set_text(label_camera, "Camera");
  lv_obj_align(label_camera, LV_ALIGN_CENTER, 0, 0);

  ui->main_btn_picture = lv_btn_create(ui->main);
  lv_obj_set_size(ui->main_btn_picture, 70, 30);
  lv_obj_align_to(ui->main_btn_picture, ui->main_btn_camera, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_t *label_picture = lv_label_create(ui->main_btn_picture);
  lv_label_set_text(label_picture, "Picture");
  lv_obj_align(label_picture, LV_ALIGN_CENTER, 0, 0);

  ui->main_btn_music = lv_btn_create(ui->main);
  lv_obj_set_size(ui->main_btn_music, 70, 30);
  lv_obj_align_to(ui->main_btn_music, ui->main_btn_ws2812, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
  lv_obj_t *label_music = lv_label_create(ui->main_btn_music);
  lv_label_set_text(label_music, "Music");
  lv_obj_align(label_music, LV_ALIGN_CENTER, 0, 0);

  ui->main_btn_recorder = lv_btn_create(ui->main);
  lv_obj_set_size(ui->main_btn_recorder, 70, 30);
  lv_obj_align_to(ui->main_btn_recorder, ui->main_btn_music, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
  lv_obj_t *label_recorder = lv_label_create(ui->main_btn_recorder);
  lv_label_set_text(label_recorder, "Recorder");
  lv_obj_align(label_recorder, LV_ALIGN_CENTER, 0, 0);

  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->main_btn_ws2812);
  lv_group_add_obj(group, ui->main_btn_camera);
  lv_group_add_obj(group, ui->main_btn_picture);
  lv_group_add_obj(group, ui->main_btn_music);
  lv_group_add_obj(group, ui->main_btn_recorder);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  lv_obj_add_event_cb(ui->main_btn_ws2812, main_btn_ws2812_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_btn_camera, main_btn_camera_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_btn_picture, main_btn_picture_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_btn_music, main_btn_music_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_btn_recorder, main_btn_recorder_event_cb, LV_EVENT_ALL, NULL);
}
