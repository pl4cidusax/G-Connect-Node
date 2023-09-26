#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <JSON.h>

StaticJsonDocument<300> json_data;

// #define hotspot
// wifi
#ifdef hotspot
const char *ssid = "wifiarduino";
const char *password = "wifiarduino";
#endif
#ifndef hotspot
const char *ssid = "JOGJA ISTIMEWA";
const char *password = "gantiwae";
#endif
WiFiClient espClient;
// WiFiStatus wifiStatus;
wl_status_t wifiStatus;

// mqtt
// const char* mqtt_server = "192.168.1.144";
const char *mqtt_server = "broker.hivemq.com";
const char *mqtt_topic = "gconnect-sensor";

unsigned long last_time = 0;

PubSubClient client(espClient);
String message = "";
String json_message = "";

void setup()
{
  Serial.begin(115200); // usb serial
  Serial2.begin(9600);  // lora

  // connect to wifi
  // WiFi.mode(WIFI_AP);
  WiFi.disconnect();
  delay(100);
  // Serial.println("scan start");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
  {
    Serial.println("no networks found");
  }
  else
  {
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < n; ++i)
    {
      // Print SSID and RSSI for each network found
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? " " : "*");
      delay(10);
    }
  }
  delay(2000);
  WiFi.begin(ssid, password);
  // wifiStatus = WiFi.begin(ssid, password);
  uint8_t wifi_status = WiFi.waitForConnectResult();
  // while (wifiStatus != WL_CONNECTED)
  while (wifi_status != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    Serial.println(wifi_status);
    delay(500);
  }
  Serial.println("Connected to WiFi network");

  // connect to mqtt broker
  client.setServer(mqtt_server, 1883); // port : 1883
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT broker...");
    while (Serial2.available())
    {
      // Serial.println("Tes");
      message = Serial2.readStringUntil('\n');
      if (message.length() > 0 && message[0] == '#')
      {

        Serial.println(message);
        String received_string[10];
        int value_index = 0;
        char separator = ',';
        int start = 0;

        // message.trim(); // delete \n

        // convert string to int
        for (int i = 0; i <= message.length() + 1; i++)
        {
          if (message[i] == separator || message[i] == '\n')
          {
            received_string[value_index] = message.substring(start, i); // split then convert to int
            Serial.println(received_string[value_index]);
            value_index++;
            start = i + 1;
          }
        }
        json_data["N"] = received_string[1];
        // json_data["ID"] = received_string[2];
        // // json_data["w"] = _wind_speed;
        // json_data["h"] = "" + received_string[3] + "";
        // json_data["t"] = "" + received_string[4] + "";
        // json_data["s"] = "" + received_string[5] + "";
        // // json_data["rr"] = _rain_rate;
        // json_data["p"] = "" + received_string[6] + "";
        // json_data["r"] = "" + received_string[7] + "";
        // json_data["wl"] = "" + received_string[8] + "";
        // json_data["lt"] = latitude;
        // json_data["ln"] = longitude;
        // sensor_data["flood"] = flood_category;
        // sensor_data["land"] = landslide_category;
        // sensor_data["x"] = acceleration_magnitude;
        serializeJson(json_data, json_message);
        json_message.remove(10, 1);

        String json_message_tmp = '{' + "\"N\":" + received_string[1] + '\"' +
                                  ",\"ID\":" + '\"' + received_string[2] + '\"' +
                                  ",\"h\":" + '\"' + received_string[3] + '\"' +
                                  ",\"t\":" + '\"' + received_string[4] + '\"' +
                                  ",\"s\":" + '\"' + received_string[5] + '\"' +
                                  ",\"p\":" + '\"' + received_string[6] + '\"' +
                                  ",\"r\":" + '\"' + received_string[7] + '\"' +
                                  ",\"wl\":" + '\"' + received_string[8] + '\"' + "}";
        json_message_tmp.remove(0, 6);
        json_message += json_message_tmp;

        Serial.println(json_message);
      }
      else
      {
        json_message = "";
      }
      break;
    }
    if (client.connect("ESP32Client"))
    {
      Serial.println("Connected to MQTT broker");
    }
    else
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
  // client.disconnect();
  // while (Serial2.available())
  // {
  //   // Serial.println("Tes");
  //   message = Serial2.readStringUntil('@');
  //   Serial.println(message);
  // }

  if (!client.connected())
  {
    reconnect();
  }
  if (millis() - last_time >= 500)
  {
    client.publish(mqtt_topic, json_message.c_str()); // send string as aarrayy of char
    last_time = millis();
  }
}
