#pragma once
#include "station_query.h"
#include "graph.h"
#include<string>
class MenuSystem {
public:
    explicit MenuSystem(StationQuery& query);
    void run();

private:
    StationQuery& stationQuery;

    // 工具函数
    void clearScreen();
    int getTerminalWidth();
    void pressAnyKeyToContinue();

    // 菜单函数
    void showMainMenu();
    void showStationQueryMenu();
    void showLineQueryMenu();
    void showBusinessModifyMenu();
    void showNavigationMenu();

    // 子功能
    void queryStationById();
    void queryStationByName();
    void showClosedStations();
    void queryLineInfo();
    void listAllLines();
    void modifySingleStation();
    void modifyLineStations();
    void findShortestPath();
    void findMinTransferPath();
    // === 路径查询 ===
    void findPaths(int start_id, int end_id, int option); // 用于路径规划

    // === 线路信息 ===
    std::vector<std::string> getAllLines() const; // 获取所有线路名称

    // === 状态管理 ===
    void setStationStatus(int stationId, const std::string& status); // 设置站点状态
};