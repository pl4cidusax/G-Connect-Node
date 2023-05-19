#pragma once

#include <Arduino.h>

#define SOIL_PIN A2
#define THRESHOLD 100 // digunakan utk kalibrasi lembab tidaknya

// int soil_moisture_value;
// bool is_dry;

void soil_init(){
  pinMode(SOIL_PIN, INPUT);
}

void soil_update() {
  _soil_moisture = analogRead(SOIL_PIN); // read 
  if (_soil_moisture < THRESHOLD)
    isDry = 1;
  else
    isDry = 0;
}


/*use scheduling
class Soil : public TimingTask
{
  public:
  Soil(uint32_t _rate):rate(_rate){updateTime(millis());}
  virtual void run(){
    soil_update();
  }
  private:
  uint32_t rate;
};
Soil soil_moisture_task(100);

*/