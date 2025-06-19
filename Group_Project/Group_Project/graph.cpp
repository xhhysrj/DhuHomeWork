#include "graph.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <iostream>
#include <queue>

// === 路径规划相关结构 ===
struct Path {
    std::vector<int> path;
    int total_time;

    Path(int start_id) : total_time(0) {
        path.push_back(start_id);
    }

    Path(const Path& prev, int next_id, int edge_time) {
        path = prev.path;
        path.push_back(next_id);
        total_time = prev.total_time + edge_time;
    }

    bool contains(int node) const {
        return std::find(path.begin(), path.end(), node) != path.end();
    }
};

struct PathCompare {
    bool operator()(const Path& a, const Path& b) const {
        return a.total_time > b.total_time; // 最小堆
    }
};

// === 辅助函数 ===
namespace {
    std::vector<std::string> splitString(const std::string& s, char delimiter) {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(s);
        while (std::getline(tokenStream, token, delimiter)) {
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }
}

// === 数据加载 ===
void MotorGraph::loadStations(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开站点文件: " + filePath);
    }

    std::string line;
    std::getline(file, line); // 跳过标题行

    while (std::getline(file, line)) {
        Station station;
        std::istringstream ss(line);
        std::string field;

        // 解析 station_id
        std::getline(ss, field, ',');
        station.id = std::stoi(field);

        // 解析 station_name
        std::getline(ss, station.name, ',');

        // 解析 line（处理多线路情况）
        std::getline(ss, field, ',');
        station.line = field;
        station.lines = splitString(field, '|');

        // 解析 status
        std::getline(ss, station.status, ',');

        stations[station.id] = station;
    }
    initAdjList();
    buildLineIndex();
}

void MotorGraph::loadEdges(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开边文件: " + filePath);
    }

    std::string line;
    std::getline(file, line); // 跳过标题行

    while (std::getline(file, line)) {
        Edge edge;
        std::istringstream ss(line);
        std::string field;

        // 解析 start_station_id
        std::getline(ss, field, ',');
        edge.startId = std::stoi(field);

        // 解析 end_station_id
        std::getline(ss, field, ',');
        edge.endId = std::stoi(field);

        // 解析 line
        std::getline(ss, edge.line, ',');

        // 解析 direction
        std::getline(ss, edge.direction, ',');

        // 解析 time
        std::getline(ss, field, ',');
        edge.time = std::stoi(field);

        // 验证站点存在性
        if (!hasStation(edge.startId) || !hasStation(edge.endId)) {
            continue;
        }

        // 添加到邻接表（双向边）
        adjList[edge.startId].push_back(edge);
        adjList[edge.endId].emplace_back(edge.endId, edge.startId, edge.line,
            edge.direction, edge.time);
    }
}

// === 图结构初始化 ===
void MotorGraph::initAdjList() {
    adjList.resize(stations.size() + 1);
}

void MotorGraph::buildLineIndex() {
    for (const auto& pair : stations) {
        for (const auto& line : pair.second.lines) {
            lineStations[line].push_back(pair.first);
        }
    }
}

// === 查询接口 ===
bool MotorGraph::hasStation(int stationId) const {
    return stations.find(stationId) != stations.end();
}

const Station& MotorGraph::getStation(int stationId) const {
    auto it = stations.find(stationId);
    if (it == stations.end()) {
        throw std::out_of_range("站点ID不存在: " + std::to_string(stationId));
    }
    return it->second;
}

const std::list<Edge>& MotorGraph::getConnectedEdges(int stationId) const {
    if (stationId < 0 || stationId >= adjList.size()) {
        throw std::out_of_range("无效站点ID");
    }
    return adjList[stationId];
}

// === 线路查询 ===
const std::vector<int>& MotorGraph::getStationsByLine(const std::string& lineName) const {
    static const std::vector<int> emptyVector;
    auto it = lineStations.find(lineName);
    return it != lineStations.end() ? it->second : emptyVector;
}

std::vector<std::string> MotorGraph::getAllLines() const {
    std::vector<std::string> lines;
    for (const auto& pair : lineStations) {
        lines.push_back(pair.first);
    }
    return lines;
}

// === 状态管理 ===
void MotorGraph::setStationStatus(int stationId, const std::string& status) {
    if (hasStation(stationId)) {
        stations[stationId].status = status;
    }
}

std::vector<int> MotorGraph::getClosedStations() const {
    std::vector<int> closedStations;
    for (const auto& pair : stations) {
        if (pair.second.status == "closed") {
            closedStations.push_back(pair.first);
        }
    }
    return closedStations;
}

// === 输出功能 ===
void MotorGraph::printAllStations() const {
    std::cout << "=== 所有站点 ===" << std::endl;
    for (const auto& pair : stations) {
        const Station& s = pair.second;
        std::cout << "ID: " << pair.first
            << " | 名称: " << s.name
            << " | 线路: ";
        for (size_t i = 0; i < s.lines.size(); ++i) {
            if (i != 0) std::cout << ",";
            std::cout << s.lines[i];
        }
        std::cout << " | 状态: " << s.status << std::endl;
    }
}

void MotorGraph::printStationConnections(int stationId) const {
    if (!hasStation(stationId)) return;

    const Station& station = getStation(stationId);
    std::cout << "=== 站点连接 ===" << std::endl;
    std::cout << "ID: " << stationId << " | 名称: " << station.name << std::endl;

    const auto& edges = getConnectedEdges(stationId);
    for (const auto& edge : edges) {
        int connectedId = (edge.startId == stationId) ? edge.endId : edge.startId;
        std::cout << "  → ID: " << connectedId
            << " | 名称: " << getStation(connectedId).name
            << " | 线路: " << edge.line
            << " | 方向: " << edge.direction
            << " | 时间: " << edge.time << "分钟" << std::endl;
    }
}

// === 最短路径算法 ===
void MotorGraph::findPaths(int start_id, int end_id, int option)  {
    int k = (option == 1) ? 1 : 3;
    std::priority_queue<Path, std::vector<Path>, PathCompare> pq;
    std::vector<Path> results;

    pq.push(Path(start_id));

    while (!pq.empty() && results.size() < k) {
        Path cur = pq.top();
        pq.pop();

        int last_node = cur.path.back();
        if (last_node == end_id) {
            results.push_back(cur);
            continue;
        }

        for (const Edge& edge : adjList[last_node]) {
            int next_id = edge.endId;
            if (!cur.contains(next_id)) {
                pq.push(Path(cur, next_id, edge.time));
            }
        }
    }

    // 输出结果
    for (const auto& path : results) {
        std::cout << "总耗时: " << path.total_time << "分钟 - ";

        std::string output;
        const Station& start_station = getStation(path.path[0]);
        output = start_station.name + "(" + start_station.line + ")";

        for (size_t i = 1; i < path.path.size(); ++i) {
            const Station& prev = getStation(path.path[i - 1]);
            const Station& curr = getStation(path.path[i]);

            if (prev.name == curr.name) {
                output += "-换乘(" + curr.line + ")";
            }
            else {
                output += "-" + curr.name + "(" + curr.line + ")";
            }
        }
        std::cout << output << std::endl;
    }
}