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

// Sonic sensor
// defines pins numbers
const int trigPin = 15;
const int echoPin = 2;

// defines variables
long duration;
int distance;
int velocity = 0;

void connectAWSIoT();
void mqttCallback (char* topic, byte* payload, unsigned int length);

//char *ssid = "Headquarters";
//char *password = "@@Casa55";
//char *ssid = "titan-share";
//char *password = "titanrover";
//char *ssid = "ASUS_ACM";
//char *password = "fullerton306";
char *ssid = "MasterMobile";
char *password = "titanrover";

const char *endpoint = "a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com";

const int port = 8883;
//char *pubTopic = "my/swarm";
//char *subTopic = "my/smarm/update";
//char *pubTopic = "$aws/things/rover_thing1/shadow/update";
//char *subTopic = "$aws/things/rover_thing1/shadow/update/delta";
char *pubTopic = "$aws/things/rover_thing2/shadow/update";
char *subTopic = "$aws/things/rover_thing2/shadow/update/delta";


const char* rootCA = "-----BEGIN CERTIFICATE-----\n"
"MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF\n"
"ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6\n"
"b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL\n"
"MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv\n"
"b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj\n"
"ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM\n"
"9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw\n"
"IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6\n"
"VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L\n"
"93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm\n"
"jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC\n"
"AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA\n"
"A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI\n"
"U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs\n"
"N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv\n"
"o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU\n"
"5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy\n"
"rqXRfboQnoZsG4q5WTP468SQvvG5\n"
"-----END CERTIFICATE-----\n";

 //David Certs
const char* certificate = "-----BEGIN CERTIFICATE-----\n"
"MIIDWjCCAkKgAwIBAgIVAIrXriYmpELGGjosQICNg37vGH9AMA0GCSqGSIb3DQEB\n"
"CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t\n"
"IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0xOTA0MTAwODEz\n"
"MDdaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh\n"
"dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDZUNHVblI5m+TgwOYQ\n"
"O1Nyo5wf6oxOuCdKYpy+XphDMv7XafpFWg4wXPJnXaZoTQE++JYLmZbMx8SM9ICB\n"
"3nXAzXJFU22BuufvKHRkGoIcmNoFKDNDSCE1o91H6MP2blloqysHBcBNsoqEztae\n"
"toetDyjERASgjIulfR8YmxFKYA7vjymuSr4pSMrNFz3PAQo/ityUblm3DbY6oft+\n"
"xFHxlHnZke+dSrqeMA8UgK7syKJ1i6o0kxBj11QxCyga6Dxngnt12hUBkABdZp+S\n"
"Z9+iaC/YKHOiTghATGl69rh1P+Ajk2T83m78AOC3blkh0LNV2YMsS6Sj33BzKEOG\n"
"EPCtAgMBAAGjYDBeMB8GA1UdIwQYMBaAFPEUgJYjjgv0j4XE3BhDLQ7/Gy60MB0G\n"
"A1UdDgQWBBSzOhwXFifkGiIHNMgZQUdJzFklizAMBgNVHRMBAf8EAjAAMA4GA1Ud\n"
"DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEATuMyIv+PFR4rJfmrGgc1d48W\n"
"siYyQRyFRnIKgaq+KEt3oHhlhQDgm3oi7RGnwVLFoNpmnlIzIcAhgJC/cP7OMOg3\n"
"U2l5ROrsc8bhU9u90VMBc3Mvtzk/c2ftTofLjpdZZHHSMN4rteB+XH+u6QDu2fMo\n"
"bByNm2G//e/pk9mKMKzL86ZaOspRDonQMepMgrxICq1Lu5iEXJcn7kyg5LRHaaRm\n"
"dDPGpTWvsJlXrGrEgRUqsHQaSD98kYSywdm1H0ulxYeDcjxaApFfmDqNInf5M85j\n"
"r/A6wgnApu0NGRgdibsSIR/dwr/voohCyTselWy6MGhs3Usx/EKoQdLFnxKsJQ==\n"
"-----END CERTIFICATE-----\n";

const char* privateKey = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEpgIBAAKCAQEA2VDR1W5SOZvk4MDmEDtTcqOcH+qMTrgnSmKcvl6YQzL+12n6\n"
"RVoOMFzyZ12maE0BPviWC5mWzMfEjPSAgd51wM1yRVNtgbrn7yh0ZBqCHJjaBSgz\n"
"Q0ghNaPdR+jD9m5ZaKsrBwXATbKKhM7WnraHrQ8oxEQEoIyLpX0fGJsRSmAO748p\n"
"rkq+KUjKzRc9zwEKP4rclG5Ztw22OqH7fsRR8ZR52ZHvnUq6njAPFICu7MiidYuq\n"
"NJMQY9dUMQsoGug8Z4J7ddoVAZAAXWafkmffomgv2Chzok4IQExpeva4dT/gI5Nk\n"
"/N5u/ADgt25ZIdCzVdmDLEuko99wcyhDhhDwrQIDAQABAoIBAQDPNXGueU+roIq/\n"
"Bw8W4QpzX69o+0KneI32LkrYdefnA0Pc8C7zW6f101pK1w/pmasbePnQu7T/M2el\n"
"Iyk5K33Jxds0YRUkQqwN7Nv16xSu4cvYaPlgKPGJbOJ8Q2Q+n/9efv865e/z5c6Q\n"
"TYx0rwR1LeQoJ9blor0woTo6QvlcmLI5ULBrVQEEuSzrxsGZcyAJGAzuhAauhTf8\n"
"i+clnB1nMgSbZIibmReWqCgFSjgxPwrAfYonyx8iBL04STbzUp2rA19il30hdcS7\n"
"4l660gkZ+lNLQ4AAYFOEikgeVGSxQEVhCJry7xmFg4HitJIB1cxEqgAjWKSgBY1c\n"
"tmnLWti9AoGBAPwYjuDLPxMnbKeKvMU3cGEdKv4AKit0Vim1WSQb+rNx98fwi+is\n"
"un81q4toHOJCO7rKoxE2TtFc/mPqa2T0sqxUHlcXwL2oDdqkdRIEDlkSRx038Rjv\n"
"rQ4vn8ZduwBU+03S6EMCvjSP/N9cCpYdhtvTNd8R7K7zB7o7jRfdzQPPAoGBANyu\n"
"X9E2oRYVmeleXjwCRxCwtvczxbfc9om7Uyh8kCVsN2u1CA8f2Vdk3UmayhJ35LwM\n"
"MkIUdiP1ig8wi4krlf3P4ZLmvuH1U9yfOVMyjjGCy/VKeQLn0b2S0NMsprehvaOs\n"
"/xEAi5XDlkf7zI2QrxJGoBC+Fs7zEL2cV2FIyNbDAoGBALcaW0SZO1H9M6UpVtzj\n"
"3IMc/WoJ0pdHY4Hdz0Yz8E6/+5nEdMpmEvtsjxGtaDVKqDmPDwvkZzEYG0mVLbXh\n"
"KZmTa4oUnZDjbFSfWMziF03IwdBOHpbfFtIT8zfQQpAGKZWoF7z8VAy3fJL7NlE8\n"
"VwR5j0pbqY6UxMsWj++C0XLvAoGBAJVRh5PQGv8Obq3xJGIGSH+KksKBTW8zG73l\n"
"+kaSwhYQOw+t1l4ldrIjlXeQPNpyNVroBbmgjq+WHGxatI+D5mFun3r+lnndvXiy\n"
"Y99ffccScV7vnSHSKaDUDmhU9hmaXu64x32BmPEk/jKGtcmz9EGxd+zcMiKlZNZn\n"
"StiQnxRxAoGBALldCtMbE8fbFlAvbaFeM4xD1xbU6jacJEQm47FIOh8VJqC/LcPv\n"
"M5oLLqrgAG36JOdA1fP/i45EM4LX+NGBx+tv7qndbrTnQUBxQPwa++zjpP9fgBik\n"
"EhQbvw4yvlXLkhOdL0hmg/9hC5Jb4Urp0qUJN312Up6vjGHGWmf8GTP8\n"
"-----END RSA PRIVATE KEY-----\n";


WiFiClientSecure httpsClient;
PubSubClient mqttClient(httpsClient);

void setup() {

    //Sonic Sensor
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  
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
    //WiFi.mode(WIFI_STA);
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
            delay(507);
        }
    }
}

long messageSentAt = 0;
int dummyValue = 0;
char pubMessage[128];

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
      digitalWrite(drive_enable,  HIGH);
      velocity = 120;
      ledcWrite(ledChannel, velocity);
      Serial.println("Lets drive");
    }
    else if (total == 529){
      digitalWrite(drive_enable,  LOW);
      if (velocity < 120){
        velocity = velocity + 10;
      }
      ledcWrite(turnChannel, velocity);
      digitalWrite(turn_enable, LOW);
      Serial.println("Speed");
    }
    else if (total == 485){
      digitalWrite(drive_enable,  LOW);
      if(velocity > 10){
        velocity = velocity - 10;
      }
      ledcWrite(turnChannel, velocity);
      digitalWrite(turn_enable, LOW);
      Serial.println("Slowing");
    }
    else if (total == 486){
      digitalWrite(drive_enable,  LOW);
      velocity = 3;
      ledcWrite(turnChannel, velocity);
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

void mqttLoop(int dist) {
    if (!mqttClient.connected()) {
        connectAWSIoT();
    }
    mqttClient.loop();
    long now = millis();
    char car[5] = "car2";
    if (now - messageSentAt > 507) {
        messageSentAt = now;
        //dummyValue = dummyValue + 100;
        //sprintf(pubMessage, "{\"car1\":\"%d\"}", dist); // dummyValue++); // {\"desired\":{\"message\":\"%d\"}}}", dummyValue++);
        sprintf(pubMessage, "{\"car\":\"%s\",\"dist\":\"%d\"}", car, dist);
        Serial.print("Publishing message to topic ");
        Serial.println(pubTopic);
        Serial.println(pubMessage);
        mqttClient.publish(pubTopic, pubMessage);
        Serial.println("Published.");
    }
}

int distanceSensor(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance= duration*0.034/2;
  
  // Prints the distance on the Serial Monitor
  //Serial.print("Distance: ");
  //Serial.println(distance);
  
  return 90; //distance;
}

void loop() {
  ledcWrite(ledChannel, 120);
  mqttLoop(distanceSensor()); 
  
  delay(10);
}