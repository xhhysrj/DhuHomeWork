#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <string>

class MenuSystem {
public:
    void run();

private:
    void showMainMenu();    //主菜单
    void showStationQueryMenu();    //站点信息
    void showLineQueryMenu();   //路线信息
    void showBusinessModifyMenu();  //修改站点开启与关闭
    void showNavigationMenu();  //导航功能
    int getTerminalWidth(); //辅助中心输出
    void clearScreen(); //清屏
};

#endif#pragma once
