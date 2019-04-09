#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>

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

void connectAWSIoT();
void mqttCallback (char* topic, byte* payload, unsigned int length);

char *ssid = "";
char *password = "";

const char *endpoint = "";
const int port = 8883;
char *pubTopic = "";
char *subTopic = "";

const char* rootCA = "";

const char* certificate = "";

const char* privateKey = "";

WiFiClientSecure httpsClient;
PubSubClient mqttClient(httpsClient);

void setup() {
   delay(1000);
   Serial.begin(115200);

   pinMode(turn_enable, OUTPUT);
   pinMode(drive_enable, OUTPUT);

   digitalWrite(turn_enable, HIGH);
   digitalWrite(drive_enable,  LOW);

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



    // Start WiFi
    Serial.println("Connecting to ");
    Serial.print(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected.");

    // Configure MQTT Client
    httpsClient.setCACert(rootCA);
    httpsClient.setCertificate(certificate);
    httpsClient.setPrivateKey(privateKey);
    mqttClient.setServer(endpoint, port);
    mqttClient.setCallback(mqttCallback);

    connectAWSIoT();


    
}

void connectAWSIoT() {
    while (!mqttClient.connected()) {
        if (mqttClient.connect("ESP32_device")) {
            Serial.println("Connected.");
            int qos = 0;
            mqttClient.subscribe(subTopic, qos);
            Serial.println("Subscribed.");
        } else {
            Serial.print("Failed. Error state=");
            Serial.print(mqttClient.state());
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}

long messageSentAt = 0;
int dummyValue = 0;
char pubMessage[128];

void mqttLoop() {
    if (!mqttClient.connected()) {
        connectAWSIoT();
    }
    mqttClient.loop();

    long now = millis();
    if (now - messageSentAt > 5000) {
        messageSentAt = now;
        sprintf(pubMessage, "{\"state\": {\"desired\":{\"foo\":\"%d\"}}}", dummyValue++);
        Serial.print("Publishing message to topic ");
        Serial.println(pubTopic);
        Serial.println(pubMessage);
        mqttClient.publish(pubTopic, pubMessage);
        Serial.println("Published.");
    }
}

void mqttCallback (char* topic, byte* payload, unsigned int length) {
   int total = 0;
   Serial.print("Received. topic=");
   Serial.println(topic);

   for (int i = 0; i < 5; i++) {
       if( i < 5){
           total += (int)payload[i];
           Serial.print((int)payload[i]);
           Serial.print(" ");
       }
   }
   if (total == 558){
     ledcWrite(ledChannel, 120);
     digitalWrite(drive_enable,  HIGH);
     Serial.println("Lets drive");
   }
   else if (total == 486){
     digitalWrite(drive_enable,  LOW);
     ledcWrite(turnChannel, 3);
     digitalWrite(turn_enable, LOW);
     Serial.println("Stopping");
   }
   else if (total == 459){
     digitalWrite(turn_right1_left0, LOW);
     digitalWrite(turn_enable, HIGH);
     ledcWrite(turnChannel, 120);
     Serial.println("Turning Left");
   }
   else if (total == 542){
     digitalWrite(turn_right1_left0, HIGH);
     digitalWrite(turn_enable, HIGH);
     ledcWrite(turnChannel, 120);
     Serial.println("Turning Right");
   }
}

void loop() {
  mqttLoop();
}
