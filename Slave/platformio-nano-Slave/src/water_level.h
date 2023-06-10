#pragma once

#include <Arduino.h>
#include "delay_m.h"
#include "sensor_data.h"

#define TRIG_PIN 11
#define ECHO_PIN 12

void update_water_level() {
  //send a 10 microseconds pulse
  digitalWrite(TRIG_PIN, LOW);
  DelayMicros(2);
  digitalWrite(TRIG_PIN, HIGH);
  DelayMicros(10);
  digitalWrite(TRIG_PIN, LOW);
  //duration of trig pulse to echo
  long duration = pulseIn(ECHO_PIN, HIGH);

  // calculate the distance based on the speed of sound
  float water_level = duration * 0.034 / 2;
}