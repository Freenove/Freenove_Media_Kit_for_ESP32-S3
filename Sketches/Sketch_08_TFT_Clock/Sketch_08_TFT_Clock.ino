/*
* Sketch_08_TFT_Clock.ino
* This sketch displays a clock on a TFT display using the TFT_eSPI library.
* It updates the time every second and draws hour, minute, and second hands.
* 
* Author: Zhentao Lin
* Date:   2025-04-07
*/

#include <TFT_eSPI.h>  // Graphics and font library for ST7735 driver chip
#include <SPI.h>

#ifdef FNK0102A_1P14_135x240_ST7789
  int screenWidth = 135;
  int screenHeight = 240;
#elif defined FNK0102B_3P5_320x480_ST7796
  int screenWidth = 320;
  int screenHeight = 480;
#endif

#define TFT_BL 20
#define TFT_DIRECTION 1  // Define the direction of the TFT display (0, 1, 2, or 3)

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

#define TFT_GREY 0xBDF7  // Define a custom grey color for the background

// Display dimensions after rotation
int displayWidth, displayHeight;
// Calculate positions for clock and text
int clockCenterX, clockCenterY;
int textX, textY;
// Adjust clock radius based on the smaller screen dimension
int clockRadius;

// Variables to store the positions and angles of the clock hands
float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;                // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;                                   // Degrees for second, minute, and hour hands
uint16_t osx, osy, omx, omy, ohx, ohy;  // Saved H, M, S x & y coords
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

// Initialize coordinates based on screen layout
void initializeDisplayParameters() {
  // After rotation, width and height are swapped
  if (TFT_DIRECTION == 1 || TFT_DIRECTION == 3) {
    displayWidth = screenHeight;
    displayHeight = screenWidth;
  } else {
    displayWidth = screenWidth;
    displayHeight = screenHeight;
  }
  
  // 根据屏幕尺寸调整布局比例
  float layoutRatio = 0.0;
  if (displayWidth >= 320) {
    // 3.5寸屏幕，使用较小的布局比例
    layoutRatio = 0.35;
  } else {
    // 1.14寸屏幕，使用较大的布局比例
    layoutRatio = 0.45;
  }
  
  // 将时钟放在左侧区域的中心
  clockCenterX = displayWidth * 0.3;  // 30%的位置
  clockCenterY = displayHeight / 2;
  
  // 将文字放在最右侧区域，确保完全超出时钟范围
  textX = displayWidth * 0.8;  // 80%的位置，更靠右
  textY = displayHeight / 2;
  
  // 根据屏幕尺寸调整时钟半径
  clockRadius = min(displayWidth, displayHeight) * layoutRatio;
  
  // Initialize hand coordinates
  osx = clockCenterX;
  osy = clockCenterY;
  omx = clockCenterX;
  omy = clockCenterY;
  ohx = clockCenterX;
  ohy = clockCenterY;
}

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
  
  initializeDisplayParameters(); // Initialize coordinates

  // Draw the clock face with dynamic sizing
  tft.fillCircle(clockCenterX, clockCenterY, clockRadius, TFT_BLUE);
  tft.fillCircle(clockCenterX, clockCenterY, clockRadius - 4, TFT_BLACK);

  // Draw 12 lines for the clock marks
  for (int i = 0; i < 360; i += 30) {
    float angle = (i - 90) * 0.0174532925;
    sx = cos(angle);
    sy = sin(angle);
    x0 = sx * (clockRadius - 4) + clockCenterX;
    yy0 = sy * (clockRadius - 4) + clockCenterY;
    x1 = sx * (clockRadius - 11) + clockCenterX;
    yy1 = sy * (clockRadius - 11) + clockCenterY;

    tft.drawLine(x0, yy0, x1, yy1, TFT_BLUE);
  }

  // Draw 60 dots for the clock marks
  for (int i = 0; i < 360; i += 6) {
    float angle = (i - 90) * 0.0174532925;
    sx = cos(angle);
    sy = sin(angle);
    x0 = sx * (clockRadius - 8) + clockCenterX;
    yy0 = sy * (clockRadius - 8) + clockCenterY;

    tft.drawPixel(x0, yy0, TFT_BLUE);
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

  tft.fillCircle(clockCenterX, clockCenterY, 3, TFT_RED);

  // Draw text on the right side of the screen
  tft.setTextColor(TFT_GREEN);  // Set text color
  tft.drawCentreString("Freenove", textX, textY, 4);

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

    // Calculate hand lengths proportional to clock size
    int hourHandLength = clockRadius * 0.5;
    int minuteHandLength = clockRadius * 0.7;
    int secondHandLength = clockRadius * 0.75;

    if (ss == 0 || initial) {
      initial = 0;  // Reset initial flag
      // Erase hour and minute hand positions every minute
      tft.drawLine(ohx, ohy, clockCenterX, clockCenterY, TFT_BLACK);  // Erase old hour hand position
      ohx = hx * hourHandLength + clockCenterX;                       // Calculate new hour hand x position
      ohy = hy * hourHandLength + clockCenterY;                       // Calculate new hour hand y position
      tft.drawLine(omx, omy, clockCenterX, clockCenterY, TFT_BLACK);  // Erase old minute hand position
      omx = mx * minuteHandLength + clockCenterX;                     // Calculate new minute hand x position
      omy = my * minuteHandLength + clockCenterY;                     // Calculate new minute hand y position
    }

    // Redraw new hand positions, hour and minute hands not erased here to avoid flicker
    tft.drawLine(osx, osy, clockCenterX, clockCenterY, TFT_BLACK);    // Erase old second hand position
    tft.drawLine(ohx, ohy, clockCenterX, clockCenterY, TFT_WHITE);    // Draw new hour hand position
    tft.drawLine(omx, omy, clockCenterX, clockCenterY, TFT_WHITE);    // Draw new minute hand position
    osx = sx * secondHandLength + clockCenterX;                       // Calculate new second hand x position
    osy = sy * secondHandLength + clockCenterY;                       // Calculate new second hand y position
    tft.drawLine(osx, osy, clockCenterX, clockCenterY, TFT_RED);      // Draw new second hand position

    tft.fillCircle(clockCenterX, clockCenterY, 3, TFT_RED);  // Draw the center dot
  }
}