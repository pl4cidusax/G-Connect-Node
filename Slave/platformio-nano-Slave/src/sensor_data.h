#pragma once

#include <Arduino.h>

struct sensorData
{
    float humidity;
    float wind_speed;
    float temperature;
    float rain_rate;
    float soil_moisture;
    float water_level;
    // bool isDry;
    float longitude, latitude;
    float roll;
    float pitch;
};

float _wind_speed;
float _temperature;
float _humidity;
float _rainfall_rate;
float _rain_rate;
float _soil_moisture;
float water_level;
bool isDry;
double longitude, latitude;
int16_t _accel_x;
int16_t _accel_y;
int16_t _accel_z;

// battery status
uint8_t battery_pin = A3;
float r1 = 10000.0; // ganti dengan r (ohm) sebenarnya
float r2 = 10000.0;// ganti dengan r (ohm) sebenarnya
float v_ref = 5.0;
float v_low = 12.0;
float v_high = 12.8;

// //treshold
// float _wind_speed_th = 32.0;
// float _temperature_th = 29.0;
// float _humidity_th = 80;
// float _rainfall_rate_th = 5;
// int _soil_moisture_th = 70;
// // bool isDry;
// double longitude_th, latitude_th;
// int16_t _accel_th = 2000;
int16_t _accel_x_th = 700;
int16_t _accel_y_th = 800;
int16_t _accel_z_th = 4000;

 // threshold levels
float wind_speed_safe = 15;
float wind_speed_warning = 25;
float wind_speed_danger = 50;
float temperature_safe = 35;
float temperature_warning = 29;
float temperature_danger = 25;
float humidity_safe = 60;
float humidity_warning = 83;
float humidity_danger = 90;
float rainfall_rate_safe = 1;
float rainfall_rate_warning = 5;
float rainfall_rate_danger = 10;
int soil_moisture_safe = 500;
int soil_moisture_warning = 700;
int soil_moisture_danger = 800;
float acceleration_safe = 2000;
float acceleration_warning = 5000;
float acceleration_danger = 10000;
float acceleration_magnitude;

int flood_category = 0;
int landslide_category = 0;