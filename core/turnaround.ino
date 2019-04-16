// PWM properties
const int freq = 5000;
const int resolution = 8;

// Turning motor properties
int turn_PWM = 14;  // adc2_channel 8
int turn_enable  = 27;
int turn_right1_left0 = 12;
const int turnChannel = 8;

// Driving motor properties
int drive_forward_reverse = 26;
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
    pinMode(turn_right1_left0, OUTPUT);

    pinMode(drive_enable, OUTPUT);
    pinMode(drive_PWM, OUTPUT);
    pinMode(drive_forward_reverse, OUTPUT);

    digitalWrite(turn_enable, HIGH);
    digitalWrite(drive_enable,  HIGH); //

    digitalWrite(turn_right1_left0, LOW);
    digitalWrite(drive_forward_reverse, LOW);


    ledcSetup(driveChannel, freq, resolution);
    ledcSetup(turnChannel, freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(turn_PWM, turnChannel);
    ledcAttachPin(drive_PWM, driveChannel);
}


void loop() {
  ledcWrite(driveChannel, 120);
  delay(10);
}
