#ifndef BLECallbacks_h
#define BLECallbacks_h

#include "Arduino.h"
#include "TimeController.h"
#include "RTCController.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"

class BLECallbacks : public BLECharacteristicCallbacks
{
    private:
        TimeController* timeController;
        RTCController* rtcController;

    public:
        BLECallbacks(TimeController* timeController, RTCController* rtcController);
        void onWrite(BLECharacteristic *bleCharacteristic);
};

#endif