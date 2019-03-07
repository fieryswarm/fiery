/* 
 *   Feinzimer, David
 *   Parks, Timothy
 *   P, Evan
 *   Y, Zach
 *   
 *   General Notes:
 *       Enable pins:
 *           Value of HIGH allows moto
 *           Value of LOW disables motor
 *       Channel pins:
 *           Value of power applied to motor
 */


/*
 *   Front ultrsonic (fu) configuration
 */  
const int fu_trig = 15;              
const int fu_echo = 2;                 
const int fu_freq = 5000;                  
const int fu_resolution = 8;               
const unsigned int fu_max_dist = 23200;    

                                        
/*
 *   Turn motor (tm) configuration
 */
int tm_pwm = 14;  // adc2_channel 8
int tm_enable  = 27;                  
int tm_right_left = 12; // Right = 1, Left = 0            
const int tm_channel = 8;              


/*
 *   Drive motor (dm) configuration
 */
int dm_forward_reverse = 26;
int dm_PWM = 13;   // adc1_channel 5
int dm_enable = 25;
const int dm_channel = 5;

void setup() {

  /*
   *   Setup serial output
   */ 
  Serial.begin(9600);

  /*
   *   Setup front ultrsonic (fu)
   *   
   *   The trigger pin will tell the sensor to range find
   */ 
  pinMode(fu_trig, OUTPUT);
  digitalWrite(fu_trig, LOW);

  /*
   *   Enable motor channels for output
   */ 
  pinMode(tm_enable, OUTPUT);
  pinMode(tm_right_left, OUTPUT);
  pinMode(tm_pwm, OUTPUT);
  pinMode(dm_enable, OUTPUT);
  pinMode(dm_forward_reverse, OUTPUT);
  pinMode(dm_PWM, OUTPUT);
  
  digitalWrite(tm_enable, HIGH);
  digitalWrite(dm_enable,  HIGH);
  digitalWrite(tm_right_left, LOW);
  digitalWrite(dm_forward_reverse, LOW);
  
  
  
  ledcSetup(dm_channel, fu_freq, fu_resolution);
  ledcSetup(tm_channel, fu_freq, fu_resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(tm_pwm, tm_channel);
  ledcAttachPin(dm_PWM, dm_channel);
  
}

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(fu_trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(fu_trig, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(fu_echo) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(fu_echo) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > fu_max_dist ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }

  ledcWrite(dm_channel, 80);

  if (inches < 12) {
    ledcWrite(tm_channel, 200);
  }
  else {
    ledcWrite(tm_channel, 1);
  }

}
