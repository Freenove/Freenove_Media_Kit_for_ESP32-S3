#include "picture_ui.h"

lvgl_picture_ui guider_picture_ui;  // picture ui structure
static int picture_index = 0;

static void picture_btn_left_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    Serial.printf("picture_btn_left_event_handler: %d\r\n", c);
    if (c == LV_KEY_ENTER) {
      picture_index--;
      if (picture_index < 0){
        int sd_picture_count = read_file_num(PICTURE_FOLDER);
        picture_index = sd_picture_count - 1;
      }
      String file_name = get_file_name_by_index(PICTURE_FOLDER, picture_index);
      picture_display(file_name.c_str());
    }
  }
}

static void picture_btn_right_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    Serial.printf("picture_btn_right_event_handler: %d\r\n", c);
    if (c == LV_KEY_ENTER) {
      picture_index++;
      int sd_picture_count = read_file_num(PICTURE_FOLDER);
      if (picture_index >= sd_picture_count){
        picture_index = 0;
      }
      String file_name = get_file_name_by_index(PICTURE_FOLDER, picture_index);
      picture_display(file_name.c_str());
    }
  }
}

// Gesture event handler
static void picture_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY)
  {
      uint32_t key = lv_event_get_key(event);
      if (key == LV_KEY_ENTER)
      {
          Serial.println("Picture Btn Enter!"); // Debug output
      }
      else if(key == LV_KEY_LEFT || key == LV_KEY_RIGHT)
      {
        Serial.println("Jump to Main Screen!");
        if (!lv_obj_is_valid(guider_main_ui.main))
            setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_picture_ui.picture);
      }
  }
  else if (code ==LV_EVENT_RELEASED)
  {
    Serial.println("Picture Btn Released!");
  }
}

// Parameter configuration function on the picture screen
void setup_scr_picture(lvgl_picture_ui *ui) {
  // Create a scrollable container
  ui->picture = lv_obj_create(NULL);
  lv_obj_t *scr = lv_disp_get_scr_act(NULL);          // Get the current active screen width and height
  lv_coord_t screen_width = lv_obj_get_width(scr);    // Get screen width
  lv_coord_t screen_height = lv_obj_get_height(scr);  // Get screen height
  lv_obj_set_size(ui->picture, screen_width, screen_height);

  // Write codes picture_show
  ui->picture_show = lv_img_create(ui->picture);
  lv_obj_set_size(ui->picture_show, screen_width, screen_height);  // Set the actual size of the image
  lv_obj_set_pos(ui->picture_show, 0, 0);

  ui->picture_left = lv_btn_create(ui->picture);
  lv_obj_set_size(ui->picture_left, 40, 40);
  lv_obj_align(ui->picture_left, LV_ALIGN_LEFT_MID, 0, 0);
  lv_obj_t *left_btn_label = lv_label_create(ui->picture_left);
  lv_label_set_text(left_btn_label, LV_SYMBOL_LEFT);
  lv_obj_align(left_btn_label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_move_foreground(ui->picture_left);

  ui->picture_right = lv_btn_create(ui->picture);
  lv_obj_set_size(ui->picture_right, 40, 40);
  lv_obj_align(ui->picture_right, LV_ALIGN_RIGHT_MID, 0, 0);
  lv_obj_t *right_btn_label = lv_label_create(ui->picture_right);
  lv_label_set_text(right_btn_label, LV_SYMBOL_RIGHT);
  lv_obj_align(right_btn_label, LV_ALIGN_CENTER, 0, 0);
  lv_obj_move_foreground(ui->picture_right);

  // Create a group for input devices
  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->picture_left);
  lv_group_add_obj(group, ui->picture_right);
  lv_group_add_obj(group, ui->picture);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  lv_obj_add_event_cb(ui->picture_left, picture_btn_left_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->picture_right, picture_btn_right_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->picture, picture_event_handler, LV_EVENT_KEY, NULL);

  if (read_file_num(PICTURE_FOLDER) > 0) {
    String file_name = get_file_name_by_index(PICTURE_FOLDER, picture_index);
    picture_display(file_name.c_str());
  }
}

//Read the image file and display it
void picture_display(const char *name) {
  if (name != NULL) {
    char buf_picture_name[100] = { "S:" };
    strcat(buf_picture_name, PICTURE_FOLDER);
    strcat(buf_picture_name, "/");
    strcat(buf_picture_name, name);
    lv_img_set_src(guider_picture_ui.picture_show, buf_picture_name);
  } else {
    lv_img_set_src(guider_picture_ui.picture_show, LV_SYMBOL_DUMMY "The picture folder has no files.");
  }
}
