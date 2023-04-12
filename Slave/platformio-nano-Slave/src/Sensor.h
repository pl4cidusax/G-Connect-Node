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

void detection(){
    _rainfall_rate = 5;
    if (_rainfall_rate > rainfall_rate_danger || _soil_moisture > soil_moisture_danger || _humidity > humidity_danger) {
        flood_category = 3;
    } else if ((_humidity > humidity_warning) || (_soil_moisture > soil_moisture_warning)) {
        flood_category = 2; 
    } else{
        flood_category = 1; 
    }
    
    acceleration_magnitude = sqrt(pow(_accel_x, 2) + pow(_accel_y, 2) + pow(_accel_z, 2));
    // if (acceleration_magnitude > acceleration_danger && _soil_moisture > soil_moisture_warning) {
    //     landslide_category = 3; 
    // } else if ((acceleration_magnitude > acceleration_warning && acceleration_magnitude <= acceleration_danger) || (_soil_moisture > soil_moisture_safe && _soil_moisture <= soil_moisture_warning)) {
    //     landslide_category = 2; 
    // } else{
    //     landslide_category = 1; 
    // }
    if (abs(_accel_x) > 1500 || abs(_accel_y) > 1500 && _soil_moisture > soil_moisture_danger) {
        landslide_category = 3; 
    } else if (abs(_accel_x) > 800 || abs(_accel_y) > 800 || _soil_moisture > soil_moisture_warning) {
        landslide_category = 2; 
    } else{
        landslide_category = 1; 
    }
}

// 
void UpdateSensorData(){
    anemo_update();
    dht_update();
    soil_update();
    // get_GPS_Data();
    determineRainFall();
    GetIMUData();
    detection();

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
    sensor_data["flood"] = flood_category;
    sensor_data["land"] = landslide_category;
    // sensor_data["x"] = acceleration_magnitude;
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