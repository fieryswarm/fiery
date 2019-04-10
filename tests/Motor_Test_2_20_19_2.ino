#include "Arduino.h"

int turn_PWM = 14;
int turn_enable  = 27;
int turn_right1_left0 = 12;

int drive_forward_reverse = 26;
int drive_PWM = 25;
int drive_enable = 33;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  pinMode(turn_enable, OUTPUT);
  pinMode(drive_enable, OUTPUT);
  
  digitalWrite(turn_enable, HIGH);
  digitalWrite(drive_enable,  HIGH);
  
  pinMode(turn_right1_left0, OUTPUT);
  pinMode(drive_forward_reverse, OUTPUT);
  
  digitalWrite(turn_right1_left0, LOW);
  digitalWrite(drive_forward_reverse, LOW);
  
  pinMode(turn_PWM, OUTPUT);
  pinMode(drive_PWM, OUTPUT);

  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(turn_PWM, ledChannel);
  ledcAttachPin(drive_PWM, ledChannel);  
 
}
 
void loop(){
  ledcWrite(ledChannel, 255);
}
