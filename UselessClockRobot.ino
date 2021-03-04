#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"
#include "DS1302.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"
#include "EEPROM.h"

class DateTimeController {
  private:
    String stringTime;
    String stringDate;

  public:
    DateTimeController() {
      this->stringTime = "";
      this->stringDate = "";
    }

    String getStringTime() {
      return stringTime;
    }

    void setStringTime(String stringTime) {
      this->stringTime = stringTime;
    }

    String getStringDate() {
      return stringDate;
    }

    void setStringDate(String stringDate) {
      this->stringDate = stringDate;
    }

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
};

class LedDisplay {
  private:
    Adafruit_8x16matrix matrix;
    DateTimeController dateTimeController;
    int8_t shiftIndex;

    void printFromShowMode(uint8_t showMode) {
      switch (showMode) {
        case 1:
          matrix.print(dateTimeController.getStringDate());
          break;
        default:
          matrix.print(dateTimeController.getStringTime());
      }
    }

    int8_t getStringLength(uint8_t showMode) {
      if (showMode == 1) {
        return dateTimeController.getStringDate().length() * 6;
      }
      return dateTimeController.getStringTime().length() * 6;
    }

  public:
    LedDisplay() {
      this->matrix = Adafruit_8x16matrix();
      this->dateTimeController = DateTimeController();
      this->matrix.begin(0x70);
      this->matrix.setTextSize(1);
      this->matrix.setTextWrap(false);
      this->matrix.setTextColor(LED_ON);
      this->matrix.setRotation(1);
      this->shiftIndex = 0;
    }

    void show(uint8_t showMode) {
      matrix.clear();
      matrix.setCursor(shiftIndex, 0);
      printFromShowMode(showMode);
      matrix.writeDisplay();
      shiftIndex--;
      if (shiftIndex < -getStringLength(showMode)) {
        shiftIndex = 16;
      }
    }

    void setTime(uint8_t hour, uint8_t minute) {
      dateTimeController.setStringTime(
        String(hour / 10)
        + String(hour % 10)
        + ":" + String(minute / 10)
        + String(minute % 10)
      );
    }

    void setDate(Time::Day day, uint8_t date, uint8_t month, uint16_t year) {
      dateTimeController.setStringDate(
        dateTimeController.dayAsString(day)
        + " "
        + String(date / 10)
        + String(date % 10)
        + " "
        + dateTimeController.monthAsString(month)
        + " "
        + String(year / 1000)
        + String(year % 1000 / 100)
        + String(year % 100 / 10)
        + String(year % 10)
      );
    }
};

//class BLECallbacks: public BLECharacteristicCallbacks {
//  private:
//    void saveEEPROM(
//      String hour,
//      String minute,
//      String second,
//      String day,
//      String date,
//      String month,
//      String year,
//    ) {
//      EEPROM.begin(16);
//    }
//    void subStringFromMobile(String value) {
//      // hr-min-second-day-date-month-year
//      // 00-00-00-00-00-00-0000
//
//    }
//
//  public:
//    void onWrite(BLECharacteristic *pCHaracteristic) {
//      String value = pCharacteristic->getValue();
//      if (value.length() > 0) {
//        subStringFromMobile(value);
//      }
//    }
//}
//
//class BLEController {
//  private:
//    BLECallbacks bleCallbacks;
//
//  public:
//
//}

DS1302 rtc(18, 17, 16);
LedDisplay ledDisplay = LedDisplay();

void setup() {
  Serial.begin(112500);

  rtc.writeProtect(false);
  rtc.halt(false);
  Time time(2020, 2, 28, 22, 19, 0, Time::kSunday);
  rtc.time(time);
}

void loop() {
  Time time = rtc.time();
  ledDisplay.setTime(time.hr, time.min);
  ledDisplay.setDate(time.day, time.date, time.mon, time.yr);
  ledDisplay.show(0);
  delay(100);
}
