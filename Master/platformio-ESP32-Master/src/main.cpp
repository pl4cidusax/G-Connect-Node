#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// wifi 
const char* ssid = "Hehe";
const char* password = "apahayoo";
WiFiClient espClient;
WiFiStatus wifiStatus;

// mqtt 
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "broker.hivemq.com";
const char* mqtt_topic = "test123l";
PubSubClient client(espClient);
String message = "";

void setup() 
{
  Serial.begin(57600);
  Serial1.begin(57600); //lora
  
  // connect to wifi
  wifiStatus = WiFi.begin(ssid, password);
  while (wifiStatus != WL_CONNECTED) 
  {
    Serial.println("Connecting to WiFi...");
    delay(500);
  }
  Serial.println("Connected to WiFi network");
  
  // connect to mqtt broker
  client.setServer(mqtt_server, 1883);  // port : 1883
}

void reconnect() 
{
  while (!client.connected()) 
  {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) 
    {
      Serial.println("Connected to MQTT broker");
    } else 
    {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() 
{
  while (Serial1.available() > 0)
  {
    message = Serial1.readString();
  }
  
  if (!client.connected()) 
  {
    reconnect();
  }
  client.publish(mqtt_topic, message.c_str());  // send string as aarrayy of char
}
