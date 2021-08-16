#include "TimeScreen.h"
#include <stdlib.h>
#include <time.h>
#include "GetLocation.h"

//My stuff
#include "GUI.h"
#include "MyIcons.h"
#include "BatteryIcon.h"
#include "Symtext7pt7b.h"
#include "Symtext10pt7b.h"
#include "Symtext30pt7b.h"
#include "Declarations.h"
#include "Watchy.h"
using namespace Watchy;


//My Defs
#define BT_DEBUG true
//UI control
#define DARKMODE false

//Updating Clock from Bluetouth
#define SETTING_DAYLIGHT_SAVINGS 0
#define BT_TIME_UPDATE_INTERVAL 1440 //in minutes
RTC_DATA_ATTR int TimeIntervalCounter = BT_TIME_UPDATE_INTERVAL;
//How many Minutes the watch can lose before it updates from the BT clock
#define MIN_LOSS 2
#define FUDGE 2


//---- Notification managment -----
#define CONNECTED_TRY_LIMET 2
#define TIME_MAX_TRY 10
const char max_try = 100;
const char retry_delay = 10;
boolean notificationsUpdated = false;
boolean timeUpdated = false;
RTC_DATA_ATTR int LastNotifcationCount;
//-------- TIME MANAGMENT -----
String timeStr = "";
struct tm tim;



//#######################################
//Helper Functions
//#######################################
void TimeScreen::SetTextColor(){
  if (bgColor == GxEPD_WHITE){
    TextColor = GxEPD_BLACK;
  }
  else {
    TextColor = GxEPD_WHITE;
  }
}


void TimeScreen::drawCentreString(const String &buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(buf, x, y, &x1, &y1, &w, &h); //calc width of new string
    display.setCursor(x - w / 2, y);
    display.print(buf);
}

int TimeScreen::getNumberOfLines(String data) {
  int lineCount = 0;
  for (int a = 0; a < data.length(); a++) {
    if (data[a] == '\n') {
      lineCount++;
    }
  }
  return lineCount;
}


void TimeScreen::update_RTC_from_BT(){
    tmElements_t BTLTime;
    BTLTime.Month = (String(timeStr[12]) + String(timeStr[13])).toInt(); //month is zero indexed...
    BTLTime.Day = (String(timeStr[9]) + String(timeStr[10])).toInt();
    BTLTime.Year = (String(timeStr[15]) + String(timeStr[16]) + String(timeStr[17]) + String(timeStr[18])).toInt() - 1900;
    ;
    BTLTime.Hour = (String(timeStr[0]) + String(timeStr[1])).toInt() + (SETTING_DAYLIGHT_SAVINGS);
    BTLTime.Minute = (String(timeStr[3]) + String(timeStr[4])).toInt();
    BTLTime.Second = (String(timeStr[6]) + String(timeStr[7])).toInt();


    makeTime(BTLTime);
    time_t t = makeTime(BTLTime) + FUDGE;
    RTC.set(t);
}
void TimeScreen::Check_BTtime_to_RTC(){
    Serial.println("Checking BT time Dif");
    tmElements_t BTLTime;

    BTLTime.Month = (String(timeStr[12]) + String(timeStr[13])).toInt(); //month is zero indexed...
    BTLTime.Day = (String(timeStr[9]) + String(timeStr[10])).toInt();
    BTLTime.Year = (String(timeStr[15]) + String(timeStr[16]) + String(timeStr[17]) + String(timeStr[18])).toInt() - 1900;
    ;
    BTLTime.Hour = (String(timeStr[0]) + String(timeStr[1])).toInt() + (SETTING_DAYLIGHT_SAVINGS);
    BTLTime.Minute = (String(timeStr[3]) + String(timeStr[4])).toInt();
    BTLTime.Second = (String(timeStr[6]) + String(timeStr[7])).toInt();

    tmElements_t currentTime;
    RTC.read(currentTime);
    if (currentTime.Hour != BTLTime.Hour){
        Serial.println("Hours Do not match Updating");
        update_RTC_from_BT();
    } else if ((abs(currentTime.Minute - BTLTime.Minute) >= MIN_LOSS))
    {
        Serial.println("Minuites are out by" + String(MIN_LOSS) + " Updating");
        update_RTC_from_BT();
    }
    
}

//#######################################
//BLE Functions
//#######################################

void TimeScreen::getTime()
{
    boolean success = false;
    int trycount = 0;
    int conected_try_count = 0;
    boolean Got_message = false;
    while (trycount <= TIME_MAX_TRY && (success == false))
    {
        if (connected && (trycount <= max_try))
        {
            success = sendBLE("/time", &timeStr, false);
            while (success == false && conected_try_count <= CONNECTED_TRY_LIMET)
            {
                if (success == true)
                {
                    notificationsUpdated = true;
                    Serial.println("time is updating");
                    Got_message = true;
                    trycount = max_try + 1;
                    conected_try_count = CONNECTED_TRY_LIMET + 1;
                    break;
                }

                else
                {
                    delay(retry_delay);
                    Serial.println("Watting for notifcations time");
                    success = sendBLE("/time", &timeStr, false);
                    conected_try_count = conected_try_count + 1;
                    Serial.println(conected_try_count);
                    if (success == false)
                    {
                        Serial.println("NO/SUC");
                    }
                    else
                    {
                        Serial.println("SUC");
                        break;
                    }
                }
            }
        }
        Serial.println("Trying to connect time ? ");
        delay(retry_delay);
        trycount = trycount + 1;
        Serial.println(trycount);
    }
}
void TimeScreen::getNotifications()
{
    boolean success = false;
    char trycount = 0;
    int conected_try_count = 0;
    boolean Got_message = false;
    while (trycount <= max_try && (success == false))
    {
        if (connected)
        {
            Serial.println("-------- CONNECTED NOTIFCATIONS ----------");
            success = sendBLE("/notifications", &notificationData, false);
            while (success == false && conected_try_count <= CONNECTED_TRY_LIMET)
            {
                if (success)
                {
                    notificationsUpdated = true;
                    printDebug("notifications updating");
                    Got_message = true;
                    trycount = max_try + 1;
                    conected_try_count = CONNECTED_TRY_LIMET + 1;
                    break;
                }
                else
                {
                    delay(retry_delay);
                    Serial.println("Watting for notifcations");
                    Serial.println(conected_try_count);
                    conected_try_count = conected_try_count + 1;
                    Serial.println(conected_try_count);
                    success = sendBLE("/notifications", &notificationData, false);
                    


                }
            }
        }
        if (connected && !timeUpdated)
        {
            //gets current android notifications as a string
            String timeStr = "";
            boolean success = sendBLE("/time", &timeStr, true);
            Got_message = true;
            trycount = max_try + 1;
        }
        Serial.println("Trying to connect notifcations ? ");
        delay(retry_delay);
        trycount = trycount + 1;
    }
}


//#######################################
//Main Code entery
//#######################################
void TimeScreen::show() {
    initBLE();
    SetTextColor();
    Watchy::display.fillScreen(bgColor);
    Draw_base_UI();
    Draw_Time();
    Draw_Weather();
    Draw_Battery();
    Draw_Steps();
    Draw_Sensors();
    Draw_Icons();
    display.display(true);
    #ifdef BT_DEBUG
        Serial.begin(115200);
    #endif
    getNotifications();
    getTime();

    if (timeStr != ""){
        update_RTC_from_BT();
    }
    if (notificationData != ""){
        int Linecount = getNumberOfLines(notificationData);
        Draw_Notifcations_Text(Linecount);
        LastNotifcationCount = Linecount;
        #ifdef BT_DEBUG
            Serial.println("Priting notification data");
            Serial.println(notificationData);
            Serial.print("Line Count : ");
            Serial.println(Linecount);
            Serial.println("Printing time data");
            Serial.println(timeStr);
            Serial.println("-------------------------------");
        #endif
    }
    else if (LastNotifcationCount > 0)
    {
        Draw_Notifcations_Text(LastNotifcationCount);
    }
    else{
        Draw_Notifcations_Text(0);
    }




  // ignore warning about txt not initialized, assert guarantees it will be
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
  #pragma GCC diagnostic pop


}





//#######################################
//Drawing The UI code
//#######################################
void TimeScreen::Draw_base_UI()
{
    display.drawBitmap(0, 70, myimageFace_Lines_UI, 200, 88, TextColor);
}
void TimeScreen::Draw_Time()
{
    tmElements_t currentTime;
    RTC.read(currentTime);
    display.setFont(&Symtext30pt7b);
    display.setTextColor(TextColor);
    String timeString = "";
    if (currentTime.Hour < 10)
    {
       timeString = timeString + "0";
    }
    timeString = timeString + currentTime.Hour;
    timeString = timeString + ":";
    if (currentTime.Minute < 10)
    {
    timeString = timeString + "0";
    }
    timeString = timeString + currentTime.Minute;
    drawCentreString(timeString,105,53);
    
    
    
    //Drawing date
    String dateString = "";
    String tmp = "";
    display.setFont(&Symtext10pt7b);
    display.setCursor(30, 185);
    display.setTextColor(TextColor);
    tmp = dayStr(currentTime.Wday);
    tmp = tmp.substring(0,3);
    dateString = dateString + tmp;
    dateString = dateString + ", ";  
    if(currentTime.Day < 10){
    dateString = dateString + "0";      
    }   
    dateString = dateString + currentTime.Day;
    dateString = dateString + ", ";  
    dateString = dateString + monthShortStr(currentTime.Month);
    //display.print("Mon, 12, Jun");
    drawCentreString(dateString,125,185);
}
void TimeScreen::Draw_Weather()
{
    display.drawBitmap(67, 76, myimageWeather_icons, 66, 36, TextColor);
}
void TimeScreen::Draw_Battery()
{
    float VBAT = getBatteryVoltage();
    if (VBAT > 4.1)
    {
        display.drawBitmap(2, 117, myimagenewBattery4, 22, 19, TextColor);
    }
    else if (VBAT > 3.95 && VBAT <= 4.1)
    {
        display.drawBitmap(2, 117, myimagenewBattery3, 22, 19, TextColor);
    }
    else if (VBAT > 3.80 && VBAT <= 3.95)
    {
        display.drawBitmap(2, 117, myimagenewBattery2, 22, 19, TextColor);
    }
    else if (VBAT <= 3.80)
    {
        display.drawBitmap(2, 117, myimagenewBattery1, 22, 19, TextColor);
    }

    int batleft = (((VBAT*111.905) - 370) / 100) * 100;
    display.setFont(&Symtext7pt7b);
    display.setCursor(3, 152);
    if (batleft < 10)
    {
        display.print("0");
    }
    display.print(batleft);
    display.print("%");
}
void TimeScreen::Draw_Steps()
{
    display.drawBitmap(83, 109, myimageroad_icon, 35, 35, TextColor);
    uint32_t stepCount = sensor.getCounter();
    String StepCountStr = "";
    display.setFont(&Symtext7pt7b);
    display.setCursor(73, 150);
    if (stepCount < 10000)
    {
        StepCountStr = StepCountStr + "0";
    }
    if (stepCount < 1000)
    {
        StepCountStr = StepCountStr + "0";
    }
    if (stepCount < 100)
    {
        StepCountStr = StepCountStr + "0";
    }
    if (stepCount < 10)
    {
        StepCountStr = StepCountStr + "0";
    }
    drawCentreString(StepCountStr + String(stepCount),100,150);
    //display.print(stepCount);
}
void TimeScreen::Draw_Icons(){
    display.drawBitmap(169, 104, myimageNotifcation_Icon, 35, 35, TextColor);
}
void TimeScreen::Draw_Notifcations_Text(int count)
{
    String countToString;
    if (count < 10) {
        countToString = "0";
        countToString =  countToString + String(count);
    }
    else {
       countToString = String(count); 
    }
    display.setFont(&Symtext7pt7b);
    display.setCursor(155, 152);
    display.print(countToString);
}
void TimeScreen::Draw_Sensors()
{
    display.setFont(&Symtext7pt7b);
    display.drawBitmap(3, 79, myimageTemperatire, 40, 19, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(3, 109);
    display.print("10C");
    //Draw Humidity
    display.drawBitmap(156, 77, myimageHumitiy_icon, 40, 19, DARKMODE ? GxEPD_WHITE : GxEPD_BLACK);
    display.setCursor(155, 109);
    display.print("10H");
}
