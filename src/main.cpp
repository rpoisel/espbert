#include "SampleMp3.h"

#include <SPI.h>
#if 0
#include <MFRC522.h>
#include <SdFat.h>
#else
#include <VS1053.h>
#endif

#if 0
MFRC522 mfrc522(16 /* SS_PIN */, 17 /* RST_PIN */);
SdFat sd;
#else
VS1053 player(32 /* VS1053_CS */, 33 /* VS1053_DCS */, 34 /* VS1053_DREQ */);
#endif

void setup()
{
  Serial.begin(115200);
  while(!Serial)
  {
  }
  SPI.begin();

#if 0
  mfrc522.PCD_Init();
  sd.begin(0 /* CS_PIN */);
#else
  player.begin();
  player.switchToMp3Mode();
  player.setVolume(100 /* VOLUME */);

#endif
}

void loop()
{
  static size_t cnt = 0;
#if 0
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
  {
    return;
  }

  Serial.print("New card detected: ");
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
#else
  player.playChunk(sampleMp3, sizeof(sampleMp3));
  Serial.print(cnt++);
  Serial.print(": played chunk of size ");
  Serial.print(sizeof(sampleMp3));
  Serial.println(" bytes.");
  delay(500);
#endif
}
