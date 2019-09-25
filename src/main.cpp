#include <avr/sleep.h>//this AVR library contains the methods that controls the sleep modes
#include <LoRaMESH.h>
#include <Arduino.h>

#define coleiraPin 2 //Pin we are going to use to wake up the Arduino
#define rxPinTransp 6
#define txPinTransp 7
#define rxPinCom 8
#define txPinCom 9

#define baudRate 9600

#define nodeMasterCollar 1

uint16_t remoteid;
uint8_t dataReceive;
uint8_t dataReceiveSize;
uint16_t timeout = 5000;

uint16_t id;
uint16_t net;
uint32_t uniqueId;

uint8_t dataSend =1;

void CollarViolation(){
  Serial.println("Collar Violation Fired");//Print message to serial monitor
  if(PrepareFrameTransp(nodeMasterCollar, &dataSend, sizeof(dataSend)) == MESH_OK){
    SendPacket();
  }
}

void Going_To_Sleep(){
  sleep_enable();//Enabling sleep mode
  attachInterrupt(digitalPinToInterrupt(coleiraPin), CollarViolation, LOW);//attaching a interrupt to pin d2
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);//Setting the sleep mode, in our case full sli9nu9oeep
  digitalWrite(LED_BUILTIN,LOW);//turning LED off
  Serial.println("Sleeping");
  delay(1000); //wait a second to allow the led to be turned off before going to sleep
  sleep_cpu();//activating sleep mode
  Serial.println("just woke up!");//next line of code executed after the interrupt
  while(dataReceive != 3){
    Serial.println("Waiting for Collar close");
    ReceivePacketTransp(&remoteid, &dataReceive, &dataReceiveSize, timeout);
  }
  Serial.println("Collar Closed.");
  dataReceive = 0;
  
  digitalWrite(LED_BUILTIN,HIGH);//turning LED on
}

void setup() {
  Serial.begin(9600);//Start Serial Comunication
  pinMode(LED_BUILTIN,OUTPUT); //We use the led on pin 13 to indecate when Arduino is A sleep
  pinMode(coleiraPin,INPUT); //Set pin d2 to input
  digitalWrite(LED_BUILTIN,HIGH);//turning LED on
  SerialTranspInit(rxPinTransp, txPinTransp, baudRate);
  SerialCommandsInit(rxPinCom, txPinCom, baudRate);
  if(LocalRead(&id, &net, &uniqueId) == MESH_OK){
    Serial.println(id);
    Serial.println(net);
    Serial.println(uniqueId, HEX);
  }
}

void loop() {
 delay(5000);//wait 5 seconds before going to sleep
 Going_To_Sleep();
}





