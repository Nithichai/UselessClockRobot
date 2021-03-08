#ifndef LedDisplayController_h
#define LedDisplayController_h

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

class LedDisplayController
{
private:
    Adafruit_8x16matrix* matrix;
    int16_t shiftIndex;
    String dateTimeMessage;
    String timeMessage;

    void slideDateTimeMessage();
    void slideTimeMessage();
    int16_t getDisplayDateTimeLength();
    int16_t getDisplayTimeLength();

public:
    LedDisplayController(void);
    void showDateTime();
    void showTime();
    void setDateTimeMessage(String message);
    void setTimeMessage(String message);
};

#endif