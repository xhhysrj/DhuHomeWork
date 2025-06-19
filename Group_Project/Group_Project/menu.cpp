#include "menu.h"
#include"station_query.h"
#include <iostream>
#include <limits> 
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif
using namespace std;
void MenuSystem::clearScreen()  {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}
int MenuSystem::getTerminalWidth()  {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    return size.ws_col;
#endif
}
void MenuSystem::showMainMenu() {
    system("cls"); 
    int width = getTerminalWidth();
    string text1 = "上海地铁路径规划系统";
    cout << string((width - text1.length()) / 2, ' ') << text1 << endl;
    string text2 = "Welcome！";
    cout << string((width - text2.length()) / 2, ' ') << text2 << endl;

    cout << "请选择您接下来的操作：" << endl;
    cout << "\t1.查询站点信息（根据您给出的站点名称或ID，提供该站点包括站点ID，站点名称，经过线路，站内营业信息）" << endl;
    cout << "\t2.查询地铁线路信息（根据您给出的线路名称，提供该线路所经过的站点的信息）" << endl;
    cout << "\t3.修改站点营业的信息（您可以根据实际情况提供某个站内的营业信息或者查询关闭的站点或者恢复所有站点到初始状态）" << endl;
    cout << "\t4.导航功能（根据您提供的起点和终点信息，为您规划出合理路线）" << endl;
    cout << "\t5.退出程序" << endl;
    cout << "ps.以上功能模块均具有模糊搜索的功能！" << endl;
    cout << "请输入您的选择：";
}
void MenuSystem::run() {
    int choice;
    do {
        showMainMenu();
        cin >> choice;
        switch (choice) {
        case 1: showStationQueryMenu(); break;
        case 2: showLineQueryMenu(); break;
        case 3: showBusinessModifyMenu(); break;
        case 4: showNavigationMenu(); break;
        case 5: cout << "程序已经关闭，再见！" << endl; break;
        default: cout << "无效选择！" << endl;
        }
    } while (choice != 5);
}
void MenuSystem::showStationSubMenu() {
    int choice=1;
    do {
        switch (choice) {
        case 1: showStationSubMenu(); break; // 修改这一行
        case 2: {
            string name;
            cout << "请输入站点名称: ";
            cin >> name;
            // 这里需要添加名称查询逻辑
            break;
        }
        case 3:
            cout << '0';
            break;
        case 4:
            cout << '0';
            break;
        case 0:
            return;
        default:
            cout << "无效输入!";
        }

        //pressAnyKeyToContinue();
    } while (true);
}


//int main() {
//    MenuSystem menu;  
//    menu.run();       
//    return 0;
//}