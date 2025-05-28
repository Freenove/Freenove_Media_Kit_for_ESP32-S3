/*
* Sketch_08_2_Take_A_Photo.ino
* This sketch captures images from an ESP32S3 Eye camera module and displays them on a TFT screen.
* It also allows taking photos by pressing a button and saving them to an SD card.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include <TFT_eSPI.h>
#include "esp_camera.h"
#include "driver_sdmmc.h"
#define CAMERA_MODEL_ESP32S3_EYE  // Has PSRAM
#include "camera_pins.h"

#define BUTTON_PIN 19  // Please do not modify it.
#define SD_MMC_CMD 38  // Please do not modify it.
#define SD_MMC_CLK 39  // Please do not modify it.
#define SD_MMC_D0 40   // Please do not modify it.
#define TFT_BL 20

TFT_eSPI tft = TFT_eSPI();

void camera_init(int state);
void cameraShow(void);
void cameraPhoto(void);
void tft_rst(void);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  tft_rst();
  tft.init();
  tft.setRotation(0);
  camera_init(0);
  sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  remove_dir("/video");
  create_dir("/video");
}

void loop() {
  cameraShow();   // Continuously display the camera feed on the TFT screen
  cameraPhoto();  // Check for button press to take a photo
}

void tft_rst(void) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW);
  delay(50);
  digitalWrite(TFT_BL, HIGH);
  delay(50);
}

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
    config.frame_size = FRAMESIZE_VGA;
    config.pixel_format = PIXFORMAT_JPEG;
  }
  // Deinitialize and reinitialize the camera with the new configuration
  esp_camera_deinit();
  esp_camera_return_all();
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

void cameraShow(void) {
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

void cameraPhoto(void) {
  static int fileCounter = 0;
  int analogValue = analogRead(BUTTON_PIN);
  if (analogValue < 100) {
    camera_init(1);  // Reinitialize camera for photo capture
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }
    char filename[32];
    snprintf(filename, sizeof(filename), "/video/photo_%04d.jpg", fileCounter);
    write_jpg(filename, fb->buf, fb->len);  // Save the photo to the SD card
    fileCounter++;
    camera_init(0);         // Reinitialize camera for live view
    list_dir("/video", 0);  // List the contents of the /video directory
    while (analogRead(BUTTON_PIN) < 3000)
      ;  // Wait for button release
  }
}