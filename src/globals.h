#ifndef GLOBALS_H
#define GLOBALS_H

#include <FreeRTOS.h>

#include <cstddef>
#include <cstdint>

#include <utility>
#include <vector>

extern UBaseType_t QUEUE_LENGTH;

extern uint8_t const VS1053_CS;
extern uint8_t const VS1053_DCS;
extern uint8_t const VS1053_DREQ;
extern uint8_t const VOLUME;

//  http://stream-uk1.radioparadise.com:80/mp3-128
extern char const* HOST;
extern char const* PATH;
extern int const HTTP_PORT;

extern std::vector<std::pair<char const*, char const*>> const wifiCredentials;

#endif /* GLOBALS_H */
