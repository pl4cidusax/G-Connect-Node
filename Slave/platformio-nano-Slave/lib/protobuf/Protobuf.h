#pragma once
#include "msg.pb.h"

#include "pb_common.h"
#include "pb.h"
#include "pb_decode.h"
#include "pb_encode.h"

class Protobuf{
    public:
        Protobuf();
        void Send_data(float co2, int32_t co);
        void Decode_data();
    // private:
        SensorData _sensorData;
        uint8_t oBuffer[1001];
        uint8_t iBuffer[1001];
        pb_ostream_t oStream;
        pb_istream_t iStream;
};

// extern Protobuf proto;
Protobuf::Protobuf(){
    _sensorData = SensorData_init_zero;
}

void Protobuf::Send_data(float co2, int32_t co){
    _sensorData._co2 = co2;
    _sensorData.has__co = true;
    _sensorData._co = co;

    pb_ostream_t stream = pb_ostream_from_buffer(oBuffer, sizeof(oBuffer));
	pb_encode(&stream, SensorData_fields, &_sensorData);
}

void Protobuf::Decode_data(){
    iStream = pb_istream_from_buffer(iBuffer, sizeof(iBuffer));
    pb_decode(&iStream, SensorData_fields, &_sensorData);
}
