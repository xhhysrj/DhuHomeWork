#include "menu.h"
#include <iostream>
#include <vector>
#include <unordered_map>

#include <limits>
#include <string>
#include <iomanip>
#include <algorithm>
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/ioctl.h>
#include <unistd.h>
#endif

using namespace std;

// === 构造函数 ===
MenuSystem::MenuSystem(StationQuery& query) : stationQuery(query) {}

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

void MenuSystem::pressAnyKeyToContinue() {
    cout << "\n按任意键继续...";
    cin.ignore(1000, '\n');
    cin.get();
}

// === 主菜单 ===
void MenuSystem::showMainMenu() {
    clearScreen();
    int width = getTerminalWidth();
    string title = "上海地铁路径规划系统";

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

// === 站点查询菜单 ===
void MenuSystem::showStationQueryMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== 站点查询 ===" << endl
            << "1. 按ID查询" << endl
            << "2. 按名称查询" << endl
            << "3. 查看关闭站点" << endl
            << "0. 返回主菜单" << endl
            << "请输入选择：";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效，请重新输入！" << endl;
            pressAnyKeyToContinue();
            continue;
        }

        switch (choice) {
        case 1: queryStationById(); break;
        case 2: queryStationByName(); break;
        case 3: showClosedStations(); break;
        case 0: return;
        default: cout << "无效选项！" << endl;
        }
        pressAnyKeyToContinue();
    } while (true);
}

void MenuSystem::queryStationById() {
    int id;
    cout << "请输入站点ID: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "无效的站点ID！";
        return;
    }

    try {
        stationQuery.printStationInfo(id);
    }
    catch (const out_of_range& e) {
        cout << "错误: " << e.what() << endl;
    }
}

void MenuSystem::queryStationByName() {
    string name;
    cout << "请输入站点名称: ";
    cin.ignore();
    getline(cin, name);

    bool fuzzy;
    cout << "是否模糊匹配? (1=是 0=否): ";
    cin >> fuzzy;

    auto results = stationQuery.findStationsByName(name, fuzzy);
    if (results.empty()) {
        cout << "未找到匹配站点！" << endl;
        return;
    }

    cout << "找到 " << results.size() << " 个匹配站点:" << endl;
    for (int id : results) {
        stationQuery.printStationInfo(id);
        cout << "----------------" << endl;
    }
}

void MenuSystem::showClosedStations() {
    auto closed = stationQuery.getAllClosedStations();
    if (closed.empty()) {
        cout << "当前没有关闭的站点" << endl;
        return;
    }

    cout << "=== 关闭站点列表 ===" << endl;
    for (int id : closed) {
        cout << "ID: " << id
            << " 名称: " << stationQuery.getStationName(id) << endl;
    }
}

// === 线路查询菜单 ===
void MenuSystem::showLineQueryMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== 线路查询 ===" << endl
            << "1. 查询线路详情" << endl
            << "2. 列出所有线路" << endl
            << "0. 返回主菜单" << endl
            << "请输入选择：";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效，请重新输入！" << endl;
            pressAnyKeyToContinue();
            continue;
        }

        switch (choice) {
        case 1: queryLineInfo(); break;
        case 2: listAllLines(); break;
        case 0: return;
        default: cout << "无效选项！" << endl;
        }
        pressAnyKeyToContinue();
    } while (true);
}
void MenuSystem::listAllLines() {
    auto lines = stationQuery.getAllLines(); // 调用 StationQuery 接口获取线路列表
    if (lines.empty()) {
        cout << "当前没有线路信息。" << endl;
        return;
    }

    cout << "=== 所有线路 ===" << endl;
    for (const auto& line : lines) {
        cout << "- " << line << endl;
    }
}
void MenuSystem::queryLineInfo() {
    string lineName;
    cout << "请输入线路名称(如'1号线'): ";
    cin.ignore();
    getline(cin, lineName);

    stationQuery.printLineSummary(lineName);
}


// === 站点状态修改菜单 ===
void MenuSystem::showBusinessModifyMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== 站点状态修改 ===" << endl
            << "1. 设置站点状态" << endl
            << "2. 批量设置线路状态" << endl
            << "0. 返回主菜单" << endl
            << "请输入选择：";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效，请重新输入！" << endl;
            pressAnyKeyToContinue();
            continue;
        }

        switch (choice) {
        case 1: modifySingleStation(); break;
        case 2: modifyLineStations(); break;
        case 0: return;
        default: cout << "无效选项！" << endl;
        }
        pressAnyKeyToContinue();
    } while (true);
}

void MenuSystem::modifySingleStation() {
    int id;
    string status;

    cout << "请输入站点ID: ";
    if (!(cin >> id)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "无效的站点ID！";
        return;
    }

    cout << "设置状态(open/closed): ";
    cin >> status;

    try {
        stationQuery.setStationStatus(id, status);
        cout << "修改成功！当前状态: "
            << (stationQuery.isStationClosed(id) ? "closed" : "open") << endl;
    }
    catch (const exception& e) {
        cout << "错误: " << e.what() << endl;
    }
}

void MenuSystem::modifyLineStations() {
    string lineName, status;

    cout << "请输入线路名称: ";
    cin.ignore();
    getline(cin, lineName);

    cout << "设置状态(open/closed): ";
    cin >> status;

    auto stations = stationQuery.getStationsOnLine(lineName);
    if (stations.empty()) {
        cout << "未找到该线路！" << endl;
        return;
    }

    for (int id : stations) {
        stationQuery.setStationStatus(id, status);
    }
    cout << "已批量修改 " << stations.size() << " 个站点状态为 " << status << endl;
}

// === 导航菜单 ===
void MenuSystem::showNavigationMenu() {
    int choice;
    do {
        clearScreen();
        cout << "=== 导航功能 ===" << endl
            << "1. 查询最短路径" << endl
            << "2. 查询换乘最少路径" << endl
            << "0. 返回主菜单" << endl
            << "请输入选择：";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "输入无效，请重新输入！" << endl;
            pressAnyKeyToContinue();
            continue;
        }

        switch (choice) {
        case 1: findShortestPath(); break;
        case 2: findMinTransferPath(); break;
        case 0: return;
        default: cout << "无效选项！" << endl;
        }
        pressAnyKeyToContinue();
    } while (true);
}

void MenuSystem::findShortestPath() {
    int startId, endId, option;

    cout << "请输入起点站ID: ";
    if (!(cin >> startId)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "无效的站点ID！";
        return;
    }

    cout << "请输入终点站ID: ";
    if (!(cin >> endId)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "无效的站点ID！";
        return;
    }

    cout << "显示几条路径? (1或3): ";
    cin >> option;

    try {
        stationQuery.findPaths(startId, endId, option);
    }
    catch (const exception& e) {
        cout << "导航失败: " << e.what() << endl;
    }
}

void MenuSystem::findMinTransferPath() {
    cout << "换乘最少路径功能开发中..." << endl;
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
            pressAnyKeyToContinue();
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