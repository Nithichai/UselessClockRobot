#include "TimeController.h"
#include "Time.h"

TimeController::TimeController(void)
{
    this->saveTimeFlag = false;
}

String TimeController::mapDayToString(Time::Day day)
{
    switch (day)
    {
    case Time::kSunday:
        return "SUN";
    case Time::kMonday:
        return "MON";
    case Time::kTuesday:
        return "TUE";
    case Time::kWednesday:
        return "WED";
    case Time::kThursday:
        return "THU";
    case Time::kFriday:
        return "FRI";
    case Time::kSaturday:
        return "SAT";
    }
    return "(unknown day)";
}

String TimeController::mapMonthToString(uint8_t month)
{
    switch (month)
    {
    case 1:
        return "JAN";
    case 2:
        return "FEB";
    case 3:
        return "MAR";
    case 4:
        return "APR";
    case 5:
        return "MAY";
    case 6:
        return "JUN";
    case 7:
        return "JUL";
    case 8:
        return "AUG";
    case 9:
        return "SEP";
    case 10:
        return "OCT";
    case 11:
        return "NOV";
    case 12:
        return "DEC";
    };
    return "(unknown month)";
}

Time::Day TimeController::mapNumberToDay(uint8_t number)
{
    switch (number)
    {
    case 1:
        return Time::kSunday;
    case 2:
        return Time::kMonday;
    case 3:
        return Time::kTuesday;
    case 4:
        return Time::kWednesday;
    case 5:
        return Time::kThursday;
    case 6:
        return Time::kFriday;
    case 7:
        return Time::kSaturday;
    }
    return Time::kSunday;
}

String TimeController::getDisplayTime(Time time)
{
    return String(time.hr / 10) + String(time.hr % 10) + ":" + String(time.min / 10) + String(time.min % 10);
}

String TimeController::getDisplayDateTime(Time time)
{
    return mapDayToString(time.day) + " " + String(time.date / 10) + String(time.date % 10) + " " + mapMonthToString(time.mon) + " " + String(time.yr / 1000) + String(time.yr % 1000 / 100) + String(time.yr % 100 / 10) + String(time.yr % 10);
}

Time TimeController::mapBLEMessageToTime(String bleMessage)
{
    // 16-16-16-01-25-08-2020
    uint8_t hour = bleMessage.substring(0, 2).toInt();
    uint8_t minute = bleMessage.substring(3, 5).toInt();
    uint8_t second = bleMessage.substring(6, 8).toInt();
    uint8_t day = bleMessage.substring(9, 11).toInt();
    uint8_t date = bleMessage.substring(12, 14).toInt();
    uint8_t month = bleMessage.substring(15, 17).toInt();
    uint16_t year = bleMessage.substring(18, 22).toInt();
    
    return Time(year, month, date, hour, minute, second, mapNumberToDay(day));
}

bool TimeController::getSaveTimeFlag()
{
    return saveTimeFlag;
}

void TimeController::setSavedFlag()
{
    saveTimeFlag = true;
}

void TimeController::setUnsavedFlag()
{
    saveTimeFlag = false;
}