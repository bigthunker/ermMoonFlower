
#include "secret.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

 
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub" // the format should be uniqueID/esp32/pub I want 
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"


unsigned long lastPublishTime = 0;
const unsigned long publishInterval = 600000; //60000 this is the time it takes to send a message approx every 10 minutes


int light_relay = 22; //define relay pin
int pump_relay = 23; 

float h;
float t;
bool water = true;
bool light = false;
char uniqueid[] = "2024gp9MDkFox3A3";
String uniqueID2 = "2024gp9MDkFox3A3";
String iotpub = "/esp32/pub";
String iotsub = "/esp32/sub";

String finalpub = uniqueID2 + iotpub;
String finalsub = uniqueID2 + iotsub;
//char finalpub2[] = finalpub


 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["uniqueid"] = uniqueid;
  doc["pumpState"] = water;
  doc["lightState"] = light;
  doc["pH"] = h;
  doc["EC"] = t;
  doc["humidty"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
// NEW ---------------------------------------------------------------------------------------------
  if (String(message) == "water off") {
    water = false;
    Serial.print("water off");
    onPumpStateChange();

  } else if (String(message) == "water on") {
    water = true;
    Serial.print("water on");
    onPumpStateChange();  
    }

    
  else if (String(message) == "light on") {
    light = true;
    Serial.print("light on");
    onLightStateChange();
    
  }else if (String(message) == "light off") {
    light = false;
    Serial.print("light off");
    onLightStateChange();
    
  }
  /*else if (String(message) == "water on") {
    water = true;
    Serial.print("water is on");
  }else if (String(message) == "water on") {
    water = true;
    Serial.print("water is on");
  }*/

// NEW ---------------------------------------------------------------------------------------------

  //Serial.println(message);
}
 
void setup()

{
  Serial.begin(115200);
  connectAWS();

  delay(1500); 

  pinMode(light_relay, OUTPUT); // set light pin to output 
  digitalWrite(light_relay, HIGH); // HIGH is off

  // initialize pump relay as output and initially off
  pinMode(pump_relay, OUTPUT);
  digitalWrite(pump_relay, HIGH);
}
 
void loop()
{
  //light = false;
  //water = true;
 
 
  if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
 unsigned long currentTime = millis();
  if (currentTime - lastPublishTime >= publishInterval) {
    lastPublishTime = currentTime;



  Serial.print(F("Pump State: "));
  Serial.print(light);
  Serial.print(F("  Light State: "));
  Serial.print(water);
  Serial.print(F("  pH: "));
  Serial.print(h);
  Serial.print(F("  eC: "));
  Serial.print(t);
  Serial.print(F(" humidty: "));
  Serial.print(t);
  Serial.print(F("%"));
  Serial.println(F(""));

  publishMessage();
  }
  client.loop();
  //delay(20000);
}




// pump and light functions ------------------------------------------------------------------

void onLightStateChange()  {
  // Add your code here to act upon LightState change
  if (light){ // when light state is true
    digitalWrite(light_relay, LOW); // turn lights on
  }

  if (!light){
    digitalWrite(light_relay, HIGH); // turn lights off
  }
}


void onPumpStateChange()  {
  // Add your code here to act upon PumpState change
  if (water){  // when pump state is true
    digitalWrite(pump_relay, LOW);  // turn pump on
  }

  if (!water){  // when pump state is false
    digitalWrite(pump_relay, HIGH); // turn pump off
  }
}



/*
  Since PumpSchedule is READ_WRITE variable, onPumpScheduleChange() is
  executed every time a new value is received from IoT Cloud.
*/
/*

void lightControl() {
  bool previousLightState = light_state;
  
  if(light_schedule.isActive()) {// variable is "active" when local time is in the scheduled range
    light_state = true;
  }
  else { // outside of scheduled time
    light_state = false; // turn lights off
    
  }

  if (light_state != previousLightState) {
    onLightStateChange(); // Call the function to update relay state
  }
  
}



void pumpControl(){
  bool previousPumpState = pump_state;

  if (pump_schedule.isActive()){ // in schdelued time range based on local time 
    pump_state = true; 
  }

  else {  // outside of scheduled time
    pump_state = false;
  }

  if (pump_state != previousPumpState){
    onPumpStateChange(); // call the function to update relay state
  }
}
*/