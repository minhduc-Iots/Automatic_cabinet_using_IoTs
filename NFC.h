#include <avr/interrupt.h>
#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h> // thu vien "RFID".

/*
   Reset      9          RST
   SPI SS     10         SDA
   SPI MOSI   11         MOSI
   SPI MISO   12         MISO
   SPI SCK    13         SCK
*/

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);
unsigned long MASO, uidDecTemp; // hien thi so UID dang thap phan
byte bCounter, readBit;
unsigned long ticketNumber;

void SETUP_NFC(){
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Waiting for card...");
  }

unsigned long CHECK_NFC() {
  MASO=0;
  if(!mfrc522.PICC_IsNewCardPresent()) {
  return 0;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return 0;
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    MASO = MASO * 256 + uidDecTemp;
  }
  mfrc522.PICC_HaltA();
  return MASO;
}

void IN_NFC() {
  MASO=0;
  if(!mfrc522.PICC_IsNewCardPresent()) {
  return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidDecTemp = mfrc522.uid.uidByte[i];
    MASO = MASO * 256 + uidDecTemp;
  }
  mfrc522.PICC_HaltA();
  Serial.print("Card ID: ");
  Serial.print("   [");
  Serial.print(MASO);
  Serial.println("]");
}
