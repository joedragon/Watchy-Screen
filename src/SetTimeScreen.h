#ifndef SETTIMESCREEN_H
#define SETTIMESCREEN_H

#include "Screen.h"

class SetTimeScreen : public Screen
{
public:
    SetTimeScreen() : Screen("SetTimeScreen", GxEPD_BLACK){};
    void show();
    void up();
    void down();
    void back();
    void menu();
};

extern SetTimeScreen setTimeScreen;
#endif