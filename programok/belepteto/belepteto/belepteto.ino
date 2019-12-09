/* Beléptető rendszer készítése 1.

   Készítette: Kotán Tamás Balázs - Magyar Arduino Labor©

   Dátum: Győr, 2016.01.14.
*/



#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

Servo daServo;

#define szervomotor 5

#define RST_PIN         9

#define SS_1_PIN        10

#define NR_OF_READERS  1

byte ssPins[] = {SS_1_PIN};

MFRC522 mfrc522[NR_OF_READERS];

int a = 0;





void setup() {

  Serial.begin(115200);

  daServo.attach(szervomotor);
  servo(50);

  SPI.begin();

  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)
  {
    mfrc522[reader].PCD_Init(ssPins[reader], RST_PIN);
  }
}

void loop()
{
  for (uint8_t reader = 0; reader < NR_OF_READERS; reader++)

  {

    if (mfrc522[reader].PICC_IsNewCardPresent() && mfrc522[reader].PICC_ReadCardSerial())
    {
      Serial.print("Reader ");
      Serial.print(reader);
      Serial.println(":");

      Serial.print("Card UID:");
      dump_byte_array(mfrc522[reader].uid.uidByte, mfrc522[reader].uid.size);
      Serial.println();
      Serial.print("PICC type: ");
      MFRC522::PICC_Type piccType = mfrc522[reader].PICC_GetType(mfrc522[reader].uid.sak);
      Serial.println(mfrc522[reader].PICC_GetTypeName(piccType));
      Serial.println();

      if (mfrc522[reader].uid.uidByte[0] == 89 &
          mfrc522[reader].uid.uidByte[1] == 104 &
          mfrc522[reader].uid.uidByte[2] == 156 &
          mfrc522[reader].uid.uidByte[3] == 26 &
          reader == 0 )
      {
        Serial.println("0-s kartya oke");
        a++;
      }
      if (mfrc522[reader].uid.uidByte[0] != 89 &
          mfrc522[reader].uid.uidByte[1] != 104 &
          mfrc522[reader].uid.uidByte[2] != 156 &
          mfrc522[reader].uid.uidByte[3] != 26 &
          reader != 0)
      {
        Serial.println("nem 0-s kartya");
        a = 0;
      }


      Serial.println();

      mfrc522[reader].PICC_HaltA();
      mfrc522[reader].PCD_StopCrypto1();
    }
  }

  if (a > 0)
  {
    servo(150);
    Serial.println("NYITVA");
    delay(3000);
    servo(50);
    Serial.println("ZARVA");
    a = 0;
  }

}



///////////////////////////////SZERVÓ//////////////////////////////////////
void servo(int datPos)
{
  daServo.write(datPos);
  delay(15);
}


////////////////Az UID kiiratása DECimális számokkal///////////////////////

void dump_byte_array(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}
