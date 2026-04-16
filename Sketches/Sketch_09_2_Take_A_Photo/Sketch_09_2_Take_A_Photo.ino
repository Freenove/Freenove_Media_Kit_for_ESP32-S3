/*  
* Sketch_09_2_Take_A_Photo.ino
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
#include <ESP_I2S.h>

#ifdef FNK0102A_1P14_135x240_ST7789
  int screenWidth = 135;
  int screenHeight = 240;
#elif defined FNK0102B_3P5_320x480_ST7796
  int screenWidth = 320;
  int screenHeight = 480;
#endif

#define BUTTON_PIN 19  // Please do not modify it.
#define SD_MMC_CMD 38  // Please do not modify it.
#define SD_MMC_CLK 39  // Please do not modify it.
#define SD_MMC_D0 40   // Please do not modify it.
#define TFT_BL 20

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

TFT_eSPI tft = TFT_eSPI();

void camera_init(int state);
void cameraShow(void);
void cameraPhoto(void);
void tft_rst(void);

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();
  i2s_output_init(I2S_BCLK, I2S_LRC, I2S_DOUT);
  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Check if we're using 3.5 inch screen
#ifdef FNK0102B_3P5_320x480_ST7796
  is35InchScreen = true;
#endif

  tft_rst();
  tft.init();
  if(is35InchScreen)
    tft.setRotation(1);            // Set the rotation of the TFT display
  else
    tft.setRotation(0);// Set the rotation of the TFT display
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
    if (is35InchScreen) {
      config.frame_size = FRAMESIZE_HVGA;
    } else {
      config.frame_size = FRAMESIZE_240X240;
    }
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

void cameraShow(void) {
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
    while (analogRead(BUTTON_PIN) < 3000);  // Wait for button release
  }
}
