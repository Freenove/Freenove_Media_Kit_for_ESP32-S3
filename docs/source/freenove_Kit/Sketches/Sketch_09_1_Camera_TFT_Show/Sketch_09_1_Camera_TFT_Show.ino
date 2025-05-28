/*
* Sketch_08_1_Camera_TFT_Show.ino
* This sketch captures images from an ESP32S3 Eye camera module and displays them on a TFT screen.
* It initializes the camera and TFT display, captures frames, crops the image if necessary, and displays it on the TFT.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include <TFT_eSPI.h>
#include "esp_camera.h"
#define CAMERA_MODEL_ESP32S3_EYE  // Has PSRAM
#include "camera_pins.h"

#define TFT_BL 20
#define TFT_DIRECTION 0  // Define the direction of the TFT display (0, 1, 2, or 3)

TFT_eSPI tft = TFT_eSPI();

void tftRst(void) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW);
  delay(50);
  digitalWrite(TFT_BL, HIGH);
  delay(50);
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  tftRst();
  tft.init();                      // Initialize the TFT display
  tft.setRotation(TFT_DIRECTION);  // Set the rotation of the TFT display
  camera_init();                   // Initialize the camera
}

void loop() {
  // Capture a frame from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // Define screen and camera dimensions
  int screenWidth = 135;
  int screenHeight = 240;
  int camWidth = fb->width;
  int camHeight = fb->height;

  // Calculate cropping area
  int cropWidth = screenWidth;
  int cropHeight = screenHeight;
  int cropStartX = (camWidth - cropWidth) / 2;
  int cropStartY = (camHeight - cropHeight) / 2;

  // Check if cropping is needed
  if (camWidth > screenWidth || camHeight > screenHeight) {
    // Allocate memory for cropped image
    uint16_t *croppedBuffer = (uint16_t *)malloc(cropWidth * cropHeight * sizeof(uint16_t));
    if (!croppedBuffer) {
      Serial.println("Failed to allocate memory for cropped image");
      esp_camera_fb_return(fb);
      return;
    }

    // Crop the image
    for (int y = 0; y < cropHeight; y++) {
      for (int x = 0; x < cropWidth; x++) {
        croppedBuffer[y * cropWidth + x] = ((uint16_t *)fb->buf)[(cropStartY + y) * camWidth + (cropStartX + x)];
      }
    }

    // Display cropped image on the TFT screen
    tft.startWrite();
    tft.pushImage(0, 0, cropWidth, cropHeight, croppedBuffer);
    tft.endWrite();

    // Free the cropped image buffer
    free(croppedBuffer);
  } else {
    // If camera size is less than or equal to screen size, display the image directly
    tft.startWrite();
    tft.pushImage(0, 0, camWidth, camHeight, fb->buf);
    tft.endWrite();
  }

  // Return the frame buffer to the driver for reuse
  esp_camera_fb_return(fb);
}

void camera_init(void) {
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
  config.frame_size = FRAMESIZE_240X240;
  config.pixel_format = PIXFORMAT_RGB565;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize the camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed, error code 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // The initial sensor may be vertically flipped and have high color saturation
  s->set_hmirror(s, 1);     // Mirror the image horizontally
  s->set_vflip(s, 0);       // Restore vertical orientation
  s->set_brightness(s, 1);  // Slightly increase brightness
  s->set_saturation(s, 0);  // Reduce saturation
}