/*
* Sketch_06_Video_Web_Server.ino
* This sketch sets up a camera module (ESP32S3 Eye) and serves video stream over a web server.
* It initializes the camera, connects to a Wi-Fi network, and starts a web server to stream video.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include "esp_camera.h"
#include <WiFi.h>
#include "driver_sdmmc.h"

// Select camera model
#define CAMERA_MODEL_ESP32S3_EYE  // Has PSRAM
#include "camera_pins.h"

const char* ssid = "********";         // Input your Wi-Fi name
const char* password = "**********";  // Input your Wi-Fi password

#define SD_MMC_CMD 38  // Please do not modify it.
#define SD_MMC_CLK 39  // Please do not modify it.
#define SD_MMC_D0 40   // Please do not modify it.

void camera_init(void);
void startCameraServer();

void setup() {
 Serial.begin(115200);
 while (!Serial)
   ;
 Serial.setDebugOutput(true);
 Serial.println();

 // Initialize the camera
 camera_init();
 // Initialize the SDMMC interface for SD card operations
 sdmmc_init(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
 // Remove and create a new directory for video storage
 remove_dir("/video");
 create_dir("/video");

 // Connect to Wi-Fi network
 WiFi.begin(ssid, password);

 // Wait for Wi-Fi connection
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 Serial.println("");
 Serial.println("WiFi connected");

 // Start the camera web server
 startCameraServer();

 // Print the IP address to connect to the web server
 Serial.print("Camera Ready! Use 'http://");
 Serial.print(WiFi.localIP());
 Serial.println("' to connect");
}

void loop() {
 // Main loop code, can be used for additional tasks
 delay(10000);
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
 config.frame_size = FRAMESIZE_VGA;
 config.pixel_format = PIXFORMAT_JPEG;  // for streaming
 config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
 config.fb_location = CAMERA_FB_IN_PSRAM;
 config.jpeg_quality = 12;
 config.fb_count = 1;

 // If PSRAM IC is present, use higher resolution and quality
 if (psramFound()) {
   config.jpeg_quality = 10;
   config.fb_count = 2;
   config.grab_mode = CAMERA_GRAB_LATEST;
 } else {
   // Limit the frame size when PSRAM is not available
   config.frame_size = FRAMESIZE_VGA;
   config.fb_location = CAMERA_FB_IN_DRAM;
 }

 // Initialize the camera with the specified configuration
 esp_err_t err = esp_camera_init(&config);
 if (err != ESP_OK) {
   Serial.printf("Camera init failed with error 0x%x", err);
   return;
 }

 // Get the camera sensor and adjust settings
 sensor_t* s = esp_camera_sensor_get();
 s->set_vflip(s, 0);       // Flip the image vertically
 s->set_brightness(s, 1);  // Increase brightness
 s->set_saturation(s, 0);  // Decrease saturation
}