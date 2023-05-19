#include <Arduino.h>
#include <SoftwareSerial.h>

// SoftwareSerial lora(16, 17);
#define lora Serial2

char a;

void receive(){
    // lora.write("AT+RX\r\n");
    Serial.print("Message : ");
    while (lora.available()){
        char msg = lora.read();
        
        Serial.print(msg);
    }
    Serial.println();
}

void send(){
    char data = 'T';
    // lora.write("AT+TX\r\n"); 
    lora.println(data);
}

void setup(){
    Serial.begin(57600);
    lora.begin(57600);
}

void loop(){
   receive();
//    send();
   delay(1000);
}

