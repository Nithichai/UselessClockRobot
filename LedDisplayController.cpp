#include "LedDisplayController.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

Adafruit_8x16matrix matrix;

LedDisplayController::LedDisplayController(void) {
    matrix = Adafruit_8x16matrix();
    matrix.begin(0x70);
    matrix.setTextSize(1);
    matrix.setTextWrap(false);
    matrix.setRotation(1);
    matrix.setTextColor(LED_ON);
}

void LedDisplayController::slideDateTimeMessage()
{
    shiftIndex--;
    if (shiftIndex < -getDisplayDateTimeLength())
    {
        shiftIndex = 16;
    }
}

void LedDisplayController::slideTimeMessage()
{
    shiftIndex--;
    if (shiftIndex < -getDisplayTimeLength())
    {
        shiftIndex = 16;
    }
}

int16_t LedDisplayController::getDisplayDateTimeLength()
{
    return dateTimeMessage.length() * 6;
}

int16_t LedDisplayController::getDisplayTimeLength()
{
    return timeMessage.length() * 6;
}

void LedDisplayController::showDateTime()
{
    matrix.clear();
    matrix.setCursor(shiftIndex, 0);
    matrix.print(dateTimeMessage);
    matrix.writeDisplay();
    slideDateTimeMessage();
}

void LedDisplayController::showTime()
{
    matrix.clear();
    matrix.setCursor(shiftIndex, 0);
    matrix.print(timeMessage);
    matrix.writeDisplay();
    slideTimeMessage();
}

void LedDisplayController::setDateTimeMessage(String message)
{
    dateTimeMessage = message;
}

void LedDisplayController::setTimeMessage(String message)
{
    timeMessage = message;
}