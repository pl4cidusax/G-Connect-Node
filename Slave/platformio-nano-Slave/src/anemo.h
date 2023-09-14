#pragma once

#include <Arduino.h>
#include <delay_m.h>
#include "sensor_data.h"

#define ANEMO_PIN 2
// constexpr float circumferences = 50.27;
constexpr float constant = (50.27 * 0.01) / 18;
unsigned long anemo_rate = 1000;
int count_anemo = 0;


void IR_count()
{
    if (digitalRead(ANEMO_PIN) == LOW)
        count_anemo++;
}

void anemo_init()
{
    pinMode(ANEMO_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ANEMO_PIN), IR_count, FALLING);
}

void anemo_update()
{
    // _wind_speed = count_anemo * 8.75 / 100;    //m/s
    _wind_speed = count_anemo * constant;
    count_anemo = 0;
    // / 18
}
