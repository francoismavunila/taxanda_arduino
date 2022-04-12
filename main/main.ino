#include "base64.hpp"
#include <Wire.h>
#include <ArduinoJson.h>
#include <WiFi.h>  
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <HardwareSerial.h>

uint8_t fingerTemplate[512]; // the real template



#include "configSensor.h"
#include "authenticate.h"
#include "registerDriverPrint.h"
#include "getDriverFingerPrint.h"



//---- WiFi settings
const char* ssid = "tmw";
const char* password = "francois00001";

//---- MQTT Broker settings
const char* mqtt_server = "9c641868feef4f288cb222c3d38b905e.s1.eu.hivemq.cloud";
const char* mqtt_username = "Francois";
const char* mqtt_password = "12345Fra";
const int mqtt_port =8883;

 

WiFiClientSecure espClient;  
PubSubClient client(espClient);
unsigned long lastMsg = 0;

const char* device_id = "1010";

const char* driversub_topic= "driver/register/1010/getFP";

const char* driverpub_topic="driv/register/sendFP";




// Set GPIOs for LED 
const int led = 32;
const int doors = 25;
const int authButton = 35;


boolean auth = false;



static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";


//==========================================
void setup_wifi() {
  delay(10);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi connected\nIP address: ");
  Serial.println(WiFi.localIP());
  Serial.println(MQTT_MAX_PACKET_SIZE );
}


//=====================================
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP23Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("connected");

      client.subscribe(driversub_topic);   // subscribe the topics here
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");   // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}





// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR authenticateTrig() {
  Serial.println("Authenticate");
  auth = true;
}

void setup() {
   setup_wifi();
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output



  espClient.setCACert(root_ca);      // enable this line and the the "certificate" code for secure connection
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  
  // Serial port for debugging purposes
  Serial.begin(115200);
  finger.begin(57600);
  
  // PIR Motion Sensor mode INPUT_PULLUP
  pinMode(authButton, INPUT);
  // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
  attachInterrupt(digitalPinToInterrupt(authButton), authenticateTrig, FALLING);

  // Set LED to LOW
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);

  pinMode(doors, OUTPUT);
  digitalWrite(doors, LOW);
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
  digitalWrite(led, HIGH);
  Serial.print("on");
  delay(2000);
  Serial.print("off");
  digitalWrite(led, LOW);
   delay(2000);
  if(auth) {
    //call the authentication function
    Serial.println("call the authentication function");
   
    uint8_t passId=  authenticate();
    if(passId==false){
      //dont open doors, authentication failed, failed to register passenger
      Serial.print("dont open doors, authentication failed, failed to register passenger");
    }else if(passId==250){
      //dont open doors, allert police, driver not authenticated
      Serial.print("dont open doors, allert police, driver not authenticated");
    }else{
      //get passenger id open doors and use the id to fetch and send the activity details to the server
      Serial.print("get passenger id open doors and use the id to fetch and send the activity details to the server");
      digitalWrite(doors, HIGH);
    }
    auth = false;
  }
}


void callback(char* topic, byte* payload, unsigned int length) {

    
    
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Message arrived ["+String(topic)+"]"+incommingMessage);
  
  //--- check the incomming message
    if( strcmp(topic,driversub_topic) == 0){
          String driver_id = incommingMessage;
         //call function that registers the fingerprint
         bool stats = enroll_Driver();
         if(stats){
             //call a function to get the driver's fingerprint
             downloadFingerprintTemplate(101);
             uint8_t output_buffer[512];
              encode_base64(fingerTemplate, 512, output_buffer);
              Serial.println("converted");
               char* fingerP =(char *)output_buffer;
              Serial.println(fingerP);

               StaticJsonDocument<2048> doc;
              doc["device_id"] = device_id;
              doc["driver_id"] = driver_id; 
              doc["data"] = fingerP;
              char JSONmessageBuffer[2048];
              serializeJson(doc, JSONmessageBuffer);
              Serial.println("Sending message to MQTT topic..");
              Serial.println(JSONmessageBuffer);
              

             if (!client.connected()) reconnect();
             client.loop();
             bool st = client.publish(driverpub_topic,JSONmessageBuffer);
             Serial.print(st);
     
           
                


       }else{
        //wrong topic
        Serial.println("wrong toipc value");
       }
    }else{
       Serial.println("another topic");
    }

}
