#ifndef RTCController_h
#define RTCController_h

#include "Arduino.h"
#include "DS1302.h"

class RTCController {
  private:
    DS1302* rtc;

  public:
    RTCController(void);
    void setTime(Time time);
    Time getTime();
};

#endif