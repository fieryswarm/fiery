
/*
 Stepper Motor Control - speed control

 This program drives a unipolar or bipolar stepper motor.
 The motor is attached to digital pins 8 - 11 of the Arduino.
 A potentiometer is connected to analog input 0.

 The motor will rotate in a clockwise direction. The higher the potentiometer value,
 the faster the motor speed. Because setSpeed() sets the delay between steps,
 you may notice the motor is less responsive to changes in the sensor value at
 low speeds.

 Created: 11.30.09
 Original Author: Tom Igoe

Last Update: 2.12.19.12.17
Modifiers: David Feinzimer, Tim Parks

 */

#include <Stepper.h>

const int stepsPerRevolution = 360;  // change this to fit the number of steps per revolution for your motor

// initialize the stepper library on pins 8 through 11:
Stepper minute_hand(stepsPerRevolution, 25, 26, 27, 14);

int stepCount = 0;  // number of steps the motor has taken

void setup() {
  // nothing to do inside the setup
}

void loop() {
  // read the sensor value:
  //int sensorReading = analogRead(A0);
  int sensorReading = 62;
  // map it to a range from 0 to 100:
  int motorSpeed = map(sensorReading, 0, 1023, 0, 100);
  // set the motor speed:
  if (motorSpeed > 0) {
    minute_hand.setSpeed(motorSpeed);
    // step 1/100 of a revolution:
    minute_hand.step(stepsPerRevolution / 100);
  }
}
