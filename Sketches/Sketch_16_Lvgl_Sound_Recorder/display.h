#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "lvgl.h"
#include "TFT_eSPI.h"
#include "driver_button.h"

#define TFT_BL 20
#define BUTTON_PIN 19
#define TFT_DIRECTION 1           // Define the direction of the TFT display
extern lv_indev_t* indev_keypad;  // External declaration of the keypad input device
// Class to handle display operations
class Display {
private:
    int tft_show_dirction;  // Non-static member variable for display direction

public:
    // Function to initialize the display
    void init(int screenDir);

    // Function to handle routine display tasks
    void routine();

    // Getter for tft_show_dirction
    int getTftShowDirection() const { return tft_show_dirction; }

    // Setter for tft_show_dirction
    void setTftShowDirection(int direction) { tft_show_dirction = direction; }
};

#endif