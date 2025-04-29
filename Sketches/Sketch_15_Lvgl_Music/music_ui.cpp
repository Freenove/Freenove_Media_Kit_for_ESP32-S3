#include "music_ui.h"

lvgl_music_ui guider_music_ui;//music ui structure 
int music_button_state = 0;   //UI Button status
int music_index_num = 0;      //index number of the music

int music_task_flag = 0;       //music thread running flag
TaskHandle_t musicTaskHandle;  //music thread task handle

//Click the left icon, callback function: play the last song
static void music_btn_left_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    if (c == LV_KEY_ENTER){
      Serial.println("Play the last song.");
      music_index_num--;
      if (music_index_num < 0)
        music_index_num = read_file_num(MUSIC_FOLDER)-1;
      stop_music_task();
      lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PLAY);
      music_button_state = 0;
      String music_name = get_file_name_by_index(MUSIC_FOLDER, music_index_num);
      music_set_label_text(music_name.c_str());
      if(music_name!=""){
        char buf_music_path[255] = {MUSIC_FOLDER};
        strcat(buf_music_path, "/");
        strcat(buf_music_path, music_name.c_str());
        Serial.println(buf_music_path);
        audio_output_load_music(buf_music_path);
        start_music_task();
        lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PAUSE);
        music_button_state = 1;
      }
    }
  }
}

//Click the right icon, callback function: play the next song
static void music_btn_right_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    if (c == LV_KEY_ENTER){
      Serial.println("Play the next song.");
      music_index_num++;
      if (music_index_num >= read_file_num(MUSIC_FOLDER))
        music_index_num = 0;

      stop_music_task();
      lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PLAY);
      music_button_state = 0;
      String music_name = get_file_name_by_index(MUSIC_FOLDER, music_index_num);
      music_set_label_text(music_name.c_str());
      if(music_name!="")
      {
        char buf_music_path[255] = {MUSIC_FOLDER};
        strcat(buf_music_path, "/");
        strcat(buf_music_path, music_name.c_str());
        Serial.println(buf_music_path);
        audio_output_load_music(buf_music_path);
        start_music_task();
        lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PAUSE);
        music_button_state = 1;
      }
    }
  }
}

//Click the play icon, callback function: play or pause the music
static void music_btn_play_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    if (c == LV_KEY_ENTER){
        music_button_state = !music_button_state;
        if (music_button_state == 1) 
          lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PAUSE);
        else 
          lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PLAY);

        //Gets whether a music task currently exists
        int is_task_running = music_task_is_running();
        if (is_task_running == 1) {  //If so, pause or play it
          audio_output_pause_resume();
        } else {                     
          /*If there is no music thread currently, load the music name first, and then create the audio thread*/
          String music_name = get_file_name_by_index(MUSIC_FOLDER, music_index_num);
          music_set_label_text(music_name.c_str());
          if(music_name!="")
          {
            char buf_music_path[255] = {MUSIC_FOLDER};
            strcat(buf_music_path, "/");
            strcat(buf_music_path, music_name.c_str());
            Serial.println(buf_music_path);
            audio_output_load_music(buf_music_path);
            start_music_task();
          }
        }
    }
  }
}

//Click the stop icon, callback function: stop the music
static void music_btn_stop_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    if (c == LV_KEY_ENTER){
        lv_label_set_text(guider_music_ui.music_btn_play_label, LV_SYMBOL_PLAY);
        stop_music_task();
        lv_bar_set_value(guider_music_ui.music_bar_time, 0, LV_ANIM_OFF);
        music_button_state = 0;
        Serial.println("The music has stop.");
      }
    }
}

static void music_slider_change_event_handler(lv_event_t * event){
    lv_obj_t *slider = lv_event_get_target(event);
    char buf[16];
    int volume = (int)lv_slider_get_value(slider);
    lv_snprintf(buf, sizeof(buf), "Volume:%d", volume);
    lv_label_set_text(guider_music_ui.music_slider_label, buf);
    int last_volume = audio_read_output_volume();
    if(volume != last_volume)
      audio_output_set_volume(volume);
}

static void screen_music_event_handler(lv_event_t * event)  
{
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY)
  {
      uint32_t key = lv_event_get_key(event);
      if (key == LV_KEY_ENTER)
      {
          Serial.println("Music Btn Enter!"); // Debug output
      }
      else if(key == LV_KEY_LEFT || key == LV_KEY_RIGHT)
      {
        /*
        stop_music_task();
        Serial.println("Jump to Main Screen!");
        if (!lv_obj_is_valid(guider_main_ui.main))
            setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_music_ui.music);
        */
      }
  }
  else if (code ==LV_EVENT_RELEASED)
  {
    Serial.println("Music Btn Released!");
  }
}

//Parameter configuration function on the music screen
void setup_scr_music(lvgl_music_ui *ui) {
  ui->music = lv_obj_create(NULL);
  lv_obj_t *scr = lv_disp_get_scr_act(NULL);          // Get the current active screen width and height
  lv_coord_t screen_width = lv_obj_get_width(scr);    // Get screen width
  lv_coord_t screen_height = lv_obj_get_height(scr);  // Get screen height
  lv_obj_set_size(ui->music, screen_width, screen_height);
  
  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->music, &bg_style, LV_PART_MAIN);  
  
  /*Init the pressed style*/
  static lv_style_t style_pr;//Apply for a style
  lv_style_init(&style_pr);  //Initialize it
  lv_style_set_translate_y(&style_pr, 5);//Style: Every time you trigger, move down 5 pixels

  ui->music_slider_label = lv_label_create(ui->music);
  lv_obj_set_size(ui->music_slider_label, screen_width, 20);
  lv_obj_set_pos(ui->music_slider_label, 0, 5);
  lv_obj_set_style_text_align(ui->music_slider_label, LV_TEXT_ALIGN_CENTER, 0);
  char buf[16];
  lv_snprintf(buf, sizeof(buf), "Volume:%d", 10);
  lv_label_set_text(ui->music_slider_label, buf);

  ui->music_slider_valume = lv_slider_create(ui->music);
  lv_obj_set_size(ui->music_slider_valume, screen_width, 10);
  lv_obj_align_to(ui->music_slider_valume, ui->music_slider_label, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_slider_set_mode(ui->music_slider_valume, LV_SLIDER_MODE_NORMAL);
  lv_slider_set_range(ui->music_slider_valume, 0, 21);
  lv_slider_set_value(ui->music_slider_valume, 10, LV_ANIM_OFF);

  ui->music_mp3_label = lv_label_create(ui->music);
  lv_obj_set_size(ui->music_mp3_label, screen_width, 20);
  lv_obj_align_to(ui->music_mp3_label, ui->music_slider_valume, LV_ALIGN_OUT_BOTTOM_MID, 0, 10);
  lv_label_set_long_mode(ui->music_mp3_label, LV_LABEL_LONG_SCROLL_CIRCULAR );
  lv_obj_set_style_text_align(ui->music_mp3_label, LV_TEXT_ALIGN_CENTER, 0);

  int btn_size = 35;
  int spacing_distance = (screen_width - (4 * btn_size)) / 5;
  ui->music_btn_left = lv_btn_create(ui->music);
  lv_obj_set_size(ui->music_btn_left, btn_size, btn_size);
  lv_obj_align_to(ui->music_btn_left, ui->music_mp3_label, LV_ALIGN_OUT_BOTTOM_LEFT, spacing_distance, 10);
  lv_obj_add_style(ui->music_btn_left, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed
  ui->music_btn_left_label = lv_label_create(ui->music_btn_left);
  lv_obj_set_style_text_align(ui->music_btn_left_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text(ui->music_btn_left_label, LV_SYMBOL_PREV);

  ui->music_btn_play = lv_btn_create(ui->music);
  lv_obj_set_size(ui->music_btn_play, btn_size, btn_size);
  lv_obj_align_to(ui->music_btn_play, ui->music_btn_left, LV_ALIGN_OUT_RIGHT_MID, spacing_distance, 0);
  lv_obj_add_style(ui->music_btn_play, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed
  ui->music_btn_play_label = lv_label_create(ui->music_btn_play);
  lv_obj_set_style_text_align(ui->music_btn_play_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text(ui->music_btn_play_label, LV_SYMBOL_PLAY);

  ui->music_btn_stop = lv_btn_create(ui->music);
  lv_obj_set_size(ui->music_btn_stop, btn_size, btn_size);
  lv_obj_align_to(ui->music_btn_stop, ui->music_btn_play, LV_ALIGN_OUT_RIGHT_MID, spacing_distance, 0);
  lv_obj_add_style(ui->music_btn_stop, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed
  ui->music_btn_stop_label = lv_label_create(ui->music_btn_stop);
  lv_obj_set_style_text_align(ui->music_btn_stop_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text(ui->music_btn_stop_label, LV_SYMBOL_STOP);

  ui->music_btn_right = lv_btn_create(ui->music);
  lv_obj_set_size(ui->music_btn_right, btn_size, btn_size);
  lv_obj_align_to(ui->music_btn_right, ui->music_btn_stop, LV_ALIGN_OUT_RIGHT_MID, spacing_distance, 0);
  lv_obj_add_style(ui->music_btn_right, &style_pr, LV_STATE_PRESSED);//Triggered when the button is pressed
  ui->music_btn_right_label = lv_label_create(ui->music_btn_right);
  lv_obj_set_style_text_align(ui->music_btn_right_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_label_set_text(ui->music_btn_right_label, LV_SYMBOL_NEXT);

  ui->music_bar_time = lv_bar_create(ui->music);
  lv_obj_set_size(ui->music_bar_time, screen_width, 5);
  lv_obj_set_pos(ui->music_bar_time, 0, (screen_height-10));
  lv_slider_set_mode(ui->music_bar_time, LV_SLIDER_MODE_NORMAL);
  lv_bar_set_range(ui->music_bar_time, 0, 100);
  lv_bar_set_value(ui->music_bar_time, 0, LV_ANIM_OFF);

  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->music_slider_valume);
  lv_group_add_obj(group, ui->music_btn_left);
  lv_group_add_obj(group, ui->music_btn_play);
  lv_group_add_obj(group, ui->music_btn_stop);
  lv_group_add_obj(group, ui->music_btn_right);
  lv_group_add_obj(group, ui->music);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  lv_obj_add_event_cb(ui->music_btn_left, music_btn_left_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->music_btn_right, music_btn_right_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->music_btn_play, music_btn_play_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->music_btn_stop, music_btn_stop_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->music_slider_valume, music_slider_change_event_handler, LV_EVENT_KEY, NULL);
  lv_obj_add_event_cb(ui->music, screen_music_event_handler, LV_EVENT_KEY, NULL);

  if(read_file_num(MUSIC_FOLDER)>0)
  {
    String music_name = get_file_name_by_index(MUSIC_FOLDER, music_index_num);
    music_set_label_text(music_name.c_str());
  }
}

//Set the label display content
void music_set_label_text(const char *text){
  if(text!=NULL){
    lv_label_set_text(guider_music_ui.music_mp3_label, text);
  }
  else{
    lv_label_set_text(guider_music_ui.music_mp3_label, "The music folder has no files.");
  }
}



//music player thread
void loopTask_music(void *pvParameters) {
  Serial.println("loopTask_music start...");
  int temp = 0;
  while (music_task_flag == 1) {
    audio_output_loop();
    int t1 = audio_get_total_output_playing_time();//Gets how long the music player has been playing
    int t2 = audio_output_get_file_duration();     //Gets the playing time of the music file
    int t3 = audio_read_output_play_position();    //Gets the current playing time of the music
    if(temp==1){
      int t4 = map(t3, 0, t2, 0, 100);
      if(t4<=100)
        lv_bar_set_value(guider_music_ui.music_bar_time, t4, LV_ANIM_OFF);
    }    
    if ((t1 < t2) && (t2 > 0) && (temp == 0)) { //The music starts to play
      lv_bar_set_value(guider_music_ui.music_bar_time, 0, LV_ANIM_OFF);
      temp = 1;
    } else if ((t2 == 0) && (temp == 1)) {      //The music stop to play
      temp = 0;
      music_task_flag = 0;
  
      //lv_img_set_src(guider_music_ui.music_btn_play, LV_SYMBOL_PLAY);
      music_button_state = 0;
      break;
    }
  }
  audio_output_stop();
  Serial.println("loopTask_music stop...");
  vTaskDelete(musicTaskHandle);
}

//Create music task thread
void start_music_task(void) {
  if (music_task_flag == 0) {
    music_task_flag = 1;
    xTaskCreate(loopTask_music, "loopTask_music", 10240, NULL, 1, &musicTaskHandle);
  } else {
    Serial.println("loopTask_music is running...");
  }
}

//Close the music thread
void stop_music_task(void) {
  if (music_task_flag == 1) {
    music_task_flag = 0;
    while (1) {
      if (eTaskGetState(musicTaskHandle) == eDeleted) {
        break;
      }
      vTaskDelay(10);
    }
    Serial.println("loopTask_music deleted!");
  }
}

//Check whether the thread is running
int music_task_is_running(void) {
  return music_task_flag;
}







