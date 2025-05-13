/*
* Sketch_07_TFT_Clock.ino
* This sketch displays a clock on a TFT display using the TFT_eSPI library.
* It updates the time every second and draws hour, minute, and second hands.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#define TFT_BL 20
#define TFT_DIRECTION 1  // Define the direction of the TFT display (0, 1, 2, or 3)

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define TFT_GREY 0xBDF7  // Define a custom grey color for the background

// Variables to store the positions and angles of the clock hands
float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;                // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;                                   // Degrees for second, minute, and hour hands
uint16_t osx = 64, osy = 64, omx = 64, omy = 64, ohx = 64, ohy = 64;  // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;                            // Temporary variables for drawing lines
uint32_t targetTime = 0;                                              // Time for the next second timeout

// Function to convert a two-character string to an integer
static uint8_t conv2d(const char *p) {
  uint8_t v = 0;
  if ('0' <= *p && *p <= '9')
    v = *p - '0';
  return 10 * v + *++p - '0';
}

// Get the current time from the compile time
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6);

bool initial = 1;  // Flag to indicate initial setup

void tftRst(void) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW);
  delay(50);
  digitalWrite(TFT_BL, HIGH);
  delay(50);
}

void setup(void) {
  tftRst();
  tft.init();                             // Initialize the TFT display
  tft.setRotation(TFT_DIRECTION);         // Set the rotation of the display
  tft.fillScreen(TFT_GREY);               // Fill the screen with grey color
  tft.setTextColor(TFT_GREEN, TFT_GREY);  // Set text color and background color

  // Draw the clock face
  tft.fillCircle(64, 64, 61, TFT_BLUE);   // Outer circle
  tft.fillCircle(64, 64, 57, TFT_BLACK);  // Inner circle

  // Draw 12 lines for the clock marks
  for (int i = 0; i < 360; i += 30) {
    sx = cos((i - 90) * 0.0174532925);  // Calculate x multiplier
    sy = sin((i - 90) * 0.0174532925);  // Calculate y multiplier
    x0 = sx * 57 + 64;                  // Calculate x start point
    yy0 = sy * 57 + 64;                 // Calculate y start point
    x1 = sx * 50 + 64;                  // Calculate x end point
    yy1 = sy * 50 + 64;                 // Calculate y end point

    tft.drawLine(x0, yy0, x1, yy1, TFT_BLUE);  // Draw the line
  }

  // Draw 60 dots for the clock marks
  for (int i = 0; i < 360; i += 6) {
    sx = cos((i - 90) * 0.0174532925);  // Calculate x multiplier
    sy = sin((i - 90) * 0.0174532925);  // Calculate y multiplier
    x0 = sx * 53 + 64;                  // Calculate x point
    yy0 = sy * 53 + 64;                 // Calculate y point

    tft.drawPixel(x0, yy0, TFT_BLUE);  // Draw the dot
    // Draw larger dots at 12, 3, 6, 9 o'clock positions
    if (i == 0 || i == 180)
      tft.fillCircle(x0, yy0, 1, TFT_CYAN);
    if (i == 0 || i == 180)
      tft.fillCircle(x0 + 1, yy0, 1, TFT_CYAN);
    if (i == 90 || i == 270)
      tft.fillCircle(x0, yy0, 1, TFT_CYAN);
    if (i == 90 || i == 270)
      tft.fillCircle(x0 + 1, yy0, 1, TFT_CYAN);
  }

  tft.fillCircle(65, 65, 3, TFT_RED);  // Draw the center dot

  // Draw text at the center of the display
  tft.setTextColor(TFT_GREEN);  // Set text color

  // Position text based on the display rotation
#if TFT_DIRECTION == 0 || TFT_DIRECTION == 2
  tft.drawCentreString("Freenove", 64, 130, 4);
#elif TFT_DIRECTION == 1 || TFT_DIRECTION == 3
  tft.drawCentreString("Freenove", 184, 54, 4);
#endif

  targetTime = millis() + 1000;  // Set the target time for the next second
}

void loop() {
  if (targetTime < millis()) {
    targetTime = millis() + 1000;  // Update the target time for the next second
    ss++;                          // Increment seconds
    if (ss == 60) {
      ss = 0;  // Reset seconds
      mm++;    // Increment minutes
      if (mm > 59) {
        mm = 0;  // Reset minutes
        hh++;    // Increment hours
        if (hh > 23) {
          hh = 0;  // Reset hours
        }
      }
    }

    // Pre-compute hand degrees and positions for a fast screen update
    sdeg = ss * 6;                         // Calculate second hand angle (0-59 -> 0-354)
    mdeg = mm * 6 + sdeg * 0.01666667;     // Calculate minute hand angle (0-59 -> 0-360) including seconds
    hdeg = hh * 30 + mdeg * 0.0833333;     // Calculate hour hand angle (0-11 -> 0-360) including minutes and seconds
    hx = cos((hdeg - 90) * 0.0174532925);  // Calculate x multiplier for hour hand
    hy = sin((hdeg - 90) * 0.0174532925);  // Calculate y multiplier for hour hand
    mx = cos((mdeg - 90) * 0.0174532925);  // Calculate x multiplier for minute hand
    my = sin((mdeg - 90) * 0.0174532925);  // Calculate y multiplier for minute hand
    sx = cos((sdeg - 90) * 0.0174532925);  // Calculate x multiplier for second hand
    sy = sin((sdeg - 90) * 0.0174532925);  // Calculate y multiplier for second hand

    if (ss == 0 || initial) {
      initial = 0;  // Reset initial flag
      // Erase hour and minute hand positions every minute
      tft.drawLine(ohx, ohy, 65, 65, TFT_BLACK);  // Erase old hour hand position
      ohx = hx * 33 + 65;                         // Calculate new hour hand x position
      ohy = hy * 33 + 65;                         // Calculate new hour hand y position
      tft.drawLine(omx, omy, 65, 65, TFT_BLACK);  // Erase old minute hand position
      omx = mx * 44 + 65;                         // Calculate new minute hand x position
      omy = my * 44 + 65;                         // Calculate new minute hand y position
    }

    // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
    tft.drawLine(osx, osy, 65, 65, TFT_BLACK);  // Erase old second hand position
    tft.drawLine(ohx, ohy, 65, 65, TFT_WHITE);  // Draw new hour hand position
    tft.drawLine(omx, omy, 65, 65, TFT_WHITE);  // Draw new minute hand position
    osx = sx * 47 + 65;                         // Calculate new second hand x position
    osy = sy * 47 + 65;                         // Calculate new second hand y position
    tft.drawLine(osx, osy, 65, 65, TFT_RED);    // Draw new second hand position

    tft.fillCircle(65, 65, 3, TFT_RED);  // Draw the center dot
  }
}