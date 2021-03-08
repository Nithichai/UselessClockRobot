#include "TimeController.h"
#include "LedDisplayController.h"
#include "RTCController.h"
#include "BLEController.h"

TimeController* timeController;
RTCController* rtcController;
LedDisplayController* ledDisplayController;
BLEController* bleController;

void setup()
{
  Serial.begin(115200);
  timeController = new TimeController();
  rtcController = new RTCController();
  ledDisplayController = new LedDisplayController();
  bleController = new BLEController();
}

void loop()
{
  if (timeController->getSaveTimeFlag())
  {
    String msg = timeController->getDisplayDateTime(rtcController->getTime());
    ledDisplayController->setDateTimeMessage(msg);
    ledDisplayController->showDateTime();
    delay(100);
  }
  else
  {
    bleController->advertising(timeController, rtcController);
  }
}
