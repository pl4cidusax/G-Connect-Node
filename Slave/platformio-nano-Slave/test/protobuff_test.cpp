#include <Arduino.h>
// #include <mqtt.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Protobuf.h>
// #include <pb_encode.h>
// #include <pb_decode.h>
// #include <parameters.pb.h>

Parameters datas = Parameters_init_zero;

const char* ssid = "Sticker";
const char* password = "terserah";

const char *mqtt_broker = "mqtt:broker.hivemq.com";
const char *topic = "test123l";
const char *mqtt_username = "";
const char *mqtt_password = "";
const int mqtt_port = 1883;
char msg[23];
String message;

WiFiClient espclient;
PubSubClient client(espclient);

void sendData(){
  uint8_t oBuffer[100];
  pb_ostream_t oStream = pb_ostream_from_buffer(oBuffer, sizeof(oBuffer));

  if (!pb_encode(&oStream, Parameters_fields, &datas)){
  //  mqtt.publish
    Serial.println("error"); 
  }
  Serial.print(oStream.bytes_written);
  Serial.print("=>");
  // client.publish(topic, oBuffer);
}
void wifi_setup(){
  Serial.println();
  Serial.print("Connecting to ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  client.publish(topic, "publish test");
}

void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");
 for (int i = 0; i < length; i++) {
     Serial.print((char) payload[i]);
 }
 Serial.println();
 Serial.println("-----------------------");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
    wifi_setup();
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
     String client_id = "esp32-client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
         Serial.println("Public emqx mqtt broker connected");
//     } else {
//         Serial.print("failed with state "); 
//         Serial.print(client.state());
//         delay(2000);
//     }
    }

  }
  sendData();
}

void loop() {
  // put your main code here, to run repeatedly:
}