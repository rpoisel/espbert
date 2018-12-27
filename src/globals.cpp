#include "globals.h"

UBaseType_t QUEUE_LENGTH = 8;

uint8_t const VS1053_CS = 32;
uint8_t const VS1053_DCS = 33;
uint8_t const VS1053_DREQ = 34;
uint8_t const VOLUME = 100;

//  http://stream-uk1.radioparadise.com:80/mp3-128
char const* HOST = "stream-uk1.radioparadise.com";
char const* PATH = "/mp3-128";
int const HTTP_PORT = 80;

std::vector<std::pair<char const*, char const*>> const wifiCredentials = {
    // {"SSID", "PASSWORD"},
};


