#include "RTCController.h"

RTCController::RTCController(void) {
  rtc = new DS1302(18, 17, 16);
  rtc->writeProtect(false);
  rtc->halt(false);
}

void RTCController::setTime(Time time) {
  rtc->time(time);
}

Time RTCController::getTime() {
  return rtc->time();
}