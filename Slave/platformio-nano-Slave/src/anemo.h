#pragma once

#include <Arduino.h>
#include <delay_m.h>
#include "sensor_data.h"
// #include "Scheduler/Scheduler.h"
// #include "Scheduler/Task.h"

#define ANEMO_PIN 3
// float wind_speed;
unsigned long delayTime_anemo = 1000;
int count_anemo = 0;

void IR_count(){
    if (digitalRead(ANEMO_PIN) == LOW)
        count_anemo++;
}

void anemo_init(){
    pinMode(ANEMO_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(ANEMO_PIN), IR_count, FALLING);

}

void anemo_update(){
    DelayMillis(delayTime_anemo);
    _wind_speed = count_anemo * 8.75 / 100;    //m/s
    count_anemo = 0;
}


/*  use scheduling
class Anemo : public TimingTask
{
    public:
    Anemo(uint32_t _rate):rate(_rate){updateTime(millis());}
    virtual void run(){
        anemo_update();
        tick(rate);
    }
    private:
    uint32_t rate;
};
Anemo anemo_task(1100);
*/