const int TRIG_PIN = 15;
const int ECHO_PIN = 2;
const int freq = 5000;
const int resolution = 8;
//const int turnChannel = 1;
//const int ledChannel = 0;
const unsigned int MAX_DIST = 23200;

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

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);

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

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }

  ledcWrite(ledChannel, 80);

  if (inches < 12) {
    ledcWrite(turnChannel, 200);
  }
  else {
    ledcWrite(turnChannel, 10);
  }

  
  /*
   delay(3000);
  
  delay(2000);
  digitalWrite(turn_enable, LOW);
  
  //ledcWrite(turnChannel, 0);
  delay(3000);
  digitalWrite(drive_enable,  LOW);

  */
}
const int TRIG_PIN = 15;
const int ECHO_PIN = 2;
const int freq = 5000;
const int resolution = 8;
//const int turnChannel = 1;
//const int ledChannel = 0;
const unsigned int MAX_DIST = 23200;

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

  // The Trigger pin will tell the sensor to range find
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);

  // We'll use the serial monitor to view the sensor output
  Serial.begin(9600);

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

void loop() {

  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  // Hold the trigger pin high for at least 10 us
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  // Wait for pulse on echo pin
  while ( digitalRead(ECHO_PIN) == 0 );

  // Measure how long the echo pin was held high (pulse width)
  // Note: the micros() counter will overflow after ~70 min
  t1 = micros();
  while ( digitalRead(ECHO_PIN) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  // Calculate distance in centimeters and inches. The constants
  // are found in the datasheet, and calculated from the assumed speed
  //of sound in air at sea level (~340 m/s).
  cm = pulse_width / 58.0;
  inches = pulse_width / 148.0;

  // Print out results
  if ( pulse_width > MAX_DIST ) {
    Serial.println("Out of range");
  } else {
    Serial.print(cm);
    Serial.print(" cm \t");
    Serial.print(inches);
    Serial.println(" in");
  }

  ledcWrite(ledChannel, 80);

  if (inches < 12) {
    ledcWrite(turnChannel, 200);
  }
  else {
    ledcWrite(turnChannel, 10);
  }

  
  /*
   delay(3000);
  
  delay(2000);
  digitalWrite(turn_enable, LOW);
  
  //ledcWrite(turnChannel, 0);
  delay(3000);
  digitalWrite(drive_enable,  LOW);

  */
}

