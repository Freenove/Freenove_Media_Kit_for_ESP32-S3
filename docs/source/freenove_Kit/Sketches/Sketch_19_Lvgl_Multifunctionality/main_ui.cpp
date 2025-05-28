
#include "main_ui.h"

lvgl_main_ui guider_main_ui;  // main ui structure

static void main_imgbtn_ws2812_event_cb(lv_event_t *event) {
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
  else if (code == LV_EVENT_FOCUSED) {
    lv_obj_scroll_to_view_recursive(lv_event_get_target(event), LV_ANIM_ON);
  }
}

static void main_imgbtn_camera_event_cb(lv_event_t *event) {
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
  else if (code == LV_EVENT_FOCUSED) {
    lv_obj_scroll_to_view_recursive(lv_event_get_target(event), LV_ANIM_ON);
  }
}

static void main_imgbtn_picture_event_cb(lv_event_t *event) {
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
  else if (code == LV_EVENT_FOCUSED) {
    lv_obj_scroll_to_view_recursive(lv_event_get_target(event), LV_ANIM_ON);
  }
}

static void main_imgbtn_music_event_cb(lv_event_t *event) {
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
  else if (code == LV_EVENT_FOCUSED) {
    lv_obj_scroll_to_view_recursive(lv_event_get_target(event), LV_ANIM_ON);
  }
}

static void main_imgbtn_recorder_event_cb(lv_event_t *event) {
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
  else if (code == LV_EVENT_FOCUSED) {
    lv_obj_scroll_to_view_recursive(lv_event_get_target(event), LV_ANIM_ON);
  }
}

// Parameter configuration function on the main screen
void setup_scr_main(lvgl_main_ui *ui) {
  // Write codes main
  ui->main = lv_obj_create(NULL);
  
  lv_obj_t *scr = lv_disp_get_scr_act(NULL);          // Get the current active screen width and height
  lv_coord_t screen_width = lv_obj_get_width(scr);    // Get screen width
  lv_coord_t screen_height = lv_obj_get_height(scr);  // Get screen height

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->main, &bg_style, LV_PART_MAIN);

  static lv_style_t focused_style;
  lv_style_init(&focused_style);
  lv_style_set_border_color(&focused_style, lv_color_hex(0x0000ff)); 
  lv_style_set_border_opa(&focused_style, LV_OPA_COVER);
  lv_style_set_border_width(&focused_style, 2);

  static lv_style_t pressed_style;              
  lv_style_init(&pressed_style);               
  lv_style_set_translate_y(&pressed_style, 5);  
  
  ui->main_label_logo = lv_label_create(ui->main);
  lv_obj_set_size(ui->main_label_logo, 100, 15);
  lv_obj_set_style_text_align(ui->main_label_logo, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text(ui->main_label_logo, "Freenove");
  lv_obj_align(ui->main_label_logo, LV_ALIGN_TOP_MID, 0, 10);

  ui->main_panel = lv_obj_create(ui->main);
  //lv_obj_remove_style_all(ui->main_panel);
  lv_obj_set_size(ui->main_panel, (screen_width-10), (SELECT_IMG_SIZE+20));
  lv_obj_set_scroll_snap_x(ui->main_panel, LV_SCROLL_SNAP_CENTER);
  lv_obj_set_flex_flow(ui->main_panel, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_pad_column(ui->main_panel, (SELECT_IMG_SIZE/4), LV_PART_MAIN);  
  lv_obj_set_scrollbar_mode(ui->main_panel, LV_SCROLLBAR_MODE_OFF);
  lv_obj_align_to(ui->main_panel, ui->main_label_logo, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

  lv_img_camera_init();
  lv_img_music_init();
  lv_img_picture_init();
  lv_img_recorder_init();
  lv_img_ws2812_init();

  ui->main_imgbtn_ws2812 = lv_imgbtn_create(ui->main_panel);
  lv_obj_remove_style_all(ui->main_imgbtn_ws2812);
  lv_obj_set_size(ui->main_imgbtn_ws2812, SELECT_IMG_SIZE, SELECT_IMG_SIZE);
  lv_img_set_src(ui->main_imgbtn_ws2812, &img_ws2812);
  lv_obj_add_style(ui->main_imgbtn_ws2812, &pressed_style, LV_STATE_PRESSED | LV_PART_MAIN);
  lv_obj_add_style(ui->main_imgbtn_ws2812, &focused_style, LV_STATE_FOCUSED | LV_PART_MAIN);
  
  ui->main_imgbtn_camera = lv_imgbtn_create(ui->main_panel);
  lv_obj_remove_style_all(ui->main_imgbtn_camera);
  lv_obj_set_size(ui->main_imgbtn_camera, SELECT_IMG_SIZE, SELECT_IMG_SIZE);
  lv_img_set_src(ui->main_imgbtn_camera, &img_camera);
  lv_obj_add_style(ui->main_imgbtn_camera, &pressed_style, LV_STATE_PRESSED | LV_PART_MAIN);
  lv_obj_add_style(ui->main_imgbtn_camera, &focused_style, LV_STATE_FOCUSED | LV_PART_MAIN);
  
  ui->main_imgbtn_picture = lv_imgbtn_create(ui->main_panel);
  lv_obj_remove_style_all(ui->main_imgbtn_picture);
  lv_obj_set_size(ui->main_imgbtn_picture, SELECT_IMG_SIZE, SELECT_IMG_SIZE);
  lv_img_set_src(ui->main_imgbtn_picture, &img_picture);
  lv_obj_add_style(ui->main_imgbtn_picture, &pressed_style, LV_STATE_PRESSED | LV_PART_MAIN);
  lv_obj_add_style(ui->main_imgbtn_picture, &focused_style, LV_STATE_FOCUSED | LV_PART_MAIN);

  ui->main_imgbtn_music = lv_imgbtn_create(ui->main_panel);
  lv_obj_remove_style_all(ui->main_imgbtn_music);
  lv_obj_set_size(ui->main_imgbtn_music, SELECT_IMG_SIZE, SELECT_IMG_SIZE);
  lv_img_set_src(ui->main_imgbtn_music, &img_music);
  lv_obj_add_style(ui->main_imgbtn_music, &pressed_style, LV_STATE_PRESSED | LV_PART_MAIN);
  lv_obj_add_style(ui->main_imgbtn_music, &focused_style, LV_STATE_FOCUSED | LV_PART_MAIN);

  ui->main_imgbtn_recorder = lv_imgbtn_create(ui->main_panel);
  lv_obj_remove_style_all(ui->main_imgbtn_recorder);
  lv_obj_set_size(ui->main_imgbtn_recorder, SELECT_IMG_SIZE, SELECT_IMG_SIZE);
  lv_img_set_src(ui->main_imgbtn_recorder, &img_recorder);
  lv_obj_add_style(ui->main_imgbtn_recorder, &pressed_style, LV_STATE_PRESSED | LV_PART_MAIN);
  lv_obj_add_style(ui->main_imgbtn_recorder, &focused_style, LV_STATE_FOCUSED | LV_PART_MAIN);

  lv_obj_update_snap(ui->main_panel, LV_ANIM_ON);
  lv_obj_scroll_to_view_recursive(ui->main_imgbtn_ws2812, LV_ANIM_ON);

  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->main_imgbtn_ws2812);
  lv_group_add_obj(group, ui->main_imgbtn_camera);
  lv_group_add_obj(group, ui->main_imgbtn_picture);
  lv_group_add_obj(group, ui->main_imgbtn_music);
  lv_group_add_obj(group, ui->main_imgbtn_recorder);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  lv_obj_add_event_cb(ui->main_imgbtn_ws2812, main_imgbtn_ws2812_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_camera, main_imgbtn_camera_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_picture, main_imgbtn_picture_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_music, main_imgbtn_music_event_cb, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->main_imgbtn_recorder, main_imgbtn_recorder_event_cb, LV_EVENT_ALL, NULL);
}
