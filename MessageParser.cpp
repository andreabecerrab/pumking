#include "MessageParser.h"
#include <string.h>

MESSAGETYPE getMessageType(const String &msg) {
    if (msg == "START") return MESSAGETYPE::START;
    if (msg == "SEEN") return MESSAGETYPE::SEEN;
    if (msg == "WATER") return MESSAGETYPE::WATER;
    if (msg == "FERTILIZE") return MESSAGETYPE::FERTILIZE;
    if (msg == "PRUNING") return MESSAGETYPE::PRUNING;
    return MESSAGETYPE::NOTFOUND;
}

MessageParser getMessage(String inputtedStr) {
  MessageParser result;

  char charArr[50];
  inputtedStr.toCharArray(charArr, 50);

  char* ptr = strtok(charArr, "||");
  result.message = ptr ? getMessageType(String(ptr)) : MESSAGETYPE::NOTFOUND;

  ptr = strtok(NULL, "||");
  result.value = ptr ? String(ptr) : "";

  return result;
}

