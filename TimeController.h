#ifndef TimeController_h
#define TimeController_h

#include "Arduino.h"
#include "DS1302.h"

class TimeController
{
private:
    bool saveTimeFlag;
    String mapDayToString(Time::Day day);
    String mapMonthToString(uint8_t month);
    Time::Day mapNumberToDay(uint8_t number);

public:
    TimeController(void);
    String getDisplayTime(Time time);
    String getDisplayDateTime(Time time);
    Time mapBLEMessageToTime(String bleMessage);
    bool getSaveTimeFlag();
    void setSavedFlag();
    void setUnsavedFlag();
};

#endif