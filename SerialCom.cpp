
#include <Arduino.h>

#include "SerialCom.h"

SerialCom::SerialCom() {
}

bool SerialCom::setup() {
  return true;
}

int SerialCom::available() {
  return Serial.available();
}

char SerialCom::read() {
  return (char)Serial.read();
}
