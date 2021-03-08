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
    BLEServer* bleServer;
    BLEService* bleService;
    BLECharacteristic* bleCharacteristic;
    BLEAdvertising* bleAdvertising;
    TimeController* timeController;
    RTCController* rtcController;
    bool isAdvertised;

public:
    BLEController(TimeController* timeController, RTCController* WrtcController);
    void advertising();
    void stop();
};

#endif