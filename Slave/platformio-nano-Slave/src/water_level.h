#pragma once

#include <Arduino.h>
#include "delay_m.h"
#include <SoftwareSerial.h>
#include "sensor_data.h"

#define RANGEr_RX 11
#define RANGEr_TX 12
SoftwareSerial ranger(12, 11); // RX, TX

unsigned char data[4] = {};
float distance;

void water_level_init()
{
  ranger.begin(9600);
}

void water_level_update()
{
  do {
    for (int i = 0; i < 4; i++)
    {
      data[i] = ranger.read();
    }
  } while (ranger.read() == 0xff);

  ranger.flush();

  if (data[0] == 0xff)
  {
    int sum;
    sum = (data[0] + data[1] + data[2]) & 0x00FF;
    if (sum == data[3])
    {
      distance = (data[1] << 8) + data[2];
      if (distance > 280)
      {
        water_level = distance / 10;
        // _sensordata.water_level = water_level;
      } else
      {
        // Serial.println("Below the lower limit");
      }
    } //else Serial.println("ERROR");
  }
  DelayMillis(150);
}

// void update_water_level() {
//   //send a 10 microseconds pulse
//   digitalWrite(TRIG_PIN, LOW);
//   DelayMicros(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   DelayMicros(10);
//   digitalWrite(TRIG_PIN, LOW);
//   //duration of trig pulse to echo
//   long duration = pulseIn(ECHO_PIN, HIGH);

//   // calculate the distance based on the speed of sound
//   water_level = duration * 0.034 / 2;
// }

