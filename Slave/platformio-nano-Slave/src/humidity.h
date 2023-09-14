#pragma once

#include <DHT22.h>

dht DHT;

#define DHT22_PIN 10

void dht_init() {
  // startMillis = millis();
}

void dht_update() {
  DHT.read22(DHT22_PIN);
    _humidity  = DHT.humidity;
    // _sensordata.humidity = _humidity;
    // read temperature in Celsius
    _temperature = DHT.temperature;
}