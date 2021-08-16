#pragma once

#include "Screen.h"

class TimeScreen : public Screen {
 public:
  TimeScreen(uint16_t bg = GxEPD_WHITE) : Screen(bg) {}
  uint16_t TextColor;
  void show() override;
  int getNumberOfLines(String data);
  //BT functions
  void getTime();
  void getNotifications();
  //Time Setting functions
  void update_RTC_from_BT();
  void Check_BTtime_to_RTC();
  //Helper functions
  void SetTextColor();
  void drawCentreString(const String &buf, int x, int y);
  void check_rtc_time();
  int MydayOfWeek(uint16_t year, uint8_t month, uint8_t day);
  void Val_To_Day(uint8_t day);

  //Drawing Main Watch Face
  void Draw_Time();
  void Draw_Weather();
  void Draw_Battery();
  void Draw_Steps();
  void Draw_base_UI();
  void Draw_Sensors();
  void Draw_Icons();
  void Draw_Notifcations_Text(int count);
};
