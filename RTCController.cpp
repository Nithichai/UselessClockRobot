#include "RTCController.h"

DS1302 rtc(18, 17, 16);

RTCController::RTCController(void) {
  rtc.writeProtect(false);
  rtc.halt(false);
}

void RTCController::setTime(Time time) {
  rtc.time(time);
}

Time RTCController::getTime() {
  return rtc.time();
}