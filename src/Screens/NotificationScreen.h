#pragma once

#include "Screen.h"

class NotificationScreen : public Screen {
 public:
  NotificationScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  void show() override;
};
