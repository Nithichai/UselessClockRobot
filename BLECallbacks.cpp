#include "BLECallbacks.h"

BLECallbacks::BLECallbacks(TimeController* timeController, RTCController* rtcController) {
    this->timeController = timeController;
    this->rtcController = rtcController;
}

void BLECallbacks::onWrite(BLECharacteristic *bleCharacteristic) {
    String message = bleCharacteristic->getValue().c_str();
    if (message.length() > 0) {
        Time time = timeController->mapBLEMessageToTime(message);
        rtcController->setTime(time);
        timeController->setSavedFlag();
    }
}