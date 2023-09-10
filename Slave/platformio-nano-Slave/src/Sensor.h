#pragma once
#include <Arduino.h>
#include <Scheduler/Scheduler.h>
#include <Scheduler/Task.h>
#include <JSON.h>

#include "anemo.h"
#include "humidity.h"
#include "MPU6050.h"
#include "rainfall.h"
#include "water_level.h"
#include "soil_moisture.h"

#include "gconnect.pb.h"

#include "pb_common.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

#define HEADER 250
#define TAIL 253 

class nanoProtobuf{
    public:
        nanoProtobuf();
        void Send_data(float hum, float windsp, float temp, float rainrt, float soilmst, float waterlvl, float lon, float lat, float _roll, float _pitch);
        void Decode_data();
    // private:
        SensorReading _sensorData;
        uint8_t oBuffer[1001];
        uint8_t iBuffer[1001];
        pb_ostream_t oStream;
        pb_istream_t iStream;
};

nanoProtobuf slave;

StaticJsonDocument<200> sensor_data;
sensorData _sensordata;

void SensorLog(); 
void detection();
//tasks
class IMUPDATE : public TimingTask
{
    public :
        IMUPDATE(uint32_t _rate):rate(_rate){updateTime(millis());}
        virtual void run(uint32_t now){
            MPU6050GetData();
            UpdateQuaternion();
            Now = micros();
            sampleFreq = (1000000.0f / (Now - lastUpdate));
            lastUpdate = Now;
            MahonyAHRSupdateIMU(gxrs, gyrs, gzrs, axg, ayg, azg);
            GetRollPitchYaw();
            tick(rate);
        }
    private:
        uint32_t rate;
};

class Blinker : public TimingTask
{
    public:
        Blinker(uint32_t _rate):rate(_rate){updateTime(millis());}
        bool led_on = true;
        virtual void run(uint32_t now){
            if (led_on){digitalWrite(13, LOW); led_on = false;}
            else{digitalWrite(13, HIGH); led_on = true;}
            tick(rate);
        } 
    private:
        uint32_t rate;
};

class SUPPORT : public TimingTask
{
    public:
        SUPPORT(uint32_t _rate):rate(_rate){updateTime(millis());}
        bool led_on = true;
        virtual void run(uint32_t now){
            anemo_update();
            // water_level_update();
            tick(rate);
        } 
    private:
        uint32_t rate;
};

class SUPPORT2 : public TimingTask
{
    public:
        SUPPORT2(uint32_t _rate):rate(_rate){updateTime(millis());}

        virtual void run(uint32_t now){
            dht_update();
            soil_update();
            _rain_rate = rainRate();
            tick(rate);
        } 
    private:
        uint32_t rate;
};

class Logger : public TimingTask
{
    public:
        Logger(uint32_t _rate):rate(_rate){updateTime(millis());}

        virtual void run(uint32_t now){
            SensorLog();
            slave.Send_data(
                _humidity,
                _wind_speed,
                _temperature,
                _rain_rate,
                _soil_moisture,
                water_level,
                longitude,
                latitude,
                roll,
                pitch
            );
            tick(rate);
        } 
    private:
        uint32_t rate;
};

void SensorInit() 
{
    // Init
    anemo_init();
    dht_init();
    imu_init();
    rainfall_init();
    // GPS_Init();
    soil_init();
    water_level_init();
    pinMode(13, OUTPUT);

    Blinker blinking(750);
    IMUPDATE imu_update(50);
    SUPPORT support_sensor(anemo_rate);
    SUPPORT2 support_sensor2(500);
    Logger logger(100);
    Task *tasks[] = {&imu_update, &blinking, &support_sensor, &support_sensor2, &logger};
    Scheduler scheduler(tasks, NUM_TASKS(tasks));
    while(1){scheduler.runTasks();}
}

void SensorLog()
{
/*JSON*/
    sensor_data["w"] = _wind_speed;
    sensor_data["h"] = _humidity;
    sensor_data["t"] = _temperature;
    sensor_data["s"] = _soil_moisture;
    sensor_data["rr"] = _rain_rate;
    // sensor_data["lt"] = latitude;
    // sensor_data["ln"] = longitude;
    sensor_data["p"] = pitch;
    sensor_data["r"] = roll;
    sensor_data["wl"] = water_level;
    // sensor_data["flood"] = flood_category;
    // sensor_data["land"] = landslide_category;
    // sensor_data["x"] = acceleration_magnitude;
    String newstr;
    serializeJson(sensor_data, newstr);
    newstr += '@';
    Serial.println(newstr);

    _sensordata.wind_speed = _wind_speed;
    _sensordata.humidity =  _humidity;
    _sensordata.temperature =  _temperature;
    _sensordata.soil_moisture =  _soil_moisture;
    _sensordata.pitch = pitch;
    _sensordata.rain_rate = _rain_rate;
    _sensordata.roll = roll;
    // Serial.write(&_sensordata, sizeof(_sensordata));
}

void SensorLoger()
{
    // Serial.write();
}

nanoProtobuf::nanoProtobuf(){
    _sensorData = SensorReading_init_zero;
}

void nanoProtobuf::Send_data
(
    float hum,
    float windsp,
    float temp,
    float rainrt,
    float soilmst,
    float waterlvl,
    float lon,
    float lat,
    float _roll,
    float _pitch
)
{
    _sensorData.humidity = hum;
    _sensorData.wind_speed = windsp;
    _sensorData.temperature = temp;
    _sensorData.soil_moisture;
    _sensorData.water_level = waterlvl;
    _sensorData.longitude = lon;
    _sensorData.latitude = lat;
    _sensorData.roll = _roll;
    _sensorData.pitch = _pitch;

    oStream = pb_ostream_from_buffer(oBuffer, sizeof(oBuffer));
	pb_encode(&oStream, SensorReading_fields, &_sensorData);
	Serial.write(HEADER);
	for (size_t i = 0; i < oStream.bytes_written; i++)
	{
		Serial.write(oBuffer[i]);
	}
	Serial.write(TAIL);
}

void nanoProtobuf::Decode_data(){
    iStream = pb_istream_from_buffer(iBuffer, sizeof(iBuffer));
    pb_decode(&iStream, SensorReading_fields, &_sensorData);
}

//     // uint32_t raw_batt_voltage = analogRead(battery_pin);
//     // float batt_voltage = (float)raw_batt_voltage * v_ref / 1023.0;
//     // int16_t battery_percent = map(batt_voltage, v_low, v_high, 0, 100); //vlow is threshold for 0 percent, vhigh is for 100 percent 
//     // // constrain
//     // if (battery_percent < 0) battery_percent = 0;
//     // if (battery_percent > 100) battery_percent = 100;

void detection(){
    _rainfall_rate = 5;
    if (_rainfall_rate > rainfall_rate_danger || _soil_moisture > soil_moisture_danger || _humidity > humidity_danger) flood_category = 3;
    else if ((_humidity > humidity_warning) || (_soil_moisture > soil_moisture_warning)) flood_category = 2; 
    else flood_category = 1;
    
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








