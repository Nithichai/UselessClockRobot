#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"
#include "DS1302.h"

Adafruit_8x16matrix matrix;
int8_t shiftIndex;
String strTime;
String strDate;
String dayAsString(const Time::Day day);
String monthAsString(uint8_t month);

DS1302 rtc(18, 17, 16);

String dayAsString(const Time::Day day) {
  switch (day) {
    case Time::kSunday: return "SUN";
    case Time::kMonday: return "MON";
    case Time::kTuesday: return "TUE";
    case Time::kWednesday: return "WED";
    case Time::kThursday: return "THU";
    case Time::kFriday: return "FRI";
    case Time::kSaturday: return "SAT";
  }
  return "(unknown day)";
}

String monthAsString(uint8_t month) {
  switch (month) {
    case 1: return "JAN";
    case 2: return "FEB";
    case 3: return "MAR";
    case 4: return "APR";
    case 5: return "MAY";
    case 6: return "JUN";
    case 7: return "JUL";
    case 8: return "AUG";
    case 9: return "SEP";
    case 10: return "OCT";
    case 11: return "NOV";
    case 12: return "DEC";
  }
  return "(unknown month)";
}

void ledDisplayFromShowMode(uint8_t showMode) {
  switch (showMode) {
    case 1:
      matrix.print(strDate);
      break;
    default:
      matrix.print(strTime);
  }
}

int8_t ledDisplayStringLength(uint8_t showMode) {
  if (showMode == 1) {
    return strDate.length() * 6;
  }
  return strTime.length() * 6;
}

void ledDisplayShow(uint8_t showMode) {
  matrix.clear();
  matrix.setCursor(shiftIndex, 0);
  ledDisplayFromShowMode(showMode);
  matrix.writeDisplay();
  shiftIndex--;
  if (shiftIndex < -ledDisplayStringLength(showMode)) {
    shiftIndex = 16;
  }
}

void ledDisplaySetTime(uint8_t hour, uint8_t minute) {
  strTime = String(hour / 10)
            + String(hour % 10)
            + ":" + String(minute / 10)
            + String(minute % 10);
}

void ledDisplaySetDate(Time::Day day, uint8_t date, uint8_t month, uint16_t year) {
  strDate = dayAsString(day)
            + " "
            + String(date / 10)
            + String(date % 10)
            + " "
            + monthAsString(month)
            + " "
            + String(year / 1000)
            + String(year % 1000 / 100)
            + String(year % 100 / 10)
            + String(year % 10);
}

void setup() {
  Serial.begin(112500);
  matrix = Adafruit_8x16matrix();
  matrix.begin(0x70);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.setTextColor(LED_ON);
  matrix.setRotation(1);
  shiftIndex = 0;
  strTime = "";
  strDate = "";

  rtc.writeProtect(false);
  rtc.halt(false);
  Time time(2020, 2, 28, 22, 19, 0, Time::kSunday);
  rtc.time(time);
}

void loop() {
  Time time = rtc.time();
  ledDisplaySetTime(time.hr, time.min);
  ledDisplaySetDate(time.day, time.date, time.mon, time.yr);
  ledDisplayShow(1);
  delay(100);
}
