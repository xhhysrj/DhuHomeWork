#pragma once
#include "graph.h"
#include <vector>
#include <string>
#include <unordered_map>
class StationQuery {
public:
    // 初始化（自动加载数据）
    explicit StationQuery(const std::string& stationFile = "Station.csv",
        const std::string& edgeFile = "Edge.csv");

    // === 基础查询 ===
    std::string getStationName(int stationId) const;
    std::vector<std::string> getStationLines(int stationId) const;
    std::string getStationStatus(int stationId) const;

    // === 高级查询 ===
    std::vector<int> findStationsByName(const std::string& name, bool fuzzyMatch = false) const;
    std::vector<int> getStationsOnLine(const std::string& lineName) const;
    std::vector<Edge> getStationConnections(int stationId) const;

    // === 状态查询 ===
    bool isStationClosed(int stationId) const;
    std::vector<int> getAllClosedStations() const;

    // === 打印输出 ===
    void printStationInfo(int stationId) const;
    void printAllStations() const;
    void printLineSummary(const std::string& lineName) const;

private:
    MotorGraph graph_; // 底层图数据结构
    std::unordered_map<int, Station> stations_;
    // 辅助函数
    static bool stringContains(const std::string& str, const std::string& substr, bool ignoreCase);
    bool stringContains(const std::string& str, const std::string& substr, bool ignoreCase) ;
};