#include "NotificationScreen.h""

#include "Symtext7pt7b.h"
#include "Watchy.h"

void NotificationScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(&Symtext7pt7b);
  Watchy::display.printf("\n\nbattery\n%4.2fv\n", Watchy::getBatteryVoltage());
}
