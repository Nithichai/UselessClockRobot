#include "BLEController.h"

BLEController::BLEController()
{
    this->isAdvertised = false;
}

void BLEController::advertising(TimeController* timeController, RTCController* rtcController)
{
    if (!isAdvertised)
    {
        BLEDevice::init("UselessClockRobot");
        BLEServer *bleServer = BLEDevice::createServer();
        BLEService *bleService = bleServer->createService("82e46351-4541-4740-92f6-329ce106dd29");
        BLECharacteristic *bleCharacteristic = bleService->createCharacteristic(
            "b88b21ed-805d-4c1c-9343-df787d4caee1",
            BLECharacteristic::PROPERTY_WRITE);
        bleCharacteristic->setCallbacks(new BLECallbacks(timeController, rtcController));
        bleService->start();

        BLEAdvertising *bleAdvertising = BLEDevice::getAdvertising();

        bleAdvertising->addServiceUUID("82e46351-4541-4740-92f6-329ce106dd29");
        bleAdvertising->setScanResponse(true);
        bleAdvertising->setMinPreferred(0x06);
        bleAdvertising->setMinPreferred(0x12);
        BLEDevice::startAdvertising();

        isAdvertised = true;
    }
}