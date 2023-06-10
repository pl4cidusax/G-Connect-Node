#include <Arduino.h>

#define FROM_ARDUINO Serial2
#define TO_LORA Serial
#define RXp2 16
#define TXp2 17

String receive_data_from_arduino()
{
	String data;
	if (FROM_ARDUINO.available())
	{
		data = FROM_ARDUINO.readString();
	}
	return data;
}

void send_data_to_lora(String messages)
{
    TO_LORA.println(messages);
}

void setup(){
    FROM_ARDUINO.begin(9600, SERIAL_8N1, 3, 1);
    TO_LORA.begin(9600, SERIAL_8N1, 16, 17);
}

void loop(){
	//receive json data from arduino
	String data = receive_data_from_arduino();
   	//send json data to master via lora 
   	send_data_to_lora(data);
}
