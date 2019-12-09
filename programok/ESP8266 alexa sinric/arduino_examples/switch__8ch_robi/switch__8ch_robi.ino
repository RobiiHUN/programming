/*
 Version 0.3-Rev2 - January 14 2019
*/ 
/*
 Original Code by Kakopappa https://github.com/kakopappa/sinric
 Modified for 8-Channel Relay by Syko2K
 For GerrysDIY channel on YouTube. 
 Link: https://www.youtube.com/channel/UCqe1yTIkHEFx17lmI1zvttA
 
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h> //  https://github.com/kakopappa/sinric/wiki/How-to-add-dependency-libraries
#include <ArduinoJson.h> // https://github.com/kakopappa/sinric/wiki/How-to-add-dependency-libraries
#include <StreamString.h>

ESP8266WiFiMulti WiFiMulti;
WebSocketsClient webSocket;
WiFiClient client;

#define MyApiKey "5fc2c989-9a58-41d9-b7da-5b597c27672c" // Place your Sinric API here
#define MySSID "Edina_lak" //   Change to your Wifi network SSID
#define MyWifiPassword "Edina.Edina" // Change to your Wifi network password

#define HEARTBEAT_INTERVAL 300000 // 5 Minutes 

const int relayPin1 = 5;
const int relayPin2 = 4;
const int relayPin3 = 0;
const int relayPin4 = 16;
const int relayPin5 = 14;
const int relayPin6 = 12;
const int relayPin7 = 13;
const int relayPin8 = 2;

uint64_t heartbeatTimestamp = 0;
bool isConnected = false;

void setPowerStateOnServer(String deviceId, String value);
void setTargetTemperatureOnServer(String deviceId, String value, String scale);

// deviceId is the ID assgined to your smart-home-device in sinric.com dashboard. Copy it from dashboard and paste it here

void turnOn(String deviceId) {
  if (deviceId == "5c6445f73bbe740f8314e6b8") // Device ID of first device(BOILER)
  {  
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin1, HIGH);
  } 
  else if (deviceId == "5c6446183bbe740f8314e6ba") // Device ID of second device(OIL)
  { 
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin2, HIGH);
  }
  
  else if (deviceId == "5c68013e0cf62128863f9f1d") // Device ID of third device (UNDER FLOOR)
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin3, HIGH);
  }
  else if (deviceId == "5c68014c0cf62128863f9f21") // Device ID of fourth device
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin4, HIGH);
  }
  else if (deviceId == "5c6801540cf62128863f9f23") // Device ID of fifth device (SITTING ROOM)
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin5, HIGH);
  }
  else if (deviceId == "5c68015d0cf62128863f9f26") // Device ID of sixth device (KITCHEN)
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin6, HIGH);
  }
  else if (deviceId == "5c68016e0cf62128863f9f2c") // Device ID of seventh device (BEDROOMS)
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin7, HIGH);
  }
  else if (deviceId == "5c6801670cf62128863f9f29") // Device ID of eighth device (BATHROOM)
  {
    Serial.print("Turn on device id: ");
    Serial.println(deviceId);
    digitalWrite(relayPin8, HIGH);
  }
  
  else {
    Serial.print("Turn on for unknown device id: ");
    Serial.println(deviceId);    
  }     
}

void turnOff(String deviceId) {
   if (deviceId == "5c6445f73bbe740f8314e6b8") // Device ID of first device(BOILER)
   {  
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(relayPin1, LOW);
   }
   else if (deviceId == "5c6446183bbe740f8314e6ba") // Device ID of second device(OIL)
   { 
     Serial.print("Turn off Device ID: ");
     Serial.println(deviceId);
     digitalWrite(relayPin2, LOW);
   }
   
   else if (deviceId == "5c68013e0cf62128863f9f1d") // Device ID of third Device(UNDER FLOOR)
   {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(relayPin3, LOW);
   }
   else if (deviceId == "5c68014c0cf62128863f9f21") // Device ID of fourth Device
   {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(relayPin4, LOW);
   }
   else if (deviceId == "5c6801540cf62128863f9f23") // Device ID of fifth Device(SITTING ROOM)
   {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(relayPin5, LOW);
   }
   else if (deviceId == "5c68015d0cf62128863f9f26") // Device ID of sixth Device (KITCHEN)
   {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(relayPin6, LOW);
   }
   else if (deviceId == "5c68016e0cf62128863f9f2c") // Device ID of seventh Device (KITCHEN)
   {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(relayPin7, LOW);
   }
   else if (deviceId == "5c6801670cf62128863f9f29") // Device ID of eighth Device (Bathroom)
   {
    Serial.print("Turn off Device ID: ");
    Serial.println(deviceId);
    digitalWrite(relayPin8, LOW);
   }
   
  else {
     Serial.print("Turn off for unknown device id: ");
     Serial.println(deviceId);    
  }
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      isConnected = false;    
      Serial.printf("[WSc] Webservice disconnected from sinric.com!\n");
      break;
    case WStype_CONNECTED: {
      isConnected = true;
      Serial.printf("[WSc] Service connected to sinric.com at url: %s\n", payload);
      Serial.printf("Waiting for commands from sinric.com ...\n");        
      }
      break;
    case WStype_TEXT: {
        Serial.printf("[WSc] get text: %s\n", payload);
        // Example payloads

        // For Switch or Light device types
        // {"deviceId": xxxx, "action": "setPowerState", value: "ON"} // https://developer.amazon.com/docs/device-apis/alexa-powercontroller.html

        // For Light device type
        // Look at the light example in github
          
        DynamicJsonBuffer jsonBuffer;
        JsonObject& json = jsonBuffer.parseObject((char*)payload); 
        String deviceId = json ["deviceId"];     
        String action = json ["action"];
        
        if(action == "setPowerState") { // Switch or Light
            String value = json ["value"];
            if(value == "ON") {
                turnOn(deviceId);
            } else {
                turnOff(deviceId);
            }
        }
        else if (action == "SetTargetTemperature") {
            String deviceId = json ["deviceId"];     
            String action = json ["action"];
            String value = json ["value"];
        }
        else if (action == "test") {
            Serial.println("[WSc] received test command from sinric.com");
        }
      }
      break;
    case WStype_BIN:
      Serial.printf("[WSc] get binary length: %u\n", length);
      break;
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  pinMode(relayPin3, OUTPUT);
  pinMode(relayPin4, OUTPUT);
  pinMode(relayPin5, OUTPUT);
  pinMode(relayPin6, OUTPUT);
  pinMode(relayPin7, OUTPUT);
  pinMode(relayPin8, OUTPUT);
  delay(50);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  digitalWrite(relayPin3, LOW);
  digitalWrite(relayPin4, LOW);
  digitalWrite(relayPin5, LOW);
  digitalWrite(relayPin6, LOW);
  digitalWrite(relayPin7, LOW);
  digitalWrite(relayPin8, LOW);
  
  WiFiMulti.addAP(MySSID, MyWifiPassword);
  Serial.println();
  Serial.print("Connecting to Wifi: ");
  Serial.println(MySSID);  

  // Waiting for Wifi connect
  while(WiFiMulti.run() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  if(WiFiMulti.run() == WL_CONNECTED) {
    Serial.println("");
    Serial.print("WiFi connected. ");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }

  // server address, port and URL
  webSocket.begin("iot.sinric.com", 80, "/");

  // event handler
  webSocket.onEvent(webSocketEvent);
  webSocket.setAuthorization("apikey", MyApiKey);
  
  // try again every 5000ms if connection has failed
  webSocket.setReconnectInterval(5000);   // If you see 'class WebSocketsClient' has no member named 'setReconnectInterval' error update arduinoWebSockets
}

void loop() {
  webSocket.loop();
  
  if(isConnected) {
      uint64_t now = millis();
      
      // Send heartbeat in order to avoid disconnections during ISP resetting IPs over night. Thanks @MacSass
      if((now - heartbeatTimestamp) > HEARTBEAT_INTERVAL) {
          heartbeatTimestamp = now;
          webSocket.sendTXT("H");          
      }
  }   
}

// If you are going to use a push button to on/off the switch manually, use this function to update the status on the server
// so it will reflect on Alexa app.
// eg: setPowerStateOnServer("deviceid", "ON")

// Call ONLY If status changed. DO NOT CALL THIS IN loop() and overload the server. 

void setPowerStateOnServer(String deviceId, String value) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["deviceId"] = deviceId;
  root["action"] = "setPowerState";
  root["value"] = value;
  StreamString databuf;
  root.printTo(databuf);
  
  webSocket.sendTXT(databuf);
}

//eg: setPowerStateOnServer("deviceid", "CELSIUS", "25.0")

// Call ONLY If status changed. DO NOT CALL THIS IN loop() and overload the server. 

void setTargetTemperatureOnServer(String deviceId, String value, String scale) {
  DynamicJsonBuffer jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["action"] = "SetTargetTemperature";
  root["deviceId"] = deviceId;
  
  JsonObject& valueObj = root.createNestedObject("value");
  JsonObject& targetSetpoint = valueObj.createNestedObject("targetSetpoint");
  targetSetpoint["value"] = value;
  targetSetpoint["scale"] = scale;
   
  StreamString databuf;
  root.printTo(databuf);
  
  webSocket.sendTXT(databuf);
}
