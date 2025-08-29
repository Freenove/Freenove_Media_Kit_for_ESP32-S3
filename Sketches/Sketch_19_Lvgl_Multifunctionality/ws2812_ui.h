#ifndef __WS2812_UI_H
#define __WS2812_UI_H

#include "public.h"
#ifdef FNK0102A_1P14_135x240_ST7789
#define LEDS_COUNT 1        // Number of LEDs
#elif defined FNK0102B_3P5_320x480_ST7796
#define LEDS_COUNT 4
#endif
#define LEDS_PIN 48         // Pin connected to the LEDs
#define CHANNEL 0           // Channel for the LED strip

// Structure to hold UI elements for WS2812 LED control
typedef struct lvgl_ws2812 {
    lv_obj_t *ws2812;                                // Main container for the WS2812 UI

    lv_obj_t *ws2812_slider_red;                     // Slider for red color
    lv_obj_t *ws2812_slider_green;                   // Slider for green color
    lv_obj_t *ws2812_slider_blue;                    // Slider for blue color
    lv_obj_t *ws2812_slider_brightness;              // Slider for brightness

    lv_obj_t *ws2812_lable_red;                      // Label for red slider
    lv_obj_t *ws2812_lable_green;                    // Label for green slider
    lv_obj_t *ws2812_lable_blue;                     // Label for blue slider
    lv_obj_t *ws2812_lable_brightness;               // Label for brightness slider

    lv_obj_t *ws2812_lable_red_value;                // Label to display red slider value
    lv_obj_t *ws2812_lable_green_value;              // Label to display green slider value
    lv_obj_t *ws2812_lable_blue_value;               // Label to display blue slider value
    lv_obj_t *ws2812_lable_brightness_value;         // Label to display brightness slider value
} lvgl_ws2812_ui;

extern lvgl_ws2812_ui guider_ws2812_ui;    // WS2812 UI structure

// Function to set up the WS2812 screen
void setup_scr_ws2812(lvgl_ws2812_ui *ui);

#endif