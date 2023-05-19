#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

// SSID/Password
const char* ssid = "Hehe";
const char* password = "apahayoo";

//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "broker.hivemq.com";

String message = "";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi network");
  client.setServer(mqtt_server, 1883);
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  //Serial.print("Message Sent ; ");
  Serial.println(message);
    if (!client.connected()) {
      reconnect();
    }
    char cstr[100];
    message.toCharArray(cstr, 100);
    //client.publish("test123l", "test-String");
    client.publish("test123l", message);
}
