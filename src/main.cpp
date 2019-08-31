#include <Arduino.h>
#include <LoRaMESH.h>

#define LED 2

// Set loRaMESH device Tx, Rx & Baudrate
#define LoRaCMD_TX 6
#define LoRaCMD_RX 7
#define LoRaBaudrate 9600

// Local LoRa INFO
SoftwareSerial* serial_Cmd = NULL;
uint16_t local_Id;
uint16_t local_Net;
uint32_t local_UniqueId;

void setup() {
    delay(1000);
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    serial_Cmd = SerialCommandsInit(LoRaCMD_TX, LoRaCMD_RX, LoRaBaudrate);
    if(LocalRead(&local_Id, &local_Net, &local_UniqueId) != MESH_OK){
        Serial.print("Error reading Lora info\n");
    }
    else {
        Serial.print("Local ID: ");
        Serial.println(local_Id);
        Serial.print("Local NET: ");
        Serial.println(local_Net);
        Serial.print("Local Unique ID: ");
        Serial.println(local_UniqueId, HEX);
    }
}

void loop() {
    delay(1000);
    digitalWrite(LED, !digitalRead(LED));
}