#include "ShowBatteryScreen.h"

#include "OptimaLTStd22pt7b.h"
#include "Watchy.h"

void ShowBatteryScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(OptimaLTStd22pt7b);
  Watchy::display.printf("\n\nbattery\n%4.2fv\n", Watchy::getBatteryVoltage());
}
