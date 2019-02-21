// PWM 14

// Duty cycle at 110

#include "Arduino.h"

int ledPin = 14;    // LED connected to digital pin 9
int enable  = 27;
int forward_reverse = 12;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

void setup() {
  pinMode(enable, OUTPUT);
  digitalWrite(enable, HIGH);
  pinMode(forward_reverse, OUTPUT);
  digitalWrite(forward_reverse, HIGH);
  pinMode(ledPin, OUTPUT);
    // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  Serial.begin(9600);
 
}


 
void loop(){
  // increase the LED brightness
  /*for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);
    delay(100);
    Serial.print("duty: ");
    Serial.println(dutyCycle);
  }
  

  // decrease the LED brightness
  for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){
    // changing the LED brightness with PWM
    ledcWrite(ledChannel, dutyCycle);   
    delay(100);
  }
  */
  ledcWrite(ledChannel, 105);
}
