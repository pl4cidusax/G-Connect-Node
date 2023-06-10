#ifndef GPS_NEO6M_H
#define GPS_NEO6M_H

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "sensor_data.h"

static const int RXPin = 8, TXPin = 9;
static const uint32_t GPSBaud = 57600;

TinyGPSPlus gps;
SoftwareSerial gpsSerial(RXPin, TXPin);

void GPS_Init(){
  gpsSerial.begin(GPSBaud);
}

void get_GPS_Data(){
  if (gpsSerial.available() > 0){
    if (gps.encode(gpsSerial.read())){
      if (gps.location.isValid()) {
        longitude = gps.location.lng();
        latitude = gps.location.lat();
      }
    }
  }
}

/*use scheduling
class GPS_TASK : public TimingTask
{
  public:
  GPS_TASK(uint32_t _rate):rate(_rate){updateTime(millis());}
  virtual void run(){
    get_GPS_Data();
  }
  private:
  uint32_t rate;
};
GPS_TASK gps_task(100);

*/

#endif