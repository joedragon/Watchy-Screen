#include "NotificationScreen.h"
#include "Declarations.h"
#include "Symtext7pt7b.h"
#include "Watchy.h"
#include "BTVars.h"



#define NOTIFICATION_LIMET 5
struct notifications_struct {
  String NotificationSoruce;
  String NotificationMessage;
} ;


notifications_struct notifcation_array[NOTIFICATION_LIMET];
String temp = "";
String NSorce;
String NMessage;
int Ncount = 0;
int Total_Messages = 0;


void NotificationScreen::NotifcationToSTArray(String data){
  bool gotMessage = false;
  bool gotSoruce = false; 
  Total_Messages = 0;
  for (int a = 0; a < data.length(); a++) {
    if (data[a] == '\n') {
      gotMessage = false;
      gotSoruce = false; 
      temp = "";
      notifcation_array[Ncount].NotificationSoruce = NSorce;
      notifcation_array[Ncount].NotificationMessage = NMessage;
      Ncount = Ncount + 1;
      //If Ncount has been reached, stop reading notifcations
      if (Ncount > NOTIFICATION_LIMET){
        a = data.length();
      }

    }
    //Getting Which app send the notifcation
    if (data[a] == ',' && gotSoruce == false) {
      gotSoruce = true;
      NSorce = temp;
      NSorce.trim();
      temp = "";
      Total_Messages = Total_Messages + 1; 
    }
    //Getting Message part of notifcation
    if (data[a] == ';' && gotMessage == false) {
      //Checking that the next value is not a ;,
      if (data[a + 1] != '\n' && temp != ""){
      NMessage = temp;
      NMessage.remove(0,1);
      temp = "";
      gotMessage = true;
      }
      else {
      temp = "";
      NMessage = "";
      gotMessage = true;
      }
    }
    if (Ncount > NOTIFICATION_LIMET){
    break;
    }
    temp = temp + data[a];
  }

  for (int x = 0; x < Total_Messages; x++){
  Serial.println("Printing Notifcation " + String(x));
  Serial.print("Priting Source : ");
  Serial.println(notifcation_array[x].NotificationSoruce);
  Serial.print("Priting Message : ");
  Serial.println(notifcation_array[x].NotificationMessage);
  }


}

void NotificationScreen::getNotifications()
{
    boolean success = false;
    char trycount = 0;
    int conected_try_count = 0;
    //boolean Got_message = false;
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
                    //Got_message = true;
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
            //Got_message = true;
            trycount = max_try + 1;
        }
        Serial.println("Trying to connect notifcations ? ");
        delay(retry_delay);
        trycount = trycount + 1;
    }
}

void NotificationScreen::DrawNotifcations(){
    //X and Y returned by get text bounds
    int16_t  x1, y1;
    //Width and hight (Not used can probably deleate)
    uint16_t w, h;
    //The cumutaive quardents
    int16_t  x, y;
    x = 5;
    y = 10;
    Watchy::display.setFont(&Symtext7pt7b);
    Watchy::display.setCursor(x, y);
    for (int x = 0; x < Total_Messages; x++){
      String Message = (notifcation_array[x].NotificationSoruce + ": " +notifcation_array[x].NotificationMessage);
      Watchy::display.getTextBounds(Message,x,y,&x1,&y1,&w,&h);
      Watchy::display.println(Message.trim());
      y = y + h + 10;
      Watchy::display.setCursor(x, y);
    }
    Watchy::display.display(true);
}
void NotificationScreen::show() {
  initBLE();
  Watchy::display.fillScreen(bgColor);
  Serial.begin(115200);
  Serial.println("Getting Notifications");
  getNotifications();
  NotifcationToSTArray(notificationData);
  DrawNotifcations();
}
