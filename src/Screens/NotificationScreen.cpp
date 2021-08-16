#include "NotificationScreen.h"
#include "Declarations.h"
#include "Symtext7pt7b.h"
#include "Watchy.h"

//void TimeScreen::getNotifications()
//{
//    boolean success = false;
//    char trycount = 0;
//    int conected_try_count = 0;
//    boolean Got_message = false;
//    while (trycount <= max_try && (success == false))
//    {
//        if (connected)
//        {
//            Serial.println("-------- CONNECTED NOTIFCATIONS ----------");
//            success = sendBLE("/notifications", &notificationData, false);
//            while (success == false && conected_try_count <= CONNECTED_TRY_LIMET)
//            {
//                if (success)
//                {
//                    notificationsUpdated = true;
//                    printDebug("notifications updating");
//                    Got_message = true;
//                    trycount = max_try + 1;
//                    conected_try_count = CONNECTED_TRY_LIMET + 1;
//                    break;
//                }
//                else
//                {
//                    delay(retry_delay);
//                    Serial.println("Watting for notifcations");
//                    Serial.println(conected_try_count);
//                    conected_try_count = conected_try_count + 1;
//                    Serial.println(conected_try_count);
//                    success = sendBLE("/notifications", &notificationData, false);
//                    
//
//
//                }
//            }
//        }
//        if (connected && !timeUpdated)
//        {
//            //gets current android notifications as a string
//            String timeStr = "";
//            boolean success = sendBLE("/time", &timeStr, true);
//            Got_message = true;
//            trycount = max_try + 1;
//        }
//        Serial.println("Trying to connect notifcations ? ");
//        delay(retry_delay);
//        trycount = trycount + 1;
//    }
//}

void NotificationScreen::show() {
  Watchy::display.fillScreen(bgColor);
  Watchy::display.setFont(&Symtext7pt7b);
  Watchy::display.printf("\n\nbattery\n%4.2fv\n", Watchy::getBatteryVoltage());
}
