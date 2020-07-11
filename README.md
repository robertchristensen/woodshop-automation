# woodshop-automation

Contact: Robert Christensen (robert.christensen@gmail.com)

Woodshop automation project using arduino.  The arduino is connected to current sensors,
which are attached to machines in a common woodshop.  When a current sensor detects current
flowing to the machine, it will activate a servo motor to open a dust pipe connected to the
machine.  The dust collector activates and will stay active while it detects the machine is
active.

A distance sensor detects if the dust bin is full.  In this case, the system will not activate.

## Parts list

| item                                        | description |
|---------------------------------------------|-------------|
| Arduino Mega 2560                           | microcontroller |
| Adafruit 16-Channel 12-bit PWM/Servo Driver | Servo motor controller |
| MG995 Servo                                 | Servo motor to control blast gate |
| ACS712 (20A)                                | Current sensor |
| SSR-40A                                     | Solid State relay to control dust collector |
| neopixel                                    | Status light |
