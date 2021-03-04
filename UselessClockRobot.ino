#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"
#include "DS1302.h"
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "BLEServer.h"
#include "EEPROM.h"

class DateTimeDisplayController {
  private:
    String stringTime;
    String stringDate;

  public:
    DateTimeDisplayController() {
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

class LedDisplayController {
  private:
    Adafruit_8x16matrix matrix;
    DateTimeDisplayController dateTimeDisplayController;
    int8_t shiftIndex;

    void printFromShowMode(uint8_t showMode) {
      switch (showMode) {
        case 1:
          matrix.print(dateTimeDisplayController.getStringDate());
          break;
        default:
          matrix.print(dateTimeDisplayController.getStringTime());
      }
    }

    int8_t getStringLength(uint8_t showMode) {
      if (showMode == 1) {
        return dateTimeDisplayController.getStringDate().length() * 6;
      }
      return dateTimeDisplayController.getStringTime().length() * 6;
    }

  public:
    LedDisplayController() {
      this->matrix = Adafruit_8x16matrix();
      this->dateTimeDisplayController = DateTimeDisplayController();
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
      dateTimeDisplayController.setStringTime(
        String(hour / 10)
        + String(hour % 10)
        + ":" + String(minute / 10)
        + String(minute % 10)
      );
    }

    void setDate(Time::Day day, uint8_t date, uint8_t month, uint16_t year) {
      dateTimeDisplayController.setStringDate(
        dateTimeDisplayController.dayAsString(day)
        + " "
        + String(date / 10)
        + String(date % 10)
        + " "
        + dateTimeDisplayController.monthAsString(month)
        + " "
        + String(year / 1000)
        + String(year % 1000 / 100)
        + String(year % 100 / 10)
        + String(year % 10)
      );
    }
};
LedDisplayController ledDisplayController = LedDisplayController();

class EEPROMDataController {
  public:
    void setDateTimeSavedFlag() {
      EEPROM.begin(1);
      EEPROM.write(0, 1);
      EEPROM.commit();
      EEPROM.end();
    }

    void setDateTimeRemovedFlag() {
      EEPROM.begin(1);
      EEPROM.write(0, 0);
      EEPROM.commit();
      EEPROM.end();
    }

    boolean getDateTimeFlag() {
      return EEPROM.read(0);
    }
};
EEPROMDataController eepromDataController = EEPROMDataController();

class BLEDataController {
  private:
    int hour;
    int minute;
    int second;
    int day;
    int date;
    int month;
    int year;

  public:
    void saveDateTime(String message) {
      hour = message.substring(0, 2).toInt();
      minute = message.substring(3, 5).toInt();
      second = message.substring(6, 8).toInt();
      day = message.substring(9, 11).toInt();
      date = message.substring(12, 14).toInt();
      month = message.substring(12, 14).toInt();
      year = message.substring(15).toInt();

      eepromDataController.setDateTimeSavedFlag();
    }

    int getHour() {
      return hour;
    }

    int getMinute() {
      return minute;
    }

    int getSecond() {
      return second;
    }

    int getDay() {
      return day;
    }

    int getDate() {
      return date;
    }

    int getMonth() {
      return month;
    }

    int getYear() {
      return year;
    }
};
BLEDataController bleDataController = BLEDataController();

class BLECallbacks: public BLECharacteristicCallbacks {
  public:
    void onWrite(BLECharacteristic *bleCharacteristic) {
      String message = bleCharacteristic->getValue().c_str();
      if (message.length() > 0) {
        bleDataController.saveDateTime(message);
      }
    }
};

class BLEController {
  private:
    BLECallbacks bleCallbacks;
    BLEServer *bleServer;
    BLEService *bleService;
    BLECharacteristic *bleCharacteristic;
    boolean isConnect;

  public:
    void advertising() {
      BLEDevice::init("UselessClockRobot");
      BLEServer *bleServer = BLEDevice::createServer();
      BLEService *bleService = bleServer->createService("82e46351-4541-4740-92f6-329ce106dd29");
      BLECharacteristic *bleCharacteristic = bleService->createCharacteristic(
                                   "b88b21ed-805d-4c1c-9343-df787d4caee1",
                                   BLECharacteristic::PROPERTY_WRITE
                                 );
      bleCharacteristic->setCallbacks(new BLECallbacks());
      BLEAdvertising *bleAdvertising = bleServer->getAdvertising();
      bleService->start();
    }
};
BLEController bleController = BLEController();


DS1302 rtc(18, 17, 16);
class RTCController {
  public:
    RTCController() {
      rtc.writeProtect(false);
      rtc.halt(false);
    }

    void setDateTime(
      uint8_t hour,
      uint8_t minute,
      uint8_t second,
      Time::Day day,
      uint8_t date,
      uint8_t month,
      uint8_t year
    ) {
      Time time(year, month, date, hour, minute, second, day);
      rtc.time(time);
    }

    uint8_t getHour() {
      return rtc.time().hr;
    }

    uint8_t getMinute() {
      return rtc.time().min;
    }

    uint8_t getSecond() {
      return rtc.time().sec;
    }

    Time::Day getDay() {
      return rtc.time().day;
    }

    uint8_t getDate() {
      return rtc.time().date;
    }

    uint8_t getMonth() {
      return rtc.time().mon;
    }

    uint16_t getYear() {
      return rtc.time().yr;
    }
};
RTCController rtcController = RTCController();

void setup() {
  // do nothing
}

void loop() {
  if (eepromDataController.getDateTimeFlag()) {
    ledDisplayController.setTime(
      rtcController.getHour(),
      rtcController.getMinute()
    );
    ledDisplayController.setDate(
      rtcController.getDay(),
      rtcController.getDate(),
      rtcController.getMonth(),
      rtcController.getYear()
    );
    ledDisplayController.show(0);
    delay(100);
  } else {
    bleController.advertising();
  }
}
