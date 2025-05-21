#define BATTERY_MIN 3200
#define BATTERY_MAX 4200

const uint8_t BATTERY_PIN = 20;

void setup() {
  // Setup code to run once:
  Serial.begin(115200);          // Set baud rate to 115200
  pinMode(BATTERY_PIN, INPUT);   // Set battery voltage detection pin to input mode
  analogReadResolution(12);      // Set ADC resolution to 12 bits
  analogSetAttenuation(ADC_11db);// Set attenuation to 11dB
}

void loop() {
  // Main code to run repeatedly:
  battery_read();
}

void battery_read() {
  int total_value = 0, valid_time = 0, adc_value = 0, battery_volts = 0;
  // Sample 20 times for filtering
  for(int i = 0; i < 20; i++) {
    // Read ADC value
    adc_value = analogReadMilliVolts(BATTERY_PIN);
    // Calculate battery voltage using voltage divider (unit: mV)
    battery_volts = (adc_value * 2.5) - 3300;
    // Filter out abnormal values
    if(battery_volts > BATTERY_MIN && battery_volts < BATTERY_MAX) {
      total_value += battery_volts;
      valid_time++;
    }
    delay(50);
  }
  // Calculate average of valid measurements
  if(valid_time != 0)
    battery_volts = total_value / valid_time;

  // Output results
  Serial.printf("valid time: %d, battery volts: %.2fV\n", valid_time, battery_volts / 1000.0);
}