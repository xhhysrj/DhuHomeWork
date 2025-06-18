#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <string>

class MenuSystem {
public:
    void run();

private:
    void showMainMenu();
    void showStationQueryMenu();
    void showLineQueryMenu();
    void showBusinessModifyMenu();
    void showNavigationMenu();
    int getTerminalWidth();
    void clearScreen();
};

#endif#pragma once
