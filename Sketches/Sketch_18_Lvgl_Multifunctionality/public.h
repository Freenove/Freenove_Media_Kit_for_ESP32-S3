#ifndef __PUBLIC_H
#define __PUBLIC_H

#include "lvgl.h"
#include "Arduino.h"
#include "stdlib.h"
#include "string.h"

#include "display.h"
#include "Freenove_WS2812_Lib_for_ESP32.h"
#include "esp_camera.h"
#define CAMERA_MODEL_ESP32S3_EYE
#include "camera_pins.h"

#include "driver_audio_input.h"
#include "driver_audio_output.h"
#include "driver_sdmmc.h"

#include "main_ui.h"
#include "ws2812_ui.h"
#include "camera_ui.h"
#include "picture_ui.h"
#include "music_ui.h"
#include "recorder_ui.h"

#define CAMERA_FOLDER   "/picture"
#define PICTURE_FOLDER  "/picture"
#define MUSIC_FOLDER    "/music"
#define RECORDER_FOLDER "/recorder"

#define MOLLOC_SIZE     (1024 * 1024)

#endif


