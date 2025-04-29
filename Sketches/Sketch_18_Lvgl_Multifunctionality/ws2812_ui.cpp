#include "ws2812_ui.h"

lvgl_ws2812_ui guider_ws2812_ui;                                                               // WS2812 UI structure
Freenove_ESP32_WS2812 strip = Freenove_ESP32_WS2812(LEDS_COUNT, LEDS_PIN, CHANNEL, TYPE_GRB);  // WS2812 LED strip initialization

// Initialize the WS2812 LED strip
bool ws2812_init(void) {
  strip.begin();           // Start the LED strip
  strip.setBrightness(1);  // Set initial brightness
  return 1;                // Return success
}

// Event callback for WS2812 screen
static void screen_ws2812_event_cb(lv_event_t *event)
{
  lv_event_code_t code = lv_event_get_code(event);
  if (code == LV_EVENT_KEY)
  {
      uint32_t key = lv_event_get_key(event);
      if (key == LV_KEY_ENTER)
      {
          Serial.println("WS2812 Btn Enter!"); // Debug output
      }
      else if(key == LV_KEY_LEFT || key == LV_KEY_RIGHT)
      {
        strip.setLedColorData(0, 0, 0, 0);
        strip.show();
        Serial.println("Jump to Main Screen!");
        if (!lv_obj_is_valid(guider_main_ui.main))
            setup_scr_main(&guider_main_ui);
        lv_scr_load(guider_main_ui.main);
        lv_obj_del(guider_ws2812_ui.ws2812);
      }
  }
  else if (code ==LV_EVENT_RELEASED)
  {
    Serial.println("WS2812 Btn Released!");
  }
}

// Event callback for slider changes
static void slider_event_cb(lv_event_t *e) {
  lv_event_code_t code = lv_event_get_code(e);
  static int slider_index = 0;
  static int slider_value[4] = { 20 };

  // Get slider values
  int color_red = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_red);
  int color_green = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_green);
  int color_blue = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_blue);
  int color_brightness = lv_slider_get_value(guider_ws2812_ui.ws2812_slider_brightness);

  // Update label values
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_lable_red_value, "%d", color_red);
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_lable_green_value, "%d", color_green);
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_lable_blue_value, "%d", color_blue);
  lv_label_set_text_fmt(guider_ws2812_ui.ws2812_lable_brightness_value, "%d", color_brightness);

  // Map slider values to LED color and brightness
  int red = map(color_red, 0, 50, 0, 255);
  int green = map(color_green, 0, 50, 0, 255);
  int blue = map(color_blue, 0, 50, 0, 255);
  int brightness = map(color_brightness, 0, 50, 0, 255);

  // Set LED strip color and brightness
  strip.setBrightness(brightness);
  strip.setLedColorData(0, red, green, blue);
  strip.show();  // Update the LED strip
}

// Create a slider with specified color
static lv_obj_t *create_slider(lvgl_ws2812_ui *ui, lv_color_t color) {
  lv_obj_t *slider = lv_slider_create(ui->ws2812);                             // Create a slider object
  lv_slider_set_range(slider, 0, 50);                                          // Set slider range from 0 to 50
  lv_obj_set_size(slider, 120, 10);                                            // Set slider size
  lv_obj_set_style_bg_color(slider, color, LV_PART_KNOB);                      // Set knob color
  lv_obj_set_style_bg_color(slider, color, LV_PART_INDICATOR);                 // Set indicator color
  lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);  // Add event callback for value changes
  return slider;                                                               // Return the created slider
}

// Create a label
static lv_obj_t *create_label(lvgl_ws2812_ui *ui) {
  lv_obj_t *label = lv_label_create(ui->ws2812);  // Create a label object
  lv_obj_set_size(label, 80, 20);                 // Set label size
  return label;                                   // Return the created label
}

// Create a label for displaying slider values
static lv_obj_t *create_label_value(lvgl_ws2812_ui *ui) {
  lv_obj_t *label = lv_label_create(ui->ws2812);  // Create a label object
  lv_obj_set_size(label, 20, 20);                 // Set label size
  return label;                                   // Return the created label
}

// Parameter configuration function on the WS2812 screen
void setup_scr_ws2812(lvgl_ws2812_ui *ui) {
  ws2812_init();  // Initialize the WS2812 LED strip

  // Write codes picture
  ui->ws2812 = lv_obj_create(NULL);  // Create the main container for the WS2812 UI
  static lv_style_t bg_style;
  lv_style_init(&bg_style);                                  // Initialize background style
  lv_style_set_bg_color(&bg_style, lv_color_hex(0xffffff));  // Set background color to white
  lv_obj_add_style(ui->ws2812, &bg_style, LV_PART_MAIN);     // Apply background style to the main container

  // Create sliders for red, green, blue, and brightness
  ui->ws2812_slider_red = create_slider(ui, lv_palette_main(LV_PALETTE_RED));
  ui->ws2812_slider_green = create_slider(ui, lv_palette_main(LV_PALETTE_GREEN));
  ui->ws2812_slider_blue = create_slider(ui, lv_palette_main(LV_PALETTE_BLUE));
  ui->ws2812_slider_brightness = create_slider(ui, lv_palette_main(LV_PALETTE_GREY));

  // Set initial slider values
  lv_slider_set_value(ui->ws2812_slider_red, 10, LV_ANIM_OFF);
  lv_slider_set_value(ui->ws2812_slider_green, 0, LV_ANIM_OFF);
  lv_slider_set_value(ui->ws2812_slider_blue, 0, LV_ANIM_OFF);
  lv_slider_set_value(ui->ws2812_slider_brightness, 10, LV_ANIM_OFF);

  // Create labels for sliders
  ui->ws2812_lable_red = create_label(ui);
  ui->ws2812_lable_green = create_label(ui);
  ui->ws2812_lable_blue = create_label(ui);
  ui->ws2812_lable_brightness = create_label(ui);

  // Create labels for slider values
  ui->ws2812_lable_red_value = create_label_value(ui);
  ui->ws2812_lable_green_value = create_label_value(ui);
  ui->ws2812_lable_blue_value = create_label_value(ui);
  ui->ws2812_lable_brightness_value = create_label_value(ui);

  // Align sliders
  lv_obj_align(ui->ws2812_slider_red, LV_ALIGN_LEFT_MID, 90, -48);
  lv_obj_align_to(ui->ws2812_slider_green, ui->ws2812_slider_red, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
  lv_obj_align_to(ui->ws2812_slider_blue, ui->ws2812_slider_green, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);
  lv_obj_align_to(ui->ws2812_slider_brightness, ui->ws2812_slider_blue, LV_ALIGN_OUT_BOTTOM_MID, 0, 20);

  // Set label texts
  lv_label_set_text(ui->ws2812_lable_red, "Red");
  lv_label_set_text(ui->ws2812_lable_green, "Green");
  lv_label_set_text(ui->ws2812_lable_blue, "Blue");
  lv_label_set_text(ui->ws2812_lable_brightness, "Brightness");

  // Align labels to sliders
  lv_obj_align_to(ui->ws2812_lable_red, ui->ws2812_slider_red, LV_ALIGN_OUT_LEFT_MID, -8, 2);
  lv_obj_align_to(ui->ws2812_lable_green, ui->ws2812_slider_green, LV_ALIGN_OUT_LEFT_MID, -8, 2);
  lv_obj_align_to(ui->ws2812_lable_blue, ui->ws2812_slider_blue, LV_ALIGN_OUT_LEFT_MID, -8, 2);
  lv_obj_align_to(ui->ws2812_lable_brightness, ui->ws2812_slider_brightness, LV_ALIGN_OUT_LEFT_MID, -8, 2);

  // Align value labels to sliders
  lv_obj_align_to(ui->ws2812_lable_red_value, ui->ws2812_slider_red, LV_ALIGN_OUT_RIGHT_MID, 10, 2);
  lv_obj_align_to(ui->ws2812_lable_green_value, ui->ws2812_slider_green, LV_ALIGN_OUT_RIGHT_MID, 10, 2);
  lv_obj_align_to(ui->ws2812_lable_blue_value, ui->ws2812_slider_blue, LV_ALIGN_OUT_RIGHT_MID, 10, 2);
  lv_obj_align_to(ui->ws2812_lable_brightness_value, ui->ws2812_slider_brightness, LV_ALIGN_OUT_RIGHT_MID, 10, 2);

  // Create a group for managing sliders
  lv_group_t *group = lv_group_create();
  lv_group_add_obj(group, ui->ws2812_slider_red);
  lv_group_add_obj(group, ui->ws2812_slider_green);
  lv_group_add_obj(group, ui->ws2812_slider_blue);
  lv_group_add_obj(group, ui->ws2812_slider_brightness);
  lv_group_add_obj(group, ui->ws2812);
  lv_group_set_editing(group, true);
  lv_indev_set_group(indev_keypad, group);

  // Send an initial value change event to update labels and LED strip
  lv_event_send(ui->ws2812_slider_brightness, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ui->ws2812, screen_ws2812_event_cb, LV_EVENT_ALL, NULL);
}