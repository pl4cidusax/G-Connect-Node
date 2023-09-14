#ifndef rainfall_h
#define rainfall_h

#include <Arduino.h>



/*
Curah hujan 1 mm adl jml air hujan yang jatuh di permukaan per satuan luas (m2) dengan volume sbnyk 1 liter tanpa ada yang menguap, meresap/mengalir.

Tinggi curah hujan (cm) = volume yang dikumpulkan (mL) / area pengumpulan (cm2)
Luas kolektor 8,7cm x 3,45cm = 30,015 cm2
Koleksi per ujung tip kami dapat dengan cara menuangkan 100ml air ke kolektor kemudian menghitung berapa kali air terbuang dari tip,
Dlm perhitungan yang kami lakukan air terbuang sebanyak 70 kali. 100ml / 70= 1.42mL per tip.
Jadi 1 tip bernilai 1.42 / 30,015 = 0,047cm atau 0.47 mm curah hujan.

- Per tip 0,47 mm.
*/

// enum RainLevel
// {
//     _HIGH,
//     _MID,
//     _LOW,
//     _NONE
// };
// int rain_level;
#define MILLIS_TO_DAY 86400000
#define rain_gauge_const 0.047 // mm curah hujan
uint8_t rain_pin = 3;
// uint8_t midpin = A0;
// uint8_t lowpin = A1;

uint16_t tip; 
uint32_t t_rain_last = 0; 

void tip_count()
{
    if (digitalRead(rain_pin) == LOW) tip++;
    if (millis() - t_rain_last > 86400000)
    {
        t_rain_last = millis();
        tip = 0;
    }
}

void rainfall_init()
{
    pinMode(rain_pin, INPUT);
    attachInterrupt(digitalPinToInterrupt(rain_pin), tip_count, CHANGE);
    // pinMode(midpin, INPUT);
    // pinMode(lowpin, INPUT);
}

float rainRate()
{
    return tip * rain_gauge_const; // per minute
}
// void determineRainFall()
// {
//     if(digitalRead(highpin) == LOW && digitalRead(midpin) == LOW && digitalRead(lowpin) == LOW)
//     {
//         rain_level = RainLevel::_NONE;
//     }
//     else if(digitalRead(highpin) == LOW && digitalRead(midpin) == LOW && digitalRead(lowpin) == HIGH)
//     {
//         rain_level = RainLevel::_LOW;
//     }
//     else if(digitalRead(highpin) == LOW && digitalRead(midpin) == HIGH && digitalRead(lowpin) == HIGH)
//     {
//         rain_level = RainLevel::_MID;
//     }
//     else if(digitalRead(highpin) == HIGH && digitalRead(midpin) == HIGH && digitalRead(lowpin) == HIGH)
//     {
//         rain_level = RainLevel::_HIGH;
//     }
// }

#endif