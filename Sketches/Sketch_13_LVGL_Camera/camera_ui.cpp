#include "camera_ui.h"

lv_img_dsc_t photo_show;          // apply an lvgl image variable
lvgl_camera_ui guider_camera_ui;  // camera ui structure

camera_fb_t *fb = NULL;              // data structure of camera frame buffer
TaskHandle_t cameraTaskHandle;       // camera thread task handle
static int camera_task_flag = 0;     // camera thread running flag
static int camera_take_a_photo = 0;  // take a photo flag
static int fileCounter = 0;          // file counter

// Define a structure to hold the parameters
typedef struct
{
  uint32_t screen_width;
  uint32_t screen_height;
} camera_task_params_t;

// Create camera task thread
void create_camera_task(uint32_t screen_width, uint32_t screen_height) {
  if (camera_task_flag == 0) {
    camera_task_flag = 1;
    camera_task_params_t *params = (camera_task_params_t *)malloc(sizeof(camera_task_params_t));
    if (params == NULL) {
      Serial.println("Failed to allocate memory for camera task parameters");
      return;
    }
    params->screen_width = screen_width;
    params->screen_height = screen_height;
    xTaskCreate(loopTask_camera, "loopTask_camera", 8192, params, 1, &cameraTaskHandle);
  } else {
    Serial.println("loopTask_camera is running...");
  }
}

// Close the camera thread
void stop_camera_task(void) {
  if (camera_task_flag == 1) {
    camera_task_flag = 0;
    while (1) {
      if (eTaskGetState(cameraTaskHandle) == eDeleted) {
        break;
      }
      vTaskDelay(10);
    }
    Serial.println("loopTask_camera deleted!");
  }
}

// Rotate and crop the image
void rotate_and_crop_image(const uint8_t *input, int inputWidth, int inputHeight,
                           uint8_t *output, int outputWidth, int outputHeight, int angle) {
  uint8_t *rotatedBuffer = (uint8_t *)malloc(inputHeight * inputWidth * 2 * sizeof(uint8_t));
  if (!rotatedBuffer) {
    Serial.println("Failed to allocate memory for rotated image.");
    return;
  }

  switch (angle) {
    case 0:
      for (int y = 0; y < inputHeight; y++) {
        for (int x = 0; x < inputWidth; x++) {
          rotatedBuffer[(y * inputWidth + x) * 2] = input[(y * inputWidth + x) * 2 + 1];
          rotatedBuffer[(y * inputWidth + x) * 2 + 1] = input[(y * inputWidth + x) * 2];
        }
      }
      break;
    case 90:
      for (int y = 0; y < inputHeight; y++) {
        for (int x = 0; x < inputWidth; x++) {
          int new_x = inputHeight - 1 - y;
          int new_y = x;
          rotatedBuffer[(new_y * inputHeight + new_x) * 2] = input[(y * inputWidth + x) * 2 + 1];
          rotatedBuffer[(new_y * inputHeight + new_x) * 2 + 1] = input[(y * inputWidth + x) * 2];
        }
      }
      break;
    case 180:
      for (int y = 0; y < inputHeight; y++) {
        for (int x = 0; x < inputWidth; x++) {
          int new_x = inputWidth - 1 - x;
          int new_y = inputHeight - 1 - y;
          rotatedBuffer[(new_y * inputWidth + new_x) * 2] = input[(y * inputWidth + x) * 2 + 1];
          rotatedBuffer[(new_y * inputWidth + new_x) * 2 + 1] = input[(y * inputWidth + x) * 2];
        }
      }
      break;
    case 270:
      for (int y = 0; y < inputHeight; y++) {
        for (int x = 0; x < inputWidth; x++) {
          int new_x = y;
          int new_y = inputWidth - 1 - x;
          rotatedBuffer[(new_y * inputHeight + new_x) * 2] = input[(y * inputWidth + x) * 2 + 1];
          rotatedBuffer[(new_y * inputHeight + new_x) * 2 + 1] = input[(y * inputWidth + x) * 2];
        }
      }
      break;
    default:
      for (int y = 0; y < inputHeight; y++) {
        for (int x = 0; x < inputWidth; x++) {
          rotatedBuffer[(y * inputWidth + x) * 2] = input[(y * inputWidth + x) * 2 + 1];
          rotatedBuffer[(y * inputWidth + x) * 2 + 1] = input[(y * inputWidth + x) * 2];
        }
      }
      break;
  }

  int cropStartX, cropStartY;
  switch (angle) {
    case 0:
    case 180:
      cropStartX = (inputWidth - outputWidth) / 2;
      cropStartY = (inputHeight - outputHeight) / 2;
      break;
    case 90:
    case 270:
      cropStartX = (inputHeight - outputWidth) / 2;
      cropStartY = (inputWidth - outputHeight) / 2;
      break;
    default:
      cropStartX = (inputWidth - outputWidth) / 2;
      cropStartY = (inputHeight - outputHeight) / 2;
      break;
  }
  for (int y = 0; y < outputHeight; y++) {
    for (int x = 0; x < outputWidth; x++) {
      output[(y * outputWidth + x) * 2] = rotatedBuffer[((cropStartY + y) * (angle % 180 == 0 ? inputWidth : inputHeight) + (cropStartX + x)) * 2];
      output[(y * outputWidth + x) * 2 + 1] = rotatedBuffer[((cropStartY + y) * (angle % 180 == 0 ? inputWidth : inputHeight) + (cropStartX + x)) * 2 + 1];
    }
  }

  free(rotatedBuffer);
}

// Camera thread
void loopTask_camera(void *pvParameters) {
  camera_task_params_t *params = (camera_task_params_t *)pvParameters;
  uint32_t screen_width = params->screen_width;
  uint32_t screen_height = params->screen_height;
  free(params);  // Free the allocated parameters
  Serial.println("loopTask_camera start...");
  while (camera_task_flag) {
    if (camera_take_a_photo == 0) {
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        return;
      }
      uint8_t *buffer = (uint8_t *)malloc(screen_width * screen_height * 2 * sizeof(uint8_t));
      if (!buffer) {
        Serial.println("Failed to allocate memory for rotated image");
        esp_camera_fb_return(fb);
        return;
      }
      rotate_and_crop_image(fb->buf, fb->width, fb->height, buffer, screen_width, screen_height, 90);
      photo_show.data = buffer;
      lv_img_set_src(guider_camera_ui.camera_video, &photo_show);
      free(buffer);  // Free image buffer
      esp_camera_fb_return(fb);
    } else {
      camera_fb_t *fb = esp_camera_fb_get();
      if (!fb) {
        Serial.println("Camera capture failed");
        return;
      }
      char filename[255];
      sensor_t *s = esp_camera_sensor_get();
      if (s->pixformat == PIXFORMAT_RGB565) {
        snprintf(filename, sizeof(filename), "%s/photo%d.bmp", CAMERA_FOLDER, fileCounter);
        Serial.printf("%s size is %d\r\n", filename, (screen_width * screen_height * 2));
        uint8_t *buffer = (uint8_t *)malloc(screen_width * screen_height * 2 * sizeof(uint8_t));
        if (!buffer) {
          Serial.println("Failed to allocate memory for rotated image");
          esp_camera_fb_return(fb);
          return;
        }
        rotate_and_crop_image(fb->buf, fb->width, fb->height, buffer, screen_width, screen_height, 90);
        write_bmp(filename, buffer, (screen_height * screen_width * 2), screen_height, screen_width);
        free(buffer);
      } else if (s->pixformat == PIXFORMAT_JPEG) {
        snprintf(filename, sizeof(filename), "%s/photo%d.jpg", CAMERA_FOLDER, fileCounter);
        Serial.printf("%s size is %d\r\n", filename, fb->len);
        write_jpg(filename, fb->buf, fb->len);
      }
      fileCounter++;
      esp_camera_fb_return(fb);
      camera_take_a_photo = 0;
    }
  }
  vTaskDelete(cameraTaskHandle);
}

// Initialize an lvgl image variable
void ui_set_photo_show(uint32_t screen_width, uint32_t screen_height) {
  lv_img_header_t header;
  header.always_zero = 0;
  header.w = screen_width;
  header.h = screen_height;
  header.cf = LV_IMG_CF_TRUE_COLOR;
  photo_show.header = header;
  photo_show.data_size = screen_width * screen_height * 2;
  photo_show.data = NULL;
}

// Gesture event handler
static void camera_screen_gesture_event_handler(lv_event_t *event) {
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY) {
    uint32_t c = lv_event_get_key(event);
    if (c == LV_KEY_ENTER) {
      if (camera_take_a_photo == 0)
        camera_take_a_photo = 1;
    }
    else if(c == LV_KEY_LEFT || c == LV_KEY_RIGHT)
    {
      /*
      stop_camera_task();
      Serial.println("Jump to Main Screen!");
      if (!lv_obj_is_valid(guider_main_ui.main))
          setup_scr_main(&guider_main_ui);
      lv_scr_load(guider_main_ui.main);
      lv_obj_del(guider_camera_ui.camera);
      */
    }
  }
} 

// Parameter configuration function on the camera screen
void setup_scr_camera(lvgl_camera_ui *ui) {
  // Write codes camera
  ui->camera = lv_obj_create(NULL);

  static lv_style_t bg_style;
  lv_style_init(&bg_style);
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));
  lv_obj_add_style(ui->camera, &bg_style, LV_PART_MAIN);

  // Get the current active screen width and height
  lv_obj_t *scr = lv_disp_get_scr_act(NULL);          // Get the current screen
  lv_coord_t screen_width = lv_obj_get_width(scr);    // Get screen width
  lv_coord_t screen_height = lv_obj_get_height(scr);  // Get screen height

  // Write codes camera_video
  ui->camera_video = lv_img_create(ui->camera);
  lv_obj_set_pos(ui->camera_video, 0, 0);
  lv_obj_set_size(ui->camera_video, screen_width, screen_height);

  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->camera);
  lv_indev_set_group(indev_keypad, group);

  ui_set_photo_show(screen_width, screen_height);
  create_camera_task(screen_width, screen_height);
  lv_obj_add_event_cb(ui->camera, camera_screen_gesture_event_handler, LV_EVENT_ALL, NULL);
}

// Camera initialization function
void camera_init(int state) {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 10000000;
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  if (state == 0) {
    config.frame_size = FRAMESIZE_240X240;
    config.pixel_format = PIXFORMAT_RGB565;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.pixel_format = PIXFORMAT_JPEG;
  }
  // Initialize the camera
  esp_camera_deinit();
  esp_camera_return_all();
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed, error code 0x%x", err);
    return;
  }
  sensor_t *s = esp_camera_sensor_get();
  // The initial sensor may be vertically flipped and have high color saturation
  s->set_hmirror(s, 1);
  s->set_vflip(s, 0);       // Restore vertical orientation
  s->set_brightness(s, 1);  // Slightly increase brightness
  s->set_saturation(s, 0);  // Reduce saturation
}