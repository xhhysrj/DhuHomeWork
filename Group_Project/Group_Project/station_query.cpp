#include "station.h"
#include "station_query.h"

#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>
#include "station_loader.h"

// === 构造函数 ===
StationQuery::StationQuery(const std::string& stationFile, const std::string& edgeFile) {
    graph_.loadStations(stationFile);
    graph_.loadEdges(edgeFile);
}

// === 基础查询实现 ===
std::string StationQuery::getStationName(int stationId) const {
    return graph_.getStation(stationId).name;
}

std::vector<std::string> StationQuery::getStationLines(int stationId) const {
    return graph_.getStation(stationId).lines;
}

std::string StationQuery::getStationStatus(int stationId) const {
    return graph_.getStation(stationId).status;
}
bool StationQuery::stringContains(const std::string& str, const std::string& substr, bool ignoreCase) {
    if (substr.empty()) return true; // 空字符串总是匹配

    auto it = std::search(
        str.begin(), str.end(),
        substr.begin(), substr.end(),
        [ignoreCase](char ch1, char ch2) {
            return ignoreCase ? (std::tolower(ch1) == std::tolower(ch2)) : (ch1 == ch2);
        }
    );
    return it != str.end();
}

// === 高级查询实现 ===
std::vector<int> StationQuery::findStationsByName(const std::string& name, bool fuzzyMatch) const {
    std::vector<int> result;
    for (const auto& pair : stations_) {
        int id = pair.first;
        const Station& station = pair.second;

        if (fuzzyMatch ? stringContains(station.name, name, true)
            : (station.name == name)) {
            result.push_back(id);
        }
    }
    return result;
}

std::vector<int> StationQuery::getStationsOnLine(const std::string& lineName) const {
    return graph_.getStationsByLine(lineName);
}

std::vector<Edge> StationQuery::getStationConnections(int stationId) const {
    std::vector<Edge> connections;
    const auto& edges = graph_.getConnectedEdges(stationId);
    connections.assign(edges.begin(), edges.end());
    return connections;
}
void StationQuery::findPaths(int start_id, int end_id, int option) {
    graph_.findPaths(start_id, end_id, option);
}
std::vector<std::string> StationQuery::getAllLines() const {
    return graph_.getAllLines();
}
void StationQuery::setStationStatus(int stationId, const std::string& status) {
    graph_.setStationStatus(stationId, status);
}
// === 状态查询实现 ===
bool StationQuery::isStationClosed(int stationId) const {
    return graph_.getStation(stationId).status == "closed";
}

std::vector<int> StationQuery::getAllClosedStations() const {
    return graph_.getClosedStations();
}

// === 打印输出实现 ===
void StationQuery::printStationInfo(int stationId) const {
    const Station& s = graph_.getStation(stationId);
    std::cout << "\n=== 站点详情 ===\n"
        << "ID: " << stationId << "\n"
        << "名称: " << s.name << "\n"
        << "状态: " << (isStationClosed(stationId) ? "关闭" : "运营") << "\n"
        << "线路: ";

    for (size_t i = 0; i < s.lines.size(); ++i) {
        if (i != 0) std::cout << ", ";
        std::cout << s.lines[i];
    }

    // 打印连接信息
    const auto& connections = getStationConnections(stationId);
    if (!connections.empty()) {
        std::cout << "\n--- 连接站点 ---\n";
        for (const auto& edge : connections) {
            int connectedId = (edge.startId == stationId) ? edge.endId : edge.startId;
            std::cout << "→ " << getStationName(connectedId)
                << " (" << edge.line << "线 "
                
                << edge.time << "分钟)\n";
        }
    }
}


void StationQuery::printLineSummary(const std::string& lineName) const {
    auto stations = getStationsOnLine(lineName);
    if (stations.empty()) {
        std::cout << "未找到线路: " << lineName << "\n";
        return;
    }

    std::cout << "\n=== " << lineName << " ===\n"
        << "站点数: " << stations.size() << "\n"
        << "途经站点: ";

    for (size_t i = 0; i < stations.size(); ++i) {
        if (i != 0) std::cout << " → ";
        std::cout << getStationName(stations[i]);
    }
    std::cout << "\n";
}

