// graph.cpp
#include "graph.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <iostream>

// === 辅助函数 ===
namespace {
    // 分割字符串（用于解析线路）
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
    initAdjList(); // 初始化邻接表
    buildLineIndex(); // 构建线路索引
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
            continue; // 或抛出异常
        }

        // 添加到邻接表（双向边）
        adjList[edge.startId].push_back(edge);
        adjList[edge.endId].emplace_back(edge.endId, edge.startId, edge.line,
            edge.direction, edge.time);
    }
}

// === 图结构初始化 ===
void MotorGraph::initAdjList() {
    adjList.resize(stations.size() + 1); // ID从1开始则+1，否则直接=size()
}

void MotorGraph::buildLineIndex() {
    for (const auto& [id, station] : stations) {
        for (const auto& line : station.lines) {
            lineStations[line].push_back(id);
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
    for (const auto& [id, station] : stations) {
        if (station.status == "closed") {
            closedStations.push_back(id);
        }
    }
    return closedStations;
}

// === 调试输出 ===
void MotorGraph::printAllStations() const {
    std::cout << "=== 所有站点 ===" << std::endl;
    for (const auto& [id, station] : stations) {
        std::cout << "ID: " << id
            << " | 名称: " << station.name
            << " | 线路: ";
        for (const auto& line : station.lines) {
            std::cout << line << " ";
        }
        std::cout << " | 状态: " << station.status << std::endl;
    }
}

void MotorGraph::printStationConnections(int stationId) const {
    if (!hasStation(stationId)) return;

    const auto& station = getStation(stationId);
    std::cout << "=== 站点连接 ===" << std::endl;
    std::cout << "ID: " << stationId << " | 名称: " << station.name << std::endl;

<<<<<<< HEAD
    const auto& edges = getConnectedEdges(stationId);
    for (const auto& edge : edges) {
        int connectedId = (edge.startId == stationId) ? edge.endId : edge.startId;
        std::cout << "  → ID: " << connectedId
            << " | 名称: " << getStation(connectedId).name
            << " | 线路: " << edge.line
            << " | 方向: " << edge.direction
            << " | 时间: " << edge.time << "分钟" << std::endl;
=======
    Path(const Path& prev, int next_id, int edge_time)
        : total_time(prev.total_time + edge_time),
        path(prev.path),
        node_set(prev.node_set) {
        path.push_back(next_id);
        node_set.insert(next_id);
    }

    bool contains(int node_id) const {
        return node_set.find(node_id) != node_set.end();
    }
};

// 自定义优先队列比较函数
struct PathCompare {
    bool operator()(const Path& a, const Path& b) const {
        return a.total_time > b.total_time; // 最小堆
    }
};


//===========================最短路径函数==============================
//需要传入图，开始站点，中止站点，以及1或3（分别表述输出1条或三条线路）

void findPaths(MotorGraph& graph, int start_id, int end_id, int option) {
    int k = (option == 1) ? 1 : 3; // 根据option确定k值
    std::priority_queue<Path, std::vector<Path>, PathCompare> pq;
    std::vector<Path> results; // 存储完整路径对象（包含耗时）

    // 初始化优先队列
    pq.push(Path(start_id));

    while (!pq.empty() && results.size() < k) {
        Path cur = pq.top();
        pq.pop();

        int last_node = cur.path.back();
        if (last_node == end_id) {
            results.push_back(cur);
            continue;
        }

        // 遍历当前节点的所有邻接边
        for (const Edge& edge : graph.edges[last_node]) {
            int next_id = edge.next_station_id;
            if (!cur.contains(next_id)) {
                pq.push(Path(cur, next_id, edge.time));
            }
        }
    }

    // 输出所有找到的路径
    for (const auto& path : results) {
        // 先输出总耗时
        std::cout << "总耗时: " << path.total_time << "分钟 - ";

        // 构建路径字符串
        std::string output;
        Station& start_station = graph.stations[path.path[0]];
        output = start_station.name + "(" + start_station.line + ")";

        for (int i = 1; i < path.path.size(); ++i) {
            Station& prev = graph.stations[path.path[i - 1]];
            Station& curr = graph.stations[path.path[i]];

            if (prev.name == curr.name) {
                // 换乘情况
                output += "-换乘(" + curr.line + ")";
            }
            else {
                // 正常站点移动
                output += "-" + curr.name + "(" + curr.line + ")";
            }
        }
        std::cout << output << std::endl;
>>>>>>> f420f8600be3d682ed1ae1404c9b8299abfbc95f
    }
}