#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif



/*/////////////////////////////////////////////////////////////////////////////
Required libraries for all vehicles.
/////////////////////////////////////////////////////////////////////////////*/
#include <WiFiClientSecure.h>
#include <PubSubClient.h>



/*/////////////////////////////////////////////////////////////////////////////
Switch for different vehicle configurations.
Set this value before flashing.
/////////////////////////////////////////////////////////////////////////////*/
#if 1
#define tim 1
#define david 0
#else
#define david 1
#define tim 0
#endif



/*/////////////////////////////////////////////////////////////////////////////
Network properties
Set these values before flashing.
/////////////////////////////////////////////////////////////////////////////*/
//char *ssid = "Headquarters";
//char *password = "@@Casa55";
//char *ssid = "titan-share";
//char *password = "titanrover";
//char *ssid = "ASUS_ACM";
//char *password = "fullerton306";
char *ssid = "MasterMobile";
char *password = "titanrover";



/*/////////////////////////////////////////////////////////////////////////////
LED properties
Currently Tim's vehicle only
/////////////////////////////////////////////////////////////////////////////*/
const int red = 23;
const int blue = 22;
const int green = 21;



/*/////////////////////////////////////////////////////////////////////////////
IMU configuration
Currently David's vehicle only
/////////////////////////////////////////////////////////////////////////////*/
#if david
  #include <Wire.h>
  #include <Adafruit_Sensor.h>
  #include <Adafruit_LSM303_U.h>
  #include <Adafruit_BMP085_U.h>
  #include <Adafruit_L3GD20_U.h>
  #include <Adafruit_10DOF.h>
  Adafruit_10DOF                dof   = Adafruit_10DOF();
  Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(30301);
  Adafruit_LSM303_Mag_Unified   mag   = Adafruit_LSM303_Mag_Unified(30302);
  Adafruit_BMP085_Unified       bmp   = Adafruit_BMP085_Unified(18001);
#endif



/*/////////////////////////////////////////////////////////////////////////////
PWM properties
/////////////////////////////////////////////////////////////////////////////*/
const int freq = 5000;
const int resolution = 8;



/*/////////////////////////////////////////////////////////////////////////////
Turning motor properties
/////////////////////////////////////////////////////////////////////////////*/
int turn_PWM = 14;  // adc2_channel 8
int turn_enable  = 27;
int turn_direction = 12;
const int turnChannel = 8;



/*/////////////////////////////////////////////////////////////////////////////
Driving motor properties
/////////////////////////////////////////////////////////////////////////////*/
int drive_direction = 26;
int drive_PWM = 13;   // adc1_channel 5
int drive_enable = 25;
const int driveChannel = 5;



/*/////////////////////////////////////////////////////////////////////////////
Front mounted ultrasonic sensor properties
/////////////////////////////////////////////////////////////////////////////*/
const int trigPin = 15;
const int echoPin = 2;



/*/////////////////////////////////////////////////////////////////////////////
Other properties
/////////////////////////////////////////////////////////////////////////////*/
long duration;
int distanceValue;
int velocity = 0;
bool turn = false;
const char *endpoint = "a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com";
const int port = 8883;



/*/////////////////////////////////////////////////////////////////////////////
Method prototypes
/////////////////////////////////////////////////////////////////////////////*/
void connectAWSIoT();
void mqttCallback (char* topic, byte* payload, unsigned int length);
void TaskConnectToAWS( void *pvParameters );
void TaskSensorUpdate( void *pvParameters );
void TaskIMUUpdate( void *pvParameters );



/*/////////////////////////////////////////////////////////////////////////////
Amazon's shared certificate.
/////////////////////////////////////////////////////////////////////////////*/
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



/*/////////////////////////////////////////////////////////////////////////////
Tim's vehicle's properties
/////////////////////////////////////////////////////////////////////////////*/
#if tim
char car[5] = "car1";
char *pubTopic = "$aws/things/rover_thing1/shadow/update";
char *subTopic = "$aws/things/rover_thing1/shadow/update/delta";
const char* certificate = "-----BEGIN CERTIFICATE-----\n"
"MIIDWTCCAkGgAwIBAgIUZWrwVmYp0w4bhrAzsczBHuefu/8wDQYJKoZIhvcNAQEL\n"
"BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g\n"
"SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTE5MDQxMDA5MzMw\n"
"N1oXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0\n"
"ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAMpgb4ueMDbn2QR79MLl\n"
"1b537qmXo+BJpOjHID92ZFoHlL5RWjAU/t7eKkRWCTtyHxciWi+kFYwW6nKoJhGE\n"
"U5JQh4v4X7jFm6yLiqmETnsbbySZu7l6262KfBEb6SeEDM65plA7T4q2p2m4z5gu\n"
"HTSE33iDyEq/aN7SukeVemZsK79YK7PZjnl2EfHOzy7wfUPSDFmOv4aNYzopOkYe\n"
"Jj6Gh2h8rhJAM24rO2zCf7RQK4e51WwXZacADP6XhLpT7vS7A0SkjQMmurFrNCeX\n"
"h8RQM/En4ZKA6ndvg6idVRxarOsqpDqfAcCzgy3rvwJpyETWI93rmdGPGdqhxe3f\n"
"fakCAwEAAaNgMF4wHwYDVR0jBBgwFoAUD6uPeDiyIfzyJp/PzPU2aCFmSLkwHQYD\n"
"VR0OBBYEFFRjDEKUE11QK8gvb4ettn+UT3xcMAwGA1UdEwEB/wQCMAAwDgYDVR0P\n"
"AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQAH9EOgcwtx/bTlNrdu+eXuC5AW\n"
"AaI/h2aeH34Gev5xM8mEyBkqp4zKk6bWCOs5JHATvwOgV9iYtVV3b+z0oQKItbhQ\n"
"ejhev5KNeUlawYU2QG3R21wTCUGy74/42lNoB/NqghaKrHCBxf+P7ZPppICJMfly\n"
"hQyL6bpYg3MBAs/hx20X8puWsx7wZZ4/zR2qsuJEbbLMNqstbMv4Rx5bVXLV8emk\n"
"utzwQUGXyuxtErQTOdZ4C8/tgtAPi+hGlEUzliNRg9h8jMGNPzHUdtDMBprsLdc0\n"
"5c/wis2QK4/k/WfH25T306g8PYdh65raEnspycF2UV2lykI4qGC9oC9Adug6\n"
"-----END CERTIFICATE-----\n";
const char* privateKey = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEpAIBAAKCAQEAymBvi54wNufZBHv0wuXVvnfuqZej4Emk6McgP3ZkWgeUvlFa\n"
"MBT+3t4qRFYJO3IfFyJaL6QVjBbqcqgmEYRTklCHi/hfuMWbrIuKqYROextvJJm7\n"
"uXrbrYp8ERvpJ4QMzrmmUDtPiranabjPmC4dNITfeIPISr9o3tK6R5V6Zmwrv1gr\n"
"s9mOeXYR8c7PLvB9Q9IMWY6/ho1jOik6Rh4mPoaHaHyuEkAzbis7bMJ/tFArh7nV\n"
"bBdlpwAM/peEulPu9LsDRKSNAya6sWs0J5eHxFAz8SfhkoDqd2+DqJ1VHFqs6yqk\n"
"Op8BwLODLeu/AmnIRNYj3euZ0Y8Z2qHF7d99qQIDAQABAoIBAG/cqCtzT9E5jCJH\n"
"Zf5IzkHOxxPk5sc1zbqBPpj1lviQki40h3ZmVI7aHGDTcZzzMq415cXJm8Noc0TX\n"
"Htu12f/ZFWNP8ZEPYUIZi5axtDOl8WQ4uaQHfd/J2VQXqmOWQQIPaheVAR16YsU0\n"
"S0Mh0jDSdTeHGDIA5TWtIxwoSpTLn0XPPlHOnZR5Nx3pRp1d3fhGjAHE/PEWF+yi\n"
"adJz2NDPvfKQ9s2+jSbOC/Aojeow0OfXSw9klgqZq0c0PGZNCkzIUIdO5reAQazV\n"
"0YA0eByjky+iR1EaNMFGK9N32nCZrpbghOwUPB7XlCm30pyCR+/9uaxmY8BMArAv\n"
"UFl8k4ECgYEA98g3GmyopXg4KI2tT55FYhRnDEEvOgogYaS0/RzyMQ+c4+2I52a6\n"
"sXv+0jf5ShRCRatWIRDQXgyrQBBT6JKQ0E2pFf90tsa9ul0f4SgpbY+0nGBaQiQj\n"
"Ev4ZI0+IsY42V/9larZMqY4N5gny0+e0Zk3hIlfvmdUUDYd3mQ4ZgdkCgYEA0Ra1\n"
"KyK7sCTDAZ2f7ePCEkXWdVOUe68EjoPDW2/COGKeFm3Sjw4jYHQG828kLLViXH7/\n"
"mEvCzduIbOYgXOUz3loM1iDxzZVMYQEggNOkveHXistQcHYmkKjpn1oxQOsxBcnZ\n"
"K0QEuEW61Clg7zNogyuZ4U3mkmiQbP4AQenvqFECgYEA0HY4rdmW9Ue0b6Hqmp58\n"
"nyytGalJs1nLrnqk7oJI3K/W/gfc81oBXcsFMxV2fTWYIAcrOjxsIYQlG4vhHD0a\n"
"6rU7uU8ngZKpEr7AUGy6lfueJyYFKbo/a/4tZgDG7yLHY7dyOppO2yOxW8Uo79Dz\n"
"fFKvQ3aaFWEoMs2y+CjdmtkCgYEArGilCuShDyXBBtwKvAZKSPb9V9GLZlFy2HEd\n"
"7Y/B/uySnrwAG6X01dvWv3Stx4/pg4vVBqeyAj07yVAY74CvvHFUlFymBndNe8+K\n"
"FbBu74QCT5hGw/Uo56/gtF2Sr40/5x4BzQNRQQF+hON54sPpMgAJ6xZpQeCm9rib\n"
"Tb6wt4ECgYAGi+UhasTDT8Qk7QciNTxWLgfQxZApwQrnDcN4ETmmPnQr9lSwCZuo\n"
"JoAhtQiyZWAK6JurR6fp8qDt7ajORRpDFl1F0TyePrd8AkiT/y4LaVyphL0yuQMd\n"
"DcgwqPcAZUmiCW3wmYnbVwyPYy86H6g0FZ1B9LF4oP7gCQOS7Op9rw==\n"
"-----END RSA PRIVATE KEY-----\n";
#endif



/*/////////////////////////////////////////////////////////////////////////////
David's vehicle's properties
//sprintf(pubMessage, "{\"car\":\"%s\",\"dist\":\"%d\"}", car, distanceValue);
/////////////////////////////////////////////////////////////////////////////*/
#if david
char car[5] = "car2";
char *pubTopic = "$aws/things/rover_thing2/shadow/update";
char *subTopic = "$aws/things/rover_thing2/shadow/update/delta";
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
#endif



/*/////////////////////////////////////////////////////////////////////////////
Object creation
/////////////////////////////////////////////////////////////////////////////*/
WiFiClientSecure httpsClient;
PubSubClient mqttClient(httpsClient);



/*/////////////////////////////////////////////////////////////////////////////
IMU functions
Currently David's vehicle only
/////////////////////////////////////////////////////////////////////////////*/
void initSensors(){
  #if david
    if(!accel.begin()) {
      Serial.println(F("No LSM303 detected - Check vehicle wiring"));
      while(1);
    }
    if(!mag.begin()) {
      Serial.println("No LSM303 detected - Check vehicle wiring");
      while(1);
    }
    if(!bmp.begin()) {
      Serial.println("No BMP180 detected - Check vehicle wiring");
      while(1);
    }
  #endif
}



/*/////////////////////////////////////////////////////////////////////////////
Runs automatically upon bootup
/////////////////////////////////////////////////////////////////////////////*/
void setup() {



  // Initialize serial communication at 115200 bits per second
  Serial.begin(115200);



  // LED setup
  #if tim
    pinMode(red, OUTPUT);
    pinMode(blue, OUTPUT);
    pinMode(green, OUTPUT);
    digitalWrite(red, HIGH);
    digitalWrite(blue, LOW);
    digitalWrite(green, LOW);
  #endif



  // IMU initialization
  #if david
    initSensors();
  #endif



  // Front mounted ultransonic setup
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input



  delay(1000);
  Serial.begin(115200);

  pinMode(turn_enable, OUTPUT);
  pinMode(drive_enable, OUTPUT);

  digitalWrite(turn_enable, HIGH);
  digitalWrite(drive_enable,  HIGH);

  pinMode(turn_direction, OUTPUT);
  pinMode(drive_direction, OUTPUT);

  digitalWrite(turn_direction, LOW);
  digitalWrite(drive_direction, LOW);

  pinMode(turn_PWM, OUTPUT);
  pinMode(drive_PWM, OUTPUT);

  // Enable all motors
  digitalWrite(turn_enable, HIGH);
  digitalWrite(drive_enable, HIGH);

  ledcSetup(driveChannel, freq, resolution);
  ledcSetup(turnChannel, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(turn_PWM, turnChannel);
  ledcAttachPin(drive_PWM, driveChannel);

  // Start WiFi
  Serial.println("Connecting to ");
  Serial.print(ssid);
  //WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
      #if tim
      digitalWrite(red, LOW);
      digitalWrite(blue, HIGH);
      digitalWrite(green, LOW);
      #endif
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



  /*/////////////////////////////////////////////////////////////////////////////
  FreeRTOS task creation
  The task scheduler will automatically initiate
  /////////////////////////////////////////////////////////////////////////////*/
  xTaskCreatePinnedToCore(
    TaskConnectToAWS
    ,  "TaskConnectToAWS"   // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    ,  ARDUINO_RUNNING_CORE
  );
  xTaskCreatePinnedToCore(
    TaskSensorUpdate
    ,  "AnalogReadA3"
    ,  1024  // Stack size
    ,  NULL
    ,  1  // Priority
    ,  NULL
    ,  ARDUINO_RUNNING_CORE
  );
  xTaskCreatePinnedToCore(
    TaskIMUUpdate
    ,  "TaskIMUUpdate"   // A name just for humans
    ,  1024  // Stack size
    ,  NULL
    ,  3  // Priority
    ,  NULL
    ,  ARDUINO_RUNNING_CORE
  );
}



/*/////////////////////////////////////////////////////////////////////////////
Add documentation here
/////////////////////////////////////////////////////////////////////////////*/
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



/*/////////////////////////////////////////////////////////////////////////////
Add documentation here
Can these be moved to the top of the doc where other variables are created?
/////////////////////////////////////////////////////////////////////////////*/
long messageSentAt = 0;
int dummyValue = 0;
char pubMessage[128];



/*/////////////////////////////////////////////////////////////////////////////
Add documentation here.
/////////////////////////////////////////////////////////////////////////////*/
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
    if(turn){
      total == 0;
    }
    switch(total){
      case 558:
        Serial.println("Lets drive");digitalWrite(drive_enable,  HIGH);digitalWrite(turn_enable, LOW);
        velocity = 240;
        ledcWrite(driveChannel, velocity);
        break;
      case 529:
        Serial.println("Speed");digitalWrite(drive_enable,  HIGH);digitalWrite(turn_enable, LOW);
        if (velocity < 240){ velocity = velocity + 10; }
        ledcWrite(driveChannel, velocity);
        break;
      case 485:
        Serial.println("Slowing");digitalWrite(drive_enable,  HIGH);digitalWrite(turn_enable, LOW);
        if(velocity > 40){ velocity = velocity - 40; }
        ledcWrite(driveChannel, velocity);
        break;
      case 486:
        Serial.println("Stopping"); digitalWrite(drive_enable,  LOW); digitalWrite(turn_enable, LOW);
        velocity = 0;
        ledcWrite(turnChannel, velocity);
        turn = true; turnaround();
        distanceValue = 35;
        mqttLoop();
        break;
      case 459:
        Serial.println("Turning Left"); digitalWrite(turn_direction, LOW); digitalWrite(turn_enable, HIGH);
        ledcWrite(turnChannel, 240);
        break;
      case 542:
        Serial.println("Turning Right"); digitalWrite(turn_direction, HIGH); digitalWrite(turn_enable, HIGH);
        ledcWrite(turnChannel, 240);
        break;
    }
}



/*/////////////////////////////////////////////////////////////////////////////
Add documentation here.
/////////////////////////////////////////////////////////////////////////////*/
void mqttLoop() {
    if (!mqttClient.connected()) {
        connectAWSIoT();
    }
    mqttClient.loop();
    long now = millis();
    if (now - messageSentAt > 507 && !turn) {
        messageSentAt = now;
        //dummyValue = dummyValue + 100;
        //sprintf(pubMessage, "{\"car1\":\"%d\"}", dist); // dummyValue++); // {\"desired\":{\"message\":\"%d\"}}}", dummyValue++);
        sprintf(pubMessage, "{\"car\":\"%s\",\"dist\":\"%d\"}", car, distanceValue);
        Serial.print("Publishing message to topic ");
        Serial.println(pubTopic);
        Serial.println(pubMessage);
        mqttClient.publish(pubTopic, pubMessage);
        Serial.println("Published.");
    }
}



/*/////////////////////////////////////////////////////////////////////////////
Add documentation here.
/////////////////////////////////////////////////////////////////////////////*/
int distanceSensor(){
  int distance = 0;
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

  return distance;
}



/*/////////////////////////////////////////////////////////////////////////////
Motor procedures for executing a 180 degree turn.
/////////////////////////////////////////////////////////////////////////////*/
void turnaround() {
  digitalWrite(turn_enable, HIGH);
  digitalWrite(drive_enable, HIGH);
  delay(3000);
  Serial.println("Start in a stopped state");
  ledcWrite(driveChannel, 3);
  ledcWrite(turnChannel, 3);

  Serial.println("Prepare wheels for right turn");
  digitalWrite(turn_direction, LOW);    // Right:     L   Left:     H

  Serial.println("Prepare wheels for reverse");
  digitalWrite(drive_direction, HIGH);   // Forward:   L   Reverse:  H

  Serial.println("Apply power to turning motors");
  ledcWrite(turnChannel, 80);

  Serial.println("Run turning motors for 1 second");
  delay(1000);

  Serial.println("Apply power to drive motors");
  ledcWrite(driveChannel, 80);

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
  ledcWrite(turnChannel, 80);

  Serial.println("Run turning motors for 1 second");
  delay(1000);

  Serial.println("Apply power to drive motors");
  ledcWrite(driveChannel, 80);

  Serial.println("Run all motors for 1.5 seconds");
  delay(1500);

  Serial.println("Kill all motors");
  ledcWrite(driveChannel, 3);
  ledcWrite(turnChannel, 3);
  turn = false;
}



/*/////////////////////////////////////////////////////////////////////////////
Loop is unused as management is performed with with FreeRTOS tasks.
/////////////////////////////////////////////////////////////////////////////*/
void loop(){

}



/*/////////////////////////////////////////////////////////////////////////////
FreeRTOS task
Add documentation here.
/////////////////////////////////////////////////////////////////////////////*/
void TaskConnectToAWS(void *pvParameters) {
  (void) pvParameters;

  /*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  If you want to know what pin the on-board LED is connected to on your ESP32 model, check
  the Technical Specs of your board.
  */

  // initialize digital LED_BUILTIN on pin 13 as an output.
  //pinMode(LED_BUILTIN, OUTPUT);

  for (;;) // A Task shall never return or exit.
  {
    #if tim
      digitalWrite(red, LOW);
      digitalWrite(blue, LOW);
      digitalWrite(green, LOW);
    #endif

    mqttLoop();

    #if tim
      digitalWrite(green, HIGH);
    #endif

    //delay(10);
    //digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    //vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
    //digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    vTaskDelay(100);  // one tick delay (15ms) in between reads for stability
  }
}



/*/////////////////////////////////////////////////////////////////////////////
FreeRTOS task
Add documentation here.
/////////////////////////////////////////////////////////////////////////////*/
void TaskSensorUpdate(void *pvParameters) {
  (void) pvParameters;

  /*
  AnalogReadSerial
  Reads an analog input on pin A3, prints the result to the serial monitor.
  Graphical representation is available using serial plotter (Tools > Serial Plotter menu)
  Attach the center pin of a potentiometer to pin A3, and the outside pins to +5V and ground.

  This example code is in the public domain.
  */

  for (;;)
  {
    distanceValue = distanceSensor();



    // read the input on analog pin A3:
    //int distanceValue = analogRead(A3);
    // print out the value you read:
    //Serial.println(distanceValue);
    vTaskDelay(10);  // one tick delay (15ms) in between reads for stability
  }
}



/*/////////////////////////////////////////////////////////////////////////////
FreeRTOS task
Add documentation here.
/////////////////////////////////////////////////////////////////////////////*/
void TaskIMUUpdate(void *pvParameters) {
  (void) pvParameters;
  for (;;) {
    #if david
      sensors_event_t accel_event;
      sensors_event_t mag_event;
      sensors_event_t bmp_event;
      sensors_vec_t   orientation;

      /* Calculate pitch and roll from the raw accelerometer data */
      accel.getEvent(&accel_event);
      if (dof.accelGetOrientation(&accel_event, &orientation)) {
        /* 'orientation' should have valid .roll and .pitch fields */
        Serial.print(F("Roll: "));
        Serial.print(orientation.roll);
        Serial.print(F("; "));
        Serial.print(F("Pitch: "));
        Serial.print(orientation.pitch);
        Serial.print(F("; "));
      }

      /* Calculate the heading using the magnetometer */
      mag.getEvent(&mag_event);
      if (dof.magGetOrientation(SENSOR_AXIS_Z, &mag_event, &orientation)) {
        /* 'orientation' should have valid .heading data now */
        Serial.print(F("Heading: "));
        Serial.print(orientation.heading);
        Serial.print(F("; "));
      }

      /* Calculate the altitude using the barometric pressure sensor */
      bmp.getEvent(&bmp_event);
      if (bmp_event.pressure) {
        /* Get ambient temperature in C */
        float temperature;
        bmp.getTemperature(&temperature);
        /* Convert atmospheric pressure, SLP and temp to altitude    */
        Serial.print(F("Alt: "));
        Serial.print(bmp.pressureToAltitude(seaLevelPressure,
                                            bmp_event.pressure,
                                            temperature));
        Serial.print(F(" m; "));
        /* Display the temperature */
        Serial.print(F("Temp: "));
        Serial.print(temperature);
        Serial.print(F(" C"));
      }

      Serial.println(F(""));
    #endif
    vTaskDelay(1000);  // one tick delay (15ms) in between reads for stability
  }
}
