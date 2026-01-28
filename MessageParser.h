#ifndef MESSAGE_PARSER_H
#define MESSAGE_PARSER_H
#include <Arduino.h>

enum class MESSAGETYPE {
    START,
    SEEN,
    WATER,
    FERTILIZE,
    PRUNING,
    NOTFOUND
};

struct MessageParser {
  MESSAGETYPE message;
  String value;
};

// Function declaration
MessageParser getMessage(String inputtedStr);
#endif
