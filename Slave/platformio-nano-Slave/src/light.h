#pragma once

#include <Arduino.h>
#include <delay_m.h>
#include "sensor_data.h"

#define LIGHT_SENSOR_PIN A0  

// constants for calibration
constexpr float calibration_factor = 1.0;  

unsigned long light_sensor_rate = 1000;
int light_readings = 0;
float light_intensity = 0.0;

// initialize light sensor
void light_sensor_init()
{
    pinMode(LIGHT_SENSOR_PIN, INPUT);
}

// update the light intensity value
void light_sensor_update()
{
    light_readings = analogRead(LIGHT_SENSOR_PIN);
    
    // convert to light intensity value (lux)
    light_intensity = light_readings * calibration_factor;
}

float get_light_intensity()
{
    return light_intensity;
}

//AD
