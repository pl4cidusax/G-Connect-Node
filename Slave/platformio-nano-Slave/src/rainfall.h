#ifndef rainfall_h
#define rainfall_h

#include <Arduino.h>

enum RainLevel
{
    _HIGH,
    _MID,
    _LOW,
    _NONE
};
int rain_level;
uint8_t highpin = 13;
uint8_t midpin = A0;
uint8_t lowpin = A1;

void rainfall_init()
{
    pinMode(highpin, INPUT);
    pinMode(midpin, INPUT);
    pinMode(lowpin, INPUT);
}

void determineRainFall()
{
    if(digitalRead(highpin) == LOW && digitalRead(midpin) == LOW && digitalRead(lowpin) == LOW)
    {
        rain_level = RainLevel::_NONE;
    }
    else if(digitalRead(highpin) == LOW && digitalRead(midpin) == LOW && digitalRead(lowpin) == HIGH)
    {
        rain_level = RainLevel::_LOW;
    }
    else if(digitalRead(highpin) == LOW && digitalRead(midpin) == HIGH && digitalRead(lowpin) == HIGH)
    {
        rain_level = RainLevel::_MID;
    }
    else if(digitalRead(highpin) == HIGH && digitalRead(midpin) == HIGH && digitalRead(lowpin) == HIGH)
    {
        rain_level = RainLevel::_HIGH;
    }
}

#endif