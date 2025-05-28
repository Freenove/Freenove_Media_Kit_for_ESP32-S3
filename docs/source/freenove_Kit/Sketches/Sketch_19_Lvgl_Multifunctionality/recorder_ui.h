// Include guard for header file
#ifndef __RECORDER_UI_H
#define __RECORDER_UI_H

#include "public.h"

// Structure definition for LVGL recorder UI components
typedef struct lvgl_recorder {
    lv_obj_t *recorder;                  // Main container object for recorder UI
    lv_obj_t *recorder_btn_sound_record; // Record button object
    lv_obj_t *recorder_btn_sound_record_label; // Label for record button
    lv_obj_t *recorder_btn_sound_play;   // Play button object
    lv_obj_t *recorder_btn_sound_play_label;   // Label for play button
} lvgl_recorder_ui;

// Global instance of recorder UI structure
extern lvgl_recorder_ui guider_recorder_ui;

// Function to initialize the sound recorder screen
void setup_scr_sound_recorder(lvgl_recorder_ui *ui);

// Task management functions for sound recorder
void start_recorder_task(void);      // Start recording task
void stop_recorder_task(void);       // Stop recording task
int recorder_task_is_running(void);  // Check if recording task is active
void loopTask_sound_recorder(void *pvParameters); // Main recording task loop

// Task management functions for audio playback
void start_audio_play_task(void);         // Start audio playback task
void stop_audio_play_task(void);          // Stop audio playback task
int play_task_is_running(void);      // Check if playback task is active
void loopTask_audio_play(void *pvParameters); // Main playback task loop

#endif