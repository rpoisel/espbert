/**
  A simple stream handler to play web radio stations using ESP8266

  Copyright (C) 2018 Vince Gellár (github.com/vincegellar)
  Licensed under GNU GPL v3

  Wiring:
  --------------------------------
  | VS1053  | ESP8266 |  Other   |
  --------------------------------
  |   SCK   |   D5    |    -     |
  |   MISO  |   D6    |    -     |
  |   MOSI  |   D7    |    -     |
  |   XRST  |    -    |    -     |
  |   CS    |   D1    |    -     |
  |   DCS   |   D0    |    -     |
  |   DREQ  |   D3    |    -     |
  |   5V    |    -    |   VCC    |
  |   GND   |    -    |   GND    |
  --------------------------------

  Dependencies:
  -VS1053 library by baldram (https://github.com/baldram/ESP_VS1053_Library)
  -ESP8266Wifi

  To run this example define the platformio.ini as below.

  [env:nodemcuv2]
  platform = espressif8266
  board = nodemcuv2
  framework = arduino
  build_flags = -D PIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH

  lib_deps =
    ESP_VS1053_Library

  Instructions:
  -Build the hardware
    (please find an additional description and Fritzing's schematic here:
     https://github.com/vincegellar/Simple-Radio-Node#wiring)
  -Set the station in this file
  -Upload the program

  IDE Settings (Tools):
  -IwIP Variant: v1.4 Higher Bandwidth
  -CPU Frequency: 160Hz
*/

#include <FreeRTOS.h>
#include <VS1053.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include <utility>

struct StreamFrame
{
  uint8_t buf[64];
  size_t numBytes;
};

uint8_t const VS1053_CS = 32;
uint8_t const VS1053_DCS = 33;
uint8_t const VS1053_DREQ = 34;
uint8_t const VOLUME = 100;

//  http://stream-uk1.radioparadise.com:80/mp3-128
char const* host = "stream-uk1.radioparadise.com";
char const* path = "/mp3-128";
int const httpPort = 80;

std::pair<char const*, char const*> const wifiCredentials[] = {
    // {"SSID", "PASSWORD"},
};

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);
WiFiClient wifiClient;
WiFiMulti wifiMulti;
auto queue = xQueueCreate(8, sizeof(StreamFrame));

static void setupWIFI(void);
static void TaskHttpReader(void* param);

void setup()
{
  Serial.begin(115200);

  // Wait for VS1053 and PAM8403 to power up
  // otherwise the system might not start up correctly
  delay(3000);

  Serial.println("\n\nSimple Radio Node WiFi Radio");

  SPI.begin();

  setupWIFI();

  player.begin();
  player.switchToMp3Mode();
  player.setVolume(VOLUME);

  xTaskCreate(TaskHttpReader, "HttpReader", 4096 /* Stack size */, nullptr, 2 /* priority */,
              nullptr);
}

void loop()
{
  StreamFrame frame;
  xQueueReceive(queue, &frame, portMAX_DELAY);
  player.playChunk(frame.buf, frame.numBytes);
}

static void TaskHttpReader(void* param)
{
  Serial.print("Connecting ");

  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("connecting to ");
  Serial.println(host);

  if (!wifiClient.connect(host, httpPort))
  {
    Serial.println("Connection failed");
    return;
  }

  Serial.print("Requesting stream: ");
  Serial.println(path);

  wifiClient.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +
                   "Connection: close\r\n\r\n");
  for (;;)
  {
    if (!wifiClient.connected())
    {
      Serial.println("Reconnecting...");
      if (wifiClient.connect(host, httpPort))
      {
        wifiClient.print(String("GET ") + path + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n");
      }
    }
    while (wifiClient.available() > 0)
    {
      StreamFrame frame;
      frame.numBytes = wifiClient.read(frame.buf, sizeof(frame.buf));
      xQueueSend(queue, &frame, portMAX_DELAY);
    }
  }
}

static void setupWIFI(void)
{
  for (auto credentialPair : wifiCredentials)
  {
    wifiMulti.addAP(credentialPair.first, credentialPair.second);
  }
}
