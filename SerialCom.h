#ifndef SerialCom_h
#define SerialCom_h

#include <Arduino.h>

class SerialCom {
  public:
    SerialCom();
    bool setup();
    int available();
    char read();

};


#endif
