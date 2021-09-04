#pragma once
/*****************************************************************************
  The MIT License (MIT)
  Copyright (c) 2020 Matthew James Bellafaire
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
******************************************************************************/
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Arduino.h>
/*******************************************************************
                              DEBUG
 *******************************************************************/
//prints debug information to the serial terminal when declared
//#define DEBUG

#ifdef DEBUG
#define printDebug(a) Serial.println(a)
#endif
#ifndef DEBUG
#define printDebug(a)
#endif

/********************************************************************
                          Globals
 ********************************************************************/
extern String notificationData;
extern String currentSong;
extern unsigned long lastSongUpdate;
//extern static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;


/********************************************************************
                            DataSeperators
 ********************************************************************/
#define FIELD_SEPARATOR ';'
#define FIELD_SEPARATOR_STRING ";"

#define optionDivider '`' //change this to anything you don't think will be used
#define SELECTION_WINDOW_BUTTON_WIDTH 20


/********************************************************************
                              BLE
 ********************************************************************/
//variables and defines used by BLEServer.ino
static String currentDataField;
extern boolean blockingCommandInProgress;
static String* bleReturnString;
#define SERVICE_UUID        "5ac9bc5e-f8ba-48d4-8908-98b80b566e49"
#define COMMAND_UUID        "bcca872f-1a3e-4491-b8ec-bfc93c5dd91a"
extern BLECharacteristic *commandCharacteristic;
extern BLEService *pService;
extern BLEServer *pServer;
extern TaskHandle_t xBLE;

//indicates connection state to the android device
extern boolean connected;

//indiciates whether or not a operation is currently in progress
extern boolean operationInProgress;

//function signitures
//String sendBLE(String command);
boolean sendBLE(String command, String* returnString, boolean blocking);
void addData(String data);  //adds data to a current string, used within BLEServer.ino
void initBLE(); //initializes the BLE connection by starting advertising.

extern boolean calculatorActive;


//My declerations
