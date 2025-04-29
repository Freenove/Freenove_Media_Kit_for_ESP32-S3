#include "recorder_ui.h"

// Global instances and flags for audio tasks
lvgl_recorder_ui guider_recorder_ui;  // UI structure instance

int recorder_task_flag = 0;       // Task status flag (0=stopped, 1=running)
int play_task_flag = 0;           // Playback task status flag

/* Event handler for record button */
static void record_btn_sound_record_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Record Btn Enter!");  // Debug output
      start_recorder_task();                  // Start recording when button released
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Record Btn Release!");  // Debug output
    stop_recorder_task();
  }
}

/* Event handler for play button */
static void record_btn_sound_play_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Play Btn Enter!");  // Debug output
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Play Btn Release!");  // Debug output
    start_audio_play_task();              // Start playback when button released
  }
}

static void screen_sound_record_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t key = lv_event_get_key(event);
    if (key == LV_KEY_ENTER) {
      Serial.println("Recorder Btn Enter!");  // Debug output
    } else if (key == LV_KEY_LEFT || key == LV_KEY_RIGHT) {
        Serial.println("Jump to Main Screen!");
        if (!lv_obj_is_valid(guider_main_ui.main))
            setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_recorder_ui.recorder);
    }
  } else if (code == LV_EVENT_RELEASED) {
    Serial.println("Recorder Btn Released!");
  }
}

/* Initialize the sound recorder screen */
void setup_scr_sound_recorder(lvgl_recorder_ui *ui) {
  // Create main screen container
  ui->recorder = lv_obj_create(NULL);
  lv_obj_t *scr = lv_disp_get_scr_act(NULL);
  lv_coord_t screen_width = lv_obj_get_width(scr);
  lv_coord_t screen_height = lv_obj_get_height(scr);
  lv_obj_set_size(ui->recorder, screen_width, screen_height);

  // Style setup for background
  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->recorder, &bg_style, LV_PART_MAIN);

  // Button pressed effect style
  static lv_style_t style_pr;
  lv_style_init(&style_pr);
  lv_style_set_translate_y(&style_pr, 5);  // Move down 5px when pressed

  // Create record button
  ui->recorder_btn_sound_record = lv_btn_create(ui->recorder);
  lv_obj_set_size(ui->recorder_btn_sound_record, 75, 50);
  lv_obj_set_pos(ui->recorder_btn_sound_record,
                 (screen_width - (75 * 2)) / 3, ((screen_height - 50) / 2));
  lv_obj_add_style(ui->recorder_btn_sound_record, &style_pr, LV_STATE_PRESSED);
  ui->recorder_btn_sound_record_label = lv_label_create(ui->recorder_btn_sound_record);
  lv_obj_set_style_text_align(ui->recorder_btn_sound_record_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(ui->recorder_btn_sound_record_label, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(ui->recorder_btn_sound_record_label, "Record");

  // Create play button
  ui->recorder_btn_sound_play = lv_btn_create(ui->recorder);
  lv_obj_set_size(ui->recorder_btn_sound_play, 75, 50);
  lv_obj_align_to(ui->recorder_btn_sound_play, ui->recorder_btn_sound_record,
                  LV_ALIGN_OUT_RIGHT_MID, 30, 0);
  lv_obj_add_style(ui->recorder_btn_sound_play, &style_pr, LV_STATE_PRESSED);
  ui->recorder_btn_sound_play_label = lv_label_create(ui->recorder_btn_sound_play);
  lv_obj_set_style_text_align(ui->recorder_btn_sound_play_label, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(ui->recorder_btn_sound_play_label, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(ui->recorder_btn_sound_play_label, "Play");

  // Input device configuration
  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->recorder_btn_sound_record);
  lv_group_add_obj(group, ui->recorder_btn_sound_play);
  lv_group_add_obj(group, ui->recorder);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  // Register event handlers
  lv_obj_add_event_cb(ui->recorder_btn_sound_record, record_btn_sound_record_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->recorder_btn_sound_play, record_btn_sound_play_event_handler, LV_EVENT_ALL, NULL);
  lv_obj_add_event_cb(ui->recorder, screen_sound_record_event_handler, LV_EVENT_ALL, NULL);
}

/* Start recording task */
void start_recorder_task(void) {
  if (recorder_task_flag == 0) {
    recorder_task_flag = 1;
    xTaskCreate(loopTask_sound_recorder, "loopTask_sound_recorder", 4096, NULL, 1, NULL);
  } else {
    Serial.println("loopTask_sound_recorder is running...");
  }
}

/* Stop recording task */
void stop_recorder_task(void) {
  if (recorder_task_flag == 1) {
    recorder_task_flag = 0;
    Serial.println("loopTask_sound_recorder deleted!");
  }
}

/* Check if recording task is active */
int recorder_task_is_running(void) {
  return recorder_task_flag;
}

/* Main recording task loop */
void loopTask_sound_recorder(void *pvParameters) {
  Serial.println("loopTask_sound_recorder start...");

  // Initialize the total size of recorded data
  int total_size = 0;
  // Allocate memory in PSRAM for storing audio data
  char *buffer = (char *)heap_caps_malloc(MOLLOC_SIZE, MALLOC_CAP_SPIRAM);
  // Get the index for the next recording file
  int wav_index = read_file_num(RECORDER_FOLDER);
  // Generate the file name for the new recording
  String file_name = String(RECORDER_FOLDER) + "/recording_" + String(wav_index) + ".wav";
  // Write the WAV header to the file
  write_wav_header(file_name.c_str(), total_size);
  // Loop while the recorder task is running
  while (recorder_task_flag == 1) {
    // Get the available IIS data size
    int iis_buffer_size = audio_input_get_iis_data_available();
    // Loop while there is IIS data available
    while (iis_buffer_size > 0) {
      // Check if the buffer is full
      if ((total_size + 512) >= MOLLOC_SIZE) {
        // Stop the recorder task if the buffer is full
        recorder_task_flag = 0;
        break;
      }
      // Read IIS data into the buffer
      int real_size = audio_input_read_iis_data(buffer + total_size, 512);
      // Update the total size of recorded data
      total_size += real_size;
      // Decrease the available IIS data size
      iis_buffer_size -= real_size;
    }
  }
  // Append the recorded data to the file
  append_file(file_name.c_str(), (uint8_t *)(buffer), total_size);
  // Update the WAV header with the final file size
  write_wav_header(file_name.c_str(), total_size);
  // Free the allocated memory in PSRAM
  heap_caps_free(buffer);
  // Print a message indicating the end of the recording task
  Serial.println("loopTask_sound_recorder stop...");
  vTaskDelete(NULL);
}

/* Start audio playback task */
void start_audio_play_task(void) {
  if (play_task_flag == 0) {
    play_task_flag = 1;
    int file_count = read_file_num(RECORDER_FOLDER) - 1;
    if (file_count >= 0) {
      String file_name = get_file_name_by_index(RECORDER_FOLDER, file_count);
      char path[255] = { 0 };
      strcat(path, RECORDER_FOLDER);
      strcat(path, "/");
      strcat(path, file_name.c_str());
      Serial.println(path);
      audio_output_load_music(path);
      xTaskCreate(loopTask_audio_play, "loopTask_audio_play", 4096, NULL, 1, NULL);
    } else {
      Serial.println("No wav to play!");
    }
  } else {
    Serial.println("loopTask_audio_play is running...");
  }
}

/* Stop audio playback task */
void stop_audio_play_task(void) {
  if (play_task_flag == 1) {
    play_task_flag = 0;
    Serial.println("loopTask_audio_play deleted!");
  }
}

/* Check if playback task is active */
int play_task_is_running(void) {
  return play_task_flag;
}

/* Main audio playback loop */
void loopTask_audio_play(void *pvParameters) {
  Serial.println("loopTask_audio_play start...");
  while (play_task_flag == 1) {
    audio_output_loop();
    play_task_flag = audio_output_is_running() ? 1 : 0;
  }
  Serial.println("loopTask_audio_play stop...");
  vTaskDelete(NULL);
}
