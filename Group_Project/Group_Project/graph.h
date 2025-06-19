// graph.h
#pragma once
#include "station.h"
#include <unordered_map>
#include <list>
#include <vector>
#include <string>

class MotorGraph {
public:
    // === 核心数据结构 ===
    std::unordered_map<int, Station> stations;   // 站点哈希表：ID -> Station
    std::vector<std::list<Edge>> adjList;        // 邻接表（按站点ID索引）
    std::unordered_map<std::string, std::vector<int>> lineStations; // 线路到站点ID的映射

    // === 数据加载接口 ===
    void loadStations(const std::string& filePath); // 加载Station.csv
    void loadEdges(const std::string& filePath);    // 加载Edge.csv

    // === 基础查询接口 ===
    bool hasStation(int stationId) const;
    const Station& getStation(int stationId) const;
    const std::list<Edge>& getConnectedEdges(int stationId) const;

    // === 线路查询接口 ===
    const std::vector<int>& getStationsByLine(const std::string& lineName) const;
    std::vector<std::string> getAllLines() const;

    // === 状态管理 ===
    void setStationStatus(int stationId, const std::string& status);
    std::vector<int> getClosedStations() const;

    // === 工具函数 ===
    
    void printStationConnections(int stationId) const;
    
    // === 最短路径算法 ===
    void findPaths(int start_id, int end_id, int option); // 
private:
    // 邻接表初始化
    void initAdjList();

    // 线路数据构建
    void buildLineIndex();
    int maxStationId = 0;               // 当前已知的最大站点 ID
    void ensureAdjSize(int id);         // 这里为了保证 adjList 足够大
};