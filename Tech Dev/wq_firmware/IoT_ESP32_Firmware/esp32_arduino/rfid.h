/*
   --------------------------------------------------------------------------------------------------------------------
   Example sketch/program for MFRC522 Abstraction and RFID UID extraction
   --------------------------------------------------------------------------------------------------------------------
   Digital Forest Sdn Bhd
   Copyright (c) Digital Forest 2020. All rights reserved.
   https://digitalforest.io
   Author: Palok Biswas
   Sensor data are available at https://hydrone.digitalforest.io/admin/login
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         22          // Configurable, see typical pin layout above
#define SS_PIN          5         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//*****************************************************************************************//
void rfid_init() {
  SPI.begin();                                                  // Init SPI bus
  mfrc522.PCD_Init();                                              // Init MFRC522 card
  //Serial.println(F("Read personal data on a MIFARE PICC:"));    //shows in serial that it is ready to read
}

//*****************************************************************************************//
bool rfid_get_id(char _id[20]) {

  // Look for new cards
  if (  mfrc522.PICC_IsNewCardPresent()) {
    mfrc522.PICC_ReadCardSerial();
    sprintf(_id, "%x%x%x%x", mfrc522.uid.uidByte[0], mfrc522.uid.uidByte[1], mfrc522.uid.uidByte[2], mfrc522.uid.uidByte[3]);

    Serial.println("Card detected, UID: "); 
    return true;
  }
  return false;
}
