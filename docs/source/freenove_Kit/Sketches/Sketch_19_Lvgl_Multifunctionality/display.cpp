#include "display.h"

#define BUTTON_PIN 19

lv_indev_t *indev_keypad; // External declaration of the keypad input device

// Define screen dimensions
static const uint16_t screenWidth = 135;
static const uint16_t screenHeight = 240;

// Buffer for drawing
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

// TFT instance
TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

// Button instance
Button button(BUTTON_PIN);

// Display instance
Display display;

#if LV_USE_LOG != 0
/* Serial debugging */
void my_print(const char *buf)
{
  Serial.printf(buf); // Print the buffer to the serial monitor
  Serial.flush();     // Ensure all data is sent
}
#endif

// Display flush function
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
  uint32_t w = (area->x2 - area->x1 + 1); // Calculate width of the area to flush
  uint32_t h = (area->y2 - area->y1 + 1); // Calculate height of the area to flush

  tft.startWrite();                                        // Start writing to the TFT
  tft.setAddrWindow(area->x1, area->y1, w, h);             // Set the address window for writing
  tft.pushColors((uint16_t *)&color_p->full, w * h, true); // Push colors to the TFT
  tft.endWrite();                                          // End writing to the TFT
  lv_disp_flush_ready(disp);                               // Inform LVGL that flushing is complete
}

// Keypad read function
void my_keypad_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
{
  static int last_key = 0; // Static variable to store the last key value

  button.key_scan();                           // Scan the button state
  int buttonState = button.get_button_state(); // Get the current button state
  int act_key = button.get_button_key_value(); // Get the current button key value

  // Update the state based on the button state
  switch (buttonState)
  {
  case Button::KEY_STATE_PRESSED:
    data->state = LV_INDEV_STATE_PR; // Button is pressed
    break;
  case Button::KEY_STATE_RELEASED:
    data->state = LV_INDEV_STATE_REL; // Button is released
    break;
  }

  // Map button key values to LVGL key codes
  switch (act_key)
  {
  case 1:
    act_key = LV_KEY_ENTER; // Map to Enter key
    break;
  case 2:
    if (display.getTftShowDirection() == 0)
      act_key = LV_KEY_PREV;
    else if (display.getTftShowDirection() == 1)
      act_key = LV_KEY_LEFT;
    else if (display.getTftShowDirection() == 2)
      act_key = LV_KEY_NEXT;
    else if (display.getTftShowDirection() == 3)
      act_key = LV_KEY_RIGHT;
    break;
  case 3:
    if (display.getTftShowDirection() == 0)
      act_key = LV_KEY_NEXT;
    else if (display.getTftShowDirection() == 1)
      act_key = LV_KEY_RIGHT;
    else if (display.getTftShowDirection() == 2)
      act_key = LV_KEY_PREV;
    else if (display.getTftShowDirection() == 3)
      act_key = LV_KEY_LEFT;
    break;
  case 4:
    if (display.getTftShowDirection() == 0)
      act_key = LV_KEY_LEFT;
    else if (display.getTftShowDirection() == 1)
      act_key = LV_KEY_NEXT;
    else if (display.getTftShowDirection() == 2)
      act_key = LV_KEY_RIGHT;
    else if (display.getTftShowDirection() == 3)
      act_key = LV_KEY_PREV;
    break;
  case 5:
    if (display.getTftShowDirection() == 0)
      act_key = LV_KEY_RIGHT;
    else if (display.getTftShowDirection() == 1)
      act_key = LV_KEY_PREV;
    else if (display.getTftShowDirection() == 2)
      act_key = LV_KEY_LEFT;
    else if (display.getTftShowDirection() == 3)
      act_key = LV_KEY_NEXT;
    break;
  default:
    break;
  }
  last_key = act_key;   // Update the last key value
  data->key = last_key; // Set the key value in the input device data
}

void tftRst(void) {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, LOW);
  delay(50);
  digitalWrite(TFT_BL, HIGH);
  delay(50);
}

// Setup the TFT display
void setupTFT(int direction)
{
  tftRst();
  display.setTftShowDirection(direction);
  tft.begin();                                    // Initialize the TFT
  tft.setRotation(display.getTftShowDirection()); // Set the rotation of the TFT using the tft_show_dirction macro
}

// Setup the button
void setupButton()
{
  button.init(); // Initialize the button
}

// Setup LVGL
void setupLVGL()
{
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print); // Register the print function for debugging
#endif
  lv_init(); // Initialize LVGL

  // Initialize the display buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, screenWidth * 10);

  // Initialize the display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  // Set the resolution based on the TFT direction
  switch (display.getTftShowDirection())
  {
  case 0: // Normal orientation
  case 2: // 180 degree rotation
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    break;
  case 1: // 90 degree rotation
  case 3: // 270 degree rotation
    disp_drv.hor_res = screenHeight;
    disp_drv.ver_res = screenWidth;
    break;
  default:
    disp_drv.hor_res = screenWidth;
    disp_drv.ver_res = screenHeight;
    break;
  }

  disp_drv.flush_cb = my_disp_flush; // Set the flush callback
  disp_drv.draw_buf = &draw_buf;     // Set the draw buffer
  lv_disp_drv_register(&disp_drv);   // Register the display driver

  // Initialize the input device driver
  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_KEYPAD;            // Set the input device type to keypad
  indev_drv.read_cb = my_keypad_read;               // Set the read callback
  indev_keypad = lv_indev_drv_register(&indev_drv); // Register the input device driver
}

// Initialize the display
void Display::init(int screenDir)
{
  setupTFT(screenDir); // Setup the TFT display
  setupButton();       // Setup the button
  setupLVGL();         // Setup LVGL
}

// Handle routine display tasks
void Display::routine(void)
{
  lv_task_handler(); // Handle LVGL tasks
}
