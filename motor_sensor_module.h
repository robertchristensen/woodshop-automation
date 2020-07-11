#include <Adafruit_PWMServoDriver.h>

/* Servo calibrations.  Default values are 
 *  MS_SERVO_CLOSED = 150
 *  MS_SERVO_OPEN = 600
 *  
 *  MS_SERVO_CLOSED is the pulse given to the motor when the gate is closed
 *  MS_SERVO_OPEN is the pulse given to the motor when the gate is open.
 *  
 *  These vales can be changed to customize the angle of the motor when the 
 *  gates are open or closed.  Be carefulu not to have the angle too large,
 *  or the servo might be damaged.
 */
//const int MS_SERVO_CLOSED = 150;
//const int MS_SERVO_OPEN = 600;
const int MS_SERVO_CLOSED = 375-100;
const int MS_SERVO_OPEN = 375+25;

const int MS_SENSOR_ADJUSTMENT = 512;

const int MS_SENSOR_TRIGGER = 50;

const int MS_ANALOG_PIN_COUNT = 16;
const int MS_MOTOR_COUNT = 16;

// After the sensor triggers, we read the sensor for this number
// of milliseconds to make sure it triggers at least one more time
const int MS_DOUBLE_TRIGGER_MS = 20;

Adafruit_PWMServoDriver pwm_controller;

// run this to setup motor controller
void setup_ms_module(){
  pwm_controller.begin();
  pwm_controller.setPWMFreq(60);
}

// For each sensor i listed in m_analog_pin_map[i], if the sensor triggers turn on the motor m_analog_motor_map[i].
// This can be used to make it so several sensors control the same motor, or a single sensor can control multiple motors.
int m_analog_motor_map[MS_ANALOG_PIN_COUNT] = {0,   1,  2,  3,  4,  5, 6,   7,  9,  9,  11,  11,  12,  13,  14,  15};
int m_analog_pin_map[MS_ANALOG_PIN_COUNT]   = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};

// read an individual sensor.
// return true if the sensor triggered
// return false otherwise
bool read_sensor(int sensor_id) {
  int sensor_value = analogRead(m_analog_pin_map[sensor_id]);
  //Serial.println(sensor_value);
  return sensor_value > (MS_SENSOR_ADJUSTMENT + MS_SENSOR_TRIGGER);
}

// scan all sensors.
// return the sensor index that triggered
// return -1 if no sensor triggered
int read_all_sensors(){
  for(int i=0; i<MS_ANALOG_PIN_COUNT; ++i) {
    if(read_sensor(i)) {
      return i;
    }
  }
  return -1;
}

// check this sensor triggers within the time limit
bool verify_sensor(int sensor_id) {
    unsigned long last_triggered = millis();
  
    while(millis() - last_triggered < MS_DOUBLE_TRIGGER_MS) {
      if(read_sensor(sensor_id))
        return true;
    }
    return false;
}

void sleep_motors() {
  pwm_controller.sleep();
}

void wakeup_motors() {
  pwm_controller.wakeup();
}

// activate the motor associated with a specified sensor_id
void open_gate(int sensor_id) {
  pwm_controller.setPWM(m_analog_motor_map[sensor_id], 0, MS_SERVO_OPEN);
}

void close_gate(int sensor_id) {
  pwm_controller.setPWM(m_analog_motor_map[sensor_id], 0, MS_SERVO_CLOSED);
}

void close_all_gates() {
  for(int i=0; i<MS_ANALOG_PIN_COUNT; i++){
    close_gate(i);
  }
}
