#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN    10   // SDA pin of MFRC522
#define RST_PIN   9    // RST pin of MFRC522
#define SERVO_PIN 6    // Servo signal pin

MFRC522 mfrc522(SS_PIN, RST_PIN);
Servo servo;

String receivedCommand = "";

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  servo.attach(SERVO_PIN);
  servo.write(0); // Initial position (door closed)

  Serial.println("RFID_READY");
}

void loop() {
  // Check for RFID card
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    // Build tag ID string from UID bytes
    String tagID = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) tagID += "0"; // pad with zero
      tagID += String(mfrc522.uid.uidByte[i], HEX);
    }
    tagID.toUpperCase();

    // Send tag to PC
    Serial.println("TAG:" + tagID);

    // Wait for response from Qt app (up to 5 seconds)
    unsigned long startTime = millis();
    bool responded = false;

    while (millis() - startTime < 5000) {
      if (Serial.available()) {
        receivedCommand = Serial.readStringUntil('\n');
        receivedCommand.trim();

        if (receivedCommand == "OPEN") {
          servo.write(30);          // Rotate 30 degrees
          delay(5000);              // Hold open for 5 seconds
          servo.write(0);           // Return to initial position
          Serial.println("DOOR_OPENED");
          responded = true;
          break;

        } else if (receivedCommand == "DENIED") {
          Serial.println("ACCESS_DENIED");
          responded = true;
          break;
        }
      }
    }

    // Timeout with no response
    if (!responded) {
      Serial.println("TIMEOUT");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();

    // Small debounce delay before scanning next card
    delay(1000);
  }
}
