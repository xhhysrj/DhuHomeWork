#include "menu.h"
#include <iostream>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

void MenuSystem::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int MenuSystem::getTerminalWidth() {
    // 实现同前...
}

void MenuSystem::showMainMenu() {
    clearScreen();
    int width = getTerminalWidth();
    string text1 = "上海地铁路径规划系统";
    cout << string((width - text1.length()) / 2, ' ') << text1 << endl;
    string text2 = "Welcome！";
    cout << string((width - text2.length()) / 2, ' ') << text2 << endl;

    cout << "请选择您接下来的操作：" << endl;
    cout << "\t1. 查询站点信息" << endl;
    cout << "\t2. 查询地铁线路信息" << endl;
    cout << "\t3. 修改站点营业信息" << endl;
    cout << "\t4. 导航功能" << endl;
    cout << "\t0. 退出程序" << endl;
    cout << "请输入您的选择：";
}

void MenuSystem::run() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            showStationQueryMenu();
            break;
        case 2:
            showLineQueryMenu();
            break;
        case 3:
            showBusinessModifyMenu();
            break;
        case 4:
            showNavigationMenu();
            break;
        case 0:
            cout << "感谢使用，再见！" << endl;
            break;
        default:
            cout << "无效的选择，请重新输入！" << endl;
            cin.ignore();
            cin.get();
            break;
        }
    } while (choice != 0);
}

// 其他菜单函数的实现...