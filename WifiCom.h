#ifndef WifiCom_h
#define WifiCom_h

#include <Arduino.h>


class WifiCom {
  public:
    WifiCom();
    bool setup();
    int available();
    char read();
};

#endif
