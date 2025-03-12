#include "motor_sensor_module.h"
#include <Adafruit_NeoPixel.h>

//const int LED_PIN = 13;
//const int LED_COUNT = 1;
#define LED_PIN 13
#define LED_COUNT 1
const int LED_BRIGHTNESS = 1; // brightness between 1-255

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int RELAY_PIN = 2;
const int DISTANCE_PIN = 11;

// time (milliseconds) to wait for the gate to open or close
const int GATE_DELAY_TIME = 1000;
// time (milliseconds) to wait for the dust collector to turn on/off
const int COLLECTOR_DELAY_TIME = 4000;
// time (milliseconds) to wait after the machine turns off before shutting the gate
const int CLEAR_DELAY_TIME = 5000;

void setup() {
  Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);

  strip.begin();
  strip.show();
  strip.setBrightness(48);
  strip.setPixelColor(0, 0, 0, 0);
  strip.show();

  setup_ms_module();
  close_all_gates();
  delay(GATE_DELAY_TIME);
  sleep_motors();
}

void loop() {
  // READ ALL SENSORS
  int sensor_id = read_all_sensors();

  if(sensor_id >= 0 && verify_sensor(sensor_id)){
    Serial.print("SENSOR ");
    Serial.print(sensor_id);
    Serial.println(" TRIGGERED");    
    // The distance sensor will be 0 if something is detected.
    if(digitalRead(DISTANCE_PIN) == 0) {
      Serial.println("DISTANCE SENSOR BLOCKING");
      strip.setPixelColor(0, 255, 0, 0);
      strip.setBrightness(255);
      strip.show();
      delay(500);
      return;
    }
    strip.setPixelColor(0, 0, 0, 255);
    strip.setBrightness(64);
    strip.show();
    // if it is running, change LED to blue
    // - READ DISTANCE SENSOR
    // * IF DISTANCE IS HIGH,
    wakeup_motors();
    open_gate(sensor_id);
    delay(GATE_DELAY_TIME);
    sleep_motors();
    digitalWrite(RELAY_PIN, HIGH);
    unsigned long last_triggered = millis();
  
    while(millis() - last_triggered < CLEAR_DELAY_TIME) {
      if(read_sensor(sensor_id))
        last_triggered = millis();
    }
    
    digitalWrite(RELAY_PIN, LOW);
    delay(COLLECTOR_DELAY_TIME);
    wakeup_motors();
    close_gate(sensor_id);
    delay(GATE_DELAY_TIME);
    sleep_motors();
  }
  // if everything is running good, change color to green.
  strip.setPixelColor(0, 0, 255, 0);
  strip.setBrightness(12);
  strip.show();
}
