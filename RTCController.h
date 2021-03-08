#ifndef RTCController_h
#define RTCController_h

#include "Arduino.h"
#include "DS1302.h"

class RTCController {
  public:
    RTCController(void);
    void setTime(Time time);
    Time getTime();
};

#endif