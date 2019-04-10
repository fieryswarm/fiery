#include "Arduino.h"

// setting PWM properties
const int freq = 5000;
const int resolution = 8;
//const int turnChannel = 1;
//const int ledChannel = 0;


// Turning properties
int turn_PWM = 14;  // adc2_channel 8
int turn_enable  = 27;
int turn_right1_left0 = 12;
const int turnChannel = 8;

// Driving properties
int drive_forward_reverse = 26;
int drive_PWM = 13;   // adc1_channel 5
int drive_enable = 25;
const int ledChannel = 5;


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
  ledcSetup(turnChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(turn_PWM, turnChannel);
  ledcAttachPin(drive_PWM, ledChannel);  
 
}
 
void loop(){
  ledcWrite(ledChannel, 120);
  delay(3000);
  ledcWrite(turnChannel, 200);
  delay(2000);
  digitalWrite(turn_enable, LOW);
  
  //ledcWrite(turnChannel, 0);
  delay(3000);
  digitalWrite(drive_enable,  LOW);
}
