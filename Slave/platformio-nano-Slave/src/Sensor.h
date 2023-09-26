#pragma once
#include <Arduino.h>
#include <Scheduler/Scheduler.h>
#include <Scheduler/Task.h>

#include "anemo.h"
#include "humidity.h"
#include "MPU6050.h"
#include "rainfall.h"
#include "water_level.h"
#include "soil_moisture.h"

#include <JSON.h>

#define HEADER 250
#define TAIL 253
StaticJsonDocument<300> json_data;
String newstr;

void SensorLog();

// tasks
class IMUPDATE : public TimingTask
{
public:
    IMUPDATE(uint32_t _rate) : rate(_rate) { updateTime(millis()); }
    virtual void run(uint32_t now)
    {
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
    Blinker(uint32_t _rate) : rate(_rate) { updateTime(millis()); }
    bool led_on = true;
    virtual void run(uint32_t now)
    {
        if (led_on)
        {
            digitalWrite(13, LOW);
            led_on = false;
        }
        else
        {
            digitalWrite(13, HIGH);
            led_on = true;
        }
        tick(rate);
    }

private:
    uint32_t rate;
};

class WATERLEVEL : public TimingTask
{
public:
    WATERLEVEL(uint32_t _rate) : rate(_rate) { updateTime(millis()); }

    virtual void run(uint32_t now)
    {
        water_level_update();
        tick(rate);
    }

private:
    uint32_t rate;
};

class SUPPORT : public TimingTask
{
public:
    SUPPORT(uint32_t _rate) : rate(_rate) { updateTime(millis()); }
    bool led_on = true;
    virtual void run(uint32_t now)
    {
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
    SUPPORT2(uint32_t _rate) : rate(_rate) { updateTime(millis()); }

    virtual void run(uint32_t now)
    {
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
    Logger(uint32_t _rate) : rate(_rate) { updateTime(millis()); }

    virtual void run(uint32_t now)
    {
        SensorLog();
        tick(rate);
    }

private:
    uint32_t rate;
};

void SensorLog()
{
    // _sensordata.wind_speed = _wind_speed;
    // _sensordata.humidity =  _humidity;
    // _sensordata.temperature =  _temperature;
    // _sensordata.soil_moisture =  _soil_moisture;
    // _sensordata.pitch = pitch;
    // _sensordata.rain_rate = _rain_rate;
    // _sensordata.roll = roll;
    // Serial.write(&_sensordata, sizeof(_sensordata));
    /*JSON*/
    // json_data["N"] = "imo";
    // json_data["ID"] = 1;
    // // json_data["w"] = _wind_speed;
    // json_data["h"] = _humidity;
    // json_data["t"] = _temperature;
    // json_data["s"] = _soil_moisture;
    // // json_data["rr"] = _rain_rate;
    // json_data["p"] = pitch;
    // json_data["r"] = roll;
    // json_data["wl"] = water_level;
    // json_data["lt"] = latitude;
    // json_data["ln"] = longitude;
    // sensor_data["flood"] = flood_category;
    // sensor_data["land"] = landslide_category;
    // sensor_data["x"] = acceleration_magnitude;

    String messg = "#,IMO,1," + String(_humidity) + "," + String(_temperature) + "," + String(_soil_moisture) + "," + String(pitch) + "," + String(roll) + "," + String(water_level) + ",";
    // serializeJson(json_data, messg);
    messg += '@';
    Serial.println(messg);
}

//     // uint32_t raw_batt_voltage = analogRead(battery_pin);
//     // float batt_voltage = (float)raw_batt_voltage * v_ref / 1023.0;
//     // int16_t battery_percent = map(batt_voltage, v_low, v_high, 0, 100); //vlow is threshold for 0 percent, vhigh is for 100 percent
//     // // constrain
//     // if (battery_percent < 0) battery_percent = 0;
//     // if (battery_percent > 100) battery_percent = 100;

void detection()
{
    _rainfall_rate = 5;
    if (_rainfall_rate > rainfall_rate_danger || _soil_moisture > soil_moisture_danger || _humidity > humidity_danger)
        flood_category = 3;
    else if ((_humidity > humidity_warning) || (_soil_moisture > soil_moisture_warning))
        flood_category = 2;
    else
        flood_category = 1;

    acceleration_magnitude = sqrt(pow(_accel_x, 2) + pow(_accel_y, 2) + pow(_accel_z, 2));
    // if (acceleration_magnitude > acceleration_danger && _soil_moisture > soil_moisture_warning) {
    //     landslide_category = 3;
    // } else if ((acceleration_magnitude > acceleration_warning && acceleration_magnitude <= acceleration_danger) || (_soil_moisture > soil_moisture_safe && _soil_moisture <= soil_moisture_warning)) {
    //     landslide_category = 2;
    // } else{
    //     landslide_category = 1;
    // }
    if (abs(_accel_x) > 1500 || abs(_accel_y) > 1500 && _soil_moisture > soil_moisture_danger)
    {
        landslide_category = 3;
    }
    else if (abs(_accel_x) > 800 || abs(_accel_y) > 800 || _soil_moisture > soil_moisture_warning)
    {
        landslide_category = 2;
    }
    else
    {
        landslide_category = 1;
    }
}

void SensorInit()
{
    // Init
    Serial.begin(115200);
    // Serial.println("Init start");
    anemo_init();
    dht_init();
    imu_init();
    rainfall_init();
    // GPS_Init();
    soil_init();
    water_level_init();
    pinMode(13, OUTPUT);
    // Serial.println("Init end");

    // Serial.println("Init lagi");
    Blinker blinking(750);
    IMUPDATE imu_update(50);
    WATERLEVEL water_level(150);
    SUPPORT support_sensor(anemo_rate);
    SUPPORT2 support_sensor2(500);
    Logger logger(100);
    Task *tasks[] = {&imu_update, &blinking, &support_sensor, &support_sensor2, &water_level, &logger};
    Scheduler scheduler(tasks, NUM_TASKS(tasks));
    // Serial.println("Init end lagi");
    while (1)
    {
        scheduler.runTasks();
    }
}