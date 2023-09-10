#pragma once

#include <DHT22.h>
// #include "Scheduler/Scheduler.h"
// #include "Scheduler/Task.h"

#define DHT_SENSOR_PIN  10
#define DHT_SENSOR_TYPE DHT22

unsigned long startMillis; 
unsigned long currentMillis;
const unsigned long period = 1000;

// float humi, tempC;

DHT dht_sensor(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void dht_init() {
  dht_sensor.begin();
  startMillis = millis();
}

void dht_update() {
  // currentMillis = millis(); 
  // if (currentMillis - startMillis >= period)
  // {
    // read humidity
    _humidity  = dht_sensor.readHumidity();
    // _sensordata.humidity = _humidity;
    // read temperature in Celsius
    _temperature = dht_sensor.readTemperature();
    // _sensordata.temperature = _temperature;
    // check whether the reading is successful or not
    // if ( isnan(tempC) || isnan(humi)) {
    //   Serial.println("Failed to read from DHT sensor!");
    // } 
    // else {
    //   Serial.print("Humidity: ");
    //   Serial.print(humi);
    //   Serial.print("%");

    //   Serial.print("  |  ");

    //   Serial.print("Temperature: ");
    //   Serial.print(tempC);
    //   Serial.print("°C  ~  ");
      // startMillis = currentMillis;
    //  }   
  // }
}

/*use scheduling
class Humidity : public TimingTask
{
  public:
  Humidity(uint32_t _rate):rate(_rate){updateTime(millis());}
  virtual void run(){
    dht_update();
  }
  private:
  uint32_t rate;
};
Humidity humidity_task(100);

*/