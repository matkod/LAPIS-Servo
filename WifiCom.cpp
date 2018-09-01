
#include <Arduino.h>

#include "WifiCom.h"

WifiCom::WifiCom() {
}

bool WifiCom::setup() {
  
}

int WifiCom::available() {
  return Serial.available();
}

char WifiCom::read() {
  return (char)Serial.read();
}
