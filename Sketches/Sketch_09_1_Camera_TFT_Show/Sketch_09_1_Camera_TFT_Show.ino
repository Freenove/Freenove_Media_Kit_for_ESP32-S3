/*
* Sketch_09_1_Camera_TFT_Show.ino
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
#include <ESP_I2S.h>

#ifdef FNK0102A_1P14_135x240_ST7789
  int screenWidth = 135;
  int screenHeight = 240;
#elif defined FNK0102B_3P5_320x480_ST7796
  int screenWidth = 320;
  int screenHeight = 480;
#endif

#define TFT_BL 20
#define TFT_114_DIRECTION 0
#define TFT_35_DIRECTION 1
TFT_eSPI tft = TFT_eSPI();

I2SClass i2s_output; 

bool i2s_output_init(int bclk, int lrc, int dout) {
  i2s_output.setPins(bclk, lrc, dout, -1);
  if (!i2s_output.begin(I2S_MODE_STD, 16000, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_RIGHT)) {
    Serial.println("Failed to initialize I2S output bus!");
    return false;
  }
  i2s_output.write(0); 
  i2s_output.write(0); 
  i2s_output.end();
  return true;
}

// Global variable to track if we're using 3.5 inch screen
bool is35InchScreen = false;

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
  i2s_output_init(I2S_BCLK, I2S_LRC, I2S_DOUT);
  // Check if we're using 3.5 inch screen
#ifdef FNK0102B_3P5_320x480_ST7796
  is35InchScreen = true;
#endif

  tftRst();
  tft.init();                      // Initialize the TFT display
  if(is35InchScreen)
    tft.setRotation(TFT_35_DIRECTION);// Set the rotation of the TFT display
  else
    tft.setRotation(TFT_114_DIRECTION);// Set the rotation of the TFT display
  
  camera_init();                   // Initialize the camera
}

void loop() {
  // Capture a frame from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }

  // For 3.5 inch screen, display directly without cropping
  if (is35InchScreen) {
    // Direct display when dimensions match
    tft.startWrite();
    tft.pushImage(0, 0, fb->width, fb->height, (uint16_t*)fb->buf);
    tft.endWrite();
  } 
  else {
    // For 1.14 inch screen, use original cropping logic
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
  
  // Set frame size based on screen type
  if (is35InchScreen) {
    config.frame_size = FRAMESIZE_HVGA;  // 320x240 for 3.5 inch screen
  } else {
    config.frame_size = FRAMESIZE_240X240;  // 240x240 for 1.14 inch screen
  }
  
  config.pixel_format = PIXFORMAT_RGB565;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // Initialize the camera with the specified configuration
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // Get the camera sensor and adjust settings
  sensor_t* s = esp_camera_sensor_get();

  uint8_t pid = s->id.PID;

  if(pid == 0x45)
  {
    s->set_hmirror(s, 1);
    vTaskDelay(500);
    s->set_vflip(s, 1);       // Flip the image vertically
  }else if(pid == 0x26)
  {
    s->set_hmirror(s, 0);
    s->set_vflip(s, 0);       // Flip the image vertically
  }else if(pid == 0x9B)
  {
    s->set_hmirror(s, 0);
    vTaskDelay(500);
    s->set_vflip(s, 0);       // Flip the image vertically
  }
  else{
    s->set_hmirror(s, 0);
    s->set_vflip(s, 1);       // Flip the image vertically
  }
  s->set_brightness(s, 1);  // Increase brightness
  s->set_saturation(s, 0);  // Decrease saturation
  s->set_ae_level(s, -3);   // Set exposure compensation level
}
