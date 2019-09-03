#include <Arduino.h>
#include <LoRaMESH.h>

#define LED 2

// button
#define PB 5
uint8_t prev_state = LOW;

// Set loRaMESH device Tx, Rx & Baudrate
#define LoRaCMD_TX 6
#define LoRaCMD_RX 7
#define LoRaTPR_TX 8
#define LoRaTPR_RX 9
#define LoRaBaudrate 9600

bool isMaster = false;

// Local LoRa INFO
SoftwareSerial* serial_Cmd = NULL;
SoftwareSerial* Serial_Tpr = NULL;
uint16_t local_Id;
uint16_t local_Net;
uint32_t local_UniqueId;

// Remote LoRa INFO
uint16_t remote_Id;
uint8_t bufferPayload[MAX_PAYLOAD_SIZE] = {0};
uint8_t buffer_size = 0;

// payload
uint16_t funciona = 2;
uint8_t payload = 1;

// Functions
bool posedge(uint8_t button, uint8_t *prev);

void setup() {
    delay(1000);
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
    pinMode(PB, INPUT);
    pinMode(10, OUTPUT);
    digitalWrite(10, HIGH);
    serial_Cmd = SerialCommandsInit(LoRaCMD_TX, LoRaCMD_RX, LoRaBaudrate);
    Serial_Tpr = SerialTranspInit(LoRaTPR_TX, LoRaTPR_RX, LoRaBaudrate);
    if(LocalRead(&local_Id, &local_Net, &local_UniqueId) != MESH_OK){
        Serial.print("Error reading LoRa Info\n");
    }
    else {
        Serial.print("Local ID: ");
        Serial.println(local_Id);
        Serial.print("Local NET: ");
        Serial.println(local_Net);
        Serial.print("Local Unique ID: ");
        Serial.println(local_UniqueId, HEX);
        if(local_Id == 0) {
            isMaster = true;
            Serial.println("Set as a master");
        }
    }
}

void loop() {
    if(isMaster){
        if(posedge(PB, &prev_state)){
            Serial.println("PB detected...");
            if(PrepareFrameTransp(funciona, &payload, sizeof(payload)) != MESH_OK){
                Serial.println("Error building msg frame");
            }
            else {
                Serial.print("Sending frame to ID: ");
                Serial.println(funciona);
                SendPacket();
            }
        }
        delay(1);
    }
    else {
        Serial.println("waiting for data...");
        if(ReceivePacketTransp(&remote_Id, bufferPayload, &buffer_size, 5000) == MESH_OK){
            Serial.print("FRAME: ");
            for(int i = 0; i < buffer_size; i++){
                Serial.print(bufferPayload[i]);
            }
            Serial.print(" SIZE: ");
            Serial.println(buffer_size);
            if(bufferPayload[0] == 1){
                digitalWrite(LED, !digitalRead(LED));
            }
        }
        delay(1);
    }
}

bool posedge(uint8_t button, uint8_t *prev){
    uint8_t current = digitalRead(button);
    if(current != *prev && current == HIGH){
        *prev = current;
        return true;
    }
    *prev = current;
    return false;
}