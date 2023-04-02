#include <Arduino.h>
#include <Sensor.h>
#include "Scheduler/Scheduler.h"
#include "Scheduler/Task.h"

void setup()
{
    Serial.begin(57600);
    SensorInit();

    /*With Scheduler*/
    // Task *task[] = {&anemo_task, &gps_task, &humidity_task, &imu_task, &soil_moisture_task};
    // Scheduler scheduler(task, NUM_TASKS(task));
    // while(1){scheduler.runTasks();}
}

void loop(){
    UpdateSensorData(); // sudah termasuk serialisasi dan send data to esp
}