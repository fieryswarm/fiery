// PWM properties
const int freq = 5000;
const int resolution = 8;

// Turning motor properties
int turn_PWM = 14;  // adc2_channel 8
int turn_enable  = 27;
int turn_direction = 12;
const int turnChannel = 8;

// Driving motor properties
int drive_direction = 26;
int drive_PWM = 13;   // adc1_channel 5
int drive_enable = 25;
const int driveChannel = 5;

// defines variables
long duration;
int distance;
int velocity = 0;

void setup() {
    Serial.begin(115200);

    pinMode(turn_enable, OUTPUT);
    pinMode(turn_PWM, OUTPUT);
    pinMode(turn_direction, OUTPUT);

    pinMode(drive_enable, OUTPUT);
    pinMode(drive_PWM, OUTPUT);
    pinMode(drive_direction, OUTPUT);

    ledcSetup(driveChannel, freq, resolution);
    ledcSetup(turnChannel, freq, resolution);

    // Attach the channel to the GPIO to be controlled
    ledcAttachPin(turn_PWM, turnChannel);
    ledcAttachPin(drive_PWM, driveChannel);

    // Enable all motors
    digitalWrite(turn_enable, HIGH);
    digitalWrite(drive_enable,  HIGH);
}

void turnaround() {
  Serial.println("Start in a stopped state");
  ledcWrite(driveChannel, 3);
  ledcWrite(turnChannel, 3);

  Serial.println("Prepare wheels for right turn");
  digitalWrite(turn_direction, LOW);    // Right:     L   Left:     H

  Serial.println("Prepare wheels for reverse");
  digitalWrite(drive_direction, HIGH);   // Forward:   L   Reverse:  H

  Serial.println("Apply power to turning motors");
  ledcWrite(turnChannel, 200);

  Serial.println("Run turning motors for 1 second");
  delay(1000);

  Serial.println("Apply power to drive motors");
  ledcWrite(driveChannel, 200);

  Serial.println("Run all motors for 1.5 seconds");
  delay(1500);

  Serial.println("Kill all motors");
  ledcWrite(driveChannel, 3);
  ledcWrite(turnChannel, 3);

  Serial.println("Prepare wheels for left turn");
  digitalWrite(turn_direction, HIGH);    // Right:     L   Left:     H

  Serial.println("Prepare wheels for forward");
  digitalWrite(drive_direction, LOW);   // Forward:   L   Reverse:  H

  Serial.println("Apply power to turning motors");
  ledcWrite(turnChannel, 200);

  Serial.println("Run turning motors for 1 second");
  delay(1000);

  Serial.println("Apply power to drive motors");
  ledcWrite(driveChannel, 200);

  Serial.println("Run all motors for 1.5 seconds");
  delay(1500);

  Serial.println("Kill all motors");
  ledcWrite(driveChannel, 3);
  ledcWrite(turnChannel, 3);
}

void loop() {
  Serial.println("turnaround()");
  turnaround();

  Serial.println("Wait 10 seconds for next run");
  delay(10000);
}
