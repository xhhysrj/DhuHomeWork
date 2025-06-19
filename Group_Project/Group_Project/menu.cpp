#include "menu.h"
#include <iostream>
#include <limits>
#include <string>
#include <iomanip>  // 用于setw格式控制

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

// === 基础工具函数 ===
void MenuSystem::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int MenuSystem::getTerminalWidth() {
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

// === 菜单实现 ===
void MenuSystem::showMainMenu() {
    clearScreen();
    int width = getTerminalWidth();
    string title = "上海地铁路径规划系统";

    // 居中显示标题
    cout << string((width - title.length()) / 2, ' ') << title << endl
        << string((width - 9) / 2, ' ') << "Welcome！" << endl << endl;

    cout << "请选择操作：" << endl
        << "  1. 查询站点信息" << endl
        << "  2. 查询线路信息" << endl
        << "  3. 修改站点状态" << endl
        << "  4. 导航功能" << endl
        << "  5. 退出程序" << endl
        << "请输入选择：";
}

void MenuSystem::showStationQueryMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== 站点查询 ===" << endl
            << "1. 按ID查询" << endl
            << "2. 按名称查询" << endl
            << "3. 查看所有站点" << endl
            << "0. 返回主菜单" << endl
            << "请输入选择：";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效，请重新输入！" << endl;
            continue;
        }

        switch (choice) {
        case 1: {
            cout << "功能开发中，按任意键返回..." << endl;
            break;
        }
        case 0:
            return;
        default:
            cout << "功能开发中，按任意键返回..." << endl;
        }
        cin.ignore();
        cin.get();
    } while (true);
}

// === 其他菜单占位实现 ===
void MenuSystem::showLineQueryMenu() {
    clearScreen();
    cout << "=== 线路查询 ===" << endl
        << "（功能开发中）" << endl
        << "按任意键返回...";
    cin.ignore();
    cin.get();
}

void MenuSystem::showBusinessModifyMenu() {
    clearScreen();
    cout << "=== 站点状态修改 ===" << endl
        << "（功能开发中）" << endl
        << "按任意键返回...";
    cin.ignore();
    cin.get();
}

void MenuSystem::showNavigationMenu() {
    clearScreen();
    cout << "=== 导航功能 ===" << endl
        << "（功能开发中）" << endl
        << "按任意键返回...";
    cin.ignore();
    cin.get();
}

// === 主控制循环 ===
void MenuSystem::run() {
    int choice;
    do {
        showMainMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "无效输入！" << endl;
            continue;
        }

        switch (choice) {
        case 1: showStationQueryMenu(); break;
        case 2: showLineQueryMenu(); break;
        case 3: showBusinessModifyMenu(); break;
        case 4: showNavigationMenu(); break;
        case 5: cout << "程序已退出" << endl; break;
        default: cout << "无效选项！" << endl;
        }
    } while (choice != 5);
}