/*
  RFID Access Control with Arduino Nano/Uno
  - Reads UID from RC522 RFID card/tag
  - Compares against list of authorized UIDs
  - If match: green LED + servo unlock + log "approved"
  - If not: red LED + log "denied"
  - Logs UID + status + timestamp (via Serial Monitor for now)

  Hardware:
    RC522 connections:
      SDA  -> D10 (Green)
      SCK  -> D13 (yellow)
      MOSI -> D11 (Blue)
      MISO -> D12 (purple)
      RST  -> D9 (Brown)
      VCC  -> 3.3V (RED)
      GND  -> GND (BLACK)

    Green LED -> D6 (with 220Ω resistor to GND) //Just 6 if UNO
    Red LED   -> D7 (with 220Ω resistor to GND) //Just 7 if UNO 
    Servo     -> D5
*/

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);

Servo doorServo;

const int greenLED = 6;
const int redLED   = 7;
const int servoPin = 5;

// Example: store 2 authorized UIDs (4-byte tags here)
byte authorized[][4] = {
  {0x93, 0x90, 0x79, 0x2D},   // Keychain UID
  {0x7E, 0xF1, 0xFB, 0x02}    // Card UID
};
int authorizedCount = 2;

void setup() {
  Serial.begin(9600);
  SPI.begin(); //Initialized Serial Peripheral Interface Hardware for SCK, MOSI MISO
  mfrc522.PCD_Init();
  Serial.println("Place your RFID card near the reader...");

  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  doorServo.attach(servoPin);
  doorServo.write(0); // locked position
}

void loop() {
  // Look for new card
  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  // Print UID
  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Check authorization
  if (isAuthorized(mfrc522.uid.uidByte, mfrc522.uid.size)) {
    grantAccess(mfrc522.uid.uidByte, mfrc522.uid.size);
  } else {
    denyAccess(mfrc522.uid.uidByte, mfrc522.uid.size);
  }

  // Halt PICC
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

bool isAuthorized(byte *uid, byte uidSize) {
  for (int i = 0; i < authorizedCount; i++) {
    bool match = true;
    for (byte j = 0; j < uidSize; j++) {
      if (authorized[i][j] != uid[j]) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}

void grantAccess(byte *uid, byte uidSize) {
  Serial.print("[");
  Serial.print(millis()); // placeholder for timestamp
  Serial.print("] ACCESS GRANTED to UID: ");
  printUID(uid, uidSize);

  digitalWrite(greenLED, HIGH);
  doorServo.write(90); // unlock
  delay(3000);
  doorServo.write(0);  // lock again
  digitalWrite(greenLED, LOW);
}

void denyAccess(byte *uid, byte uidSize) {
  Serial.print("[");
  Serial.print(millis()); // placeholder for timestamp
  Serial.print("] ACCESS DENIED to UID: ");
  printUID(uid, uidSize);

  Serial.println("Your card doesn't match one of the pre-approved options.");

  digitalWrite(redLED, HIGH);
  delay(2000);
  digitalWrite(redLED, LOW);
}

void printUID(byte *uid, byte uidSize) {
  for (byte i = 0; i < uidSize; i++) {
    Serial.print(uid[i] < 0x10 ? " 0" : " ");
    Serial.print(uid[i], HEX);
  }
  Serial.println();
}



// Read UID from card
// If matches, from array of options: 
// access granted (func to turn on green light; move servo; store UID as approved w/ date & time)
// if doesn't match: 
// access denied (print: "Your card doesn't match one of the pre-approved options; turn on red light"; store UID as denied w/ date & time)
