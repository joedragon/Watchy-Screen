#pragma once

#include "Screen.h"

class TimeScreen : public Screen {
 public:
  TimeScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  uint16_t TextColor;
  void show() override;

  //BT functions
  void getTime();
  void getNotifications();
  //Helper functions
  void SetTextColor();
  void drawCentreString(const String &buf, int x, int y);
  //Drawing Main Watch Face
  void Draw_Time();
  void Draw_Weather();
  void Draw_Battery();
  void Draw_Steps();
  void Draw_base_UI();
  void Draw_Sensors();
  void Draw_Notifcations();
};
