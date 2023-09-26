#include <Arduino.h>

#define FROM_ARDUINO Serial2
#define TO_LORA Serial
#define RXp2 16
#define TXp2 17

unsigned long last_time = 0;
String data;

String receive_data_from_arduino()
{
	String data;
	if (FROM_ARDUINO.available())
	{
		data = FROM_ARDUINO.readStringUntil('@');
		// data += '@';
	}
	return data;
}

void send_data_to_lora(String messages)
{
	TO_LORA.println(messages);
}

void setup()
{
	FROM_ARDUINO.begin(115200); //, SERIAL_8N1, 3, 1);
	TO_LORA.begin(9600);		//,SERIAL_8N1, 16, 17);
								// TO_LORA.begin(9600)//--cek print data dari esp32;
}

void loop()
{
	// receive json data from arduino
	// send json data to master via lora
	data = receive_data_from_arduino();
	//  String data ="asgfhkwegfyui";
	if (millis() - last_time >= 5000)
	{
		send_data_to_lora(data);
		last_time = millis();
	}
	// FROM_ARDUINO.println("sent");
	// delay(1000);
}
