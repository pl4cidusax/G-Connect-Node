#pragma once

#include <Arduino.h>
#include "anemo.h"
#include "humidity.h"
#include "GPS_NEO6M_V2.h"
#include "MPU6050.h"
#include "rainfall.h"
#include "soil_moisture.h"
#include "sensor_data.h"
#include <JSON.h>

StaticJsonDocument<200> sensor_data;
String desc;

void SensorInit(){
    anemo_init();
    rainfall_init();
    dht_init();
    MPU6050_Init();
    soil_init();
    // GPS_Init();
}
// 
void UpdateSensorData(){
    anemo_update();
    dht_update();
    soil_update();
    // get_GPS_Data();
    determineRainFall();
    GetIMUData();

/*JSON*/
    sensor_data["wind"] = _wind_speed;
    sensor_data["hum"] = _humidity;
    sensor_data["temp"] = _temperature;
    sensor_data["soil"] = _soil_moisture;
    sensor_data["rain"] = rain_level;
    sensor_data["lat"] = latitude;
    sensor_data["lon"] = longitude;
    sensor_data["acx"] = AcX;
    sensor_data["acy"] = AcY;
    sensor_data["acz"] = AcZ;
    serializeJson(sensor_data, Serial);
    Serial.println();

/*Custom format*/
// Serial.print(_wind_speed);
// Serial.print("#");
// Serial.print(_humidity);
// Serial.print("#");
// Serial.print(_temperature);
// Serial.print("#");
// Serial.print(_soil_moisture);
// Serial.print("#");
// // Serial.print(latitude);
// // Serial.print("#");
// // Serial.print(longitude);
// // Serial.print("#");
// Serial.print(AcX);
// Serial.print("#");
// Serial.print(AcY);
// Serial.print("#");
// Serial.print(AcZ);
// Serial.print("#");
// Serial.println();

}

/*Optional
class SendData:public TimingTask
{
    public :
    SendData(uint32_t _rate):rate(_rate){updateTime(millis());}
    virtual void run(){
        send_data_serial();
    }
    private :
    uint32_t rate;
};
*/