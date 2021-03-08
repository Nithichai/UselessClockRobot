#ifndef BLEController_h
#define BLEController_h

#include "Arduino.h"
#include "BLECallbacks.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"

class BLEController
{
private:
    TimeController* timeController;
    RTCController* rtcController;
    bool isAdvertised;

public:
    BLEController();
    void advertising(TimeController* timeController, RTCController* WrtcController);
};

#endif