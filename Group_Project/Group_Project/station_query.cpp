#include "station_query.h"
#include"station.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <iostream>

// === 构造函数与数据加载 ===
StationQuery::StationQuery(const std::string& filePath) {
    loadData(filePath);
}

void StationQuery::loadData(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file) {
        throw std::runtime_error("无法打开站点数据文件: " + filePath);
    }

    std::string line;
    std::getline(file, line); // 跳过标题行

    while (std::getline(file, line)) {
        Station station;
        std::istringstream ss(line);
        std::string field;

        // 解析ID
        std::getline(ss, field, ',');
        try {
            station.id = std::stoi(field);
        }
        catch (...) {
            throw std::runtime_error("无效的站点ID格式: " + field);
        }

        // 解析名称
        std::getline(ss, station.name, ',');

        // 解析线路（用|分隔）
        std::string linesStr;
        std::getline(ss, linesStr, ',');
        station.lines = StationQuery::splitString(linesStr, '|');

        // 解析状态
        std::getline(ss, station.status, ',');

        // 存入查找表
        stations_[station.id] = station;
    }
}

// === 核心查询接口 ===
std::string StationQuery::getStationName(int id) const {
    auto it = stations_.find(id);
    if (it == stations_.end()) {
        throw std::out_of_range("站点ID " + std::to_string(id) + " 不存在");
    }
    return it->second.name;
}

std::string StationQuery::getStationStatus(int id) const {
    return stations_.at(id).status; // 直接返回状态字段
}

std::vector<std::string> StationQuery::getLinesByStation(int id) const {
    return stations_.at(id).lines; // at()自动检查边界
}

// === 搜索功能 ===
std::vector<Station> StationQuery::fuzzySearch(const std::string& keyword) const {
    std::vector<Station> results;
    if (keyword.empty()) return results;

    // C++11/14兼容写法
    for (const auto& kv : stations_) {
        const Station& station = kv.second;
        if (StationQuery::containsIgnoreCase(station.name, keyword)) {
            results.push_back(station);
        }
    }
    return results;
}

// === 名称精确搜索 ===
std::vector<Station> StationQuery::searchByName(const std::string& name) const {
    std::vector<Station> results;
    for (const auto& kv : stations_) {
        const Station& station = kv.second;
        if (station.name == name) {
            results.push_back(station);
        }
    }
    return results;
}

// === 批量输出 ===
void StationQuery::printAllStations() const {
    std::cout << "\n=== 所有站点信息 ===" << std::endl;
    std::cout << std::left
        << std::setw(8) << "ID"
        << std::setw(20) << "名称"
        << std::setw(10) << "状态"
        << "线路" << std::endl;

    for (const auto& kv : stations_) {
        const int& id = kv.first;
        const Station& station = kv.second;

        std::cout << std::setw(8) << id
            << std::setw(20) << station.name
            << std::setw(10) << (station.status == "open" ? "运营中" : "关闭中");

        for (size_t i = 0; i < station.lines.size(); ++i) {
            if (i != 0) std::cout << "|";
            std::cout << station.lines[i];
        }
        std::cout << std::endl;
    }
}

void StationQuery::printClosedStations() const {
    std::cout << "\n=== 关闭站点列表 ===" << std::endl;
    bool found = false;
    for (const auto& kv : stations_) {
        const int& id = kv.first;
        const Station& station = kv.second;

        if (station.status == "closed") {
            std::cout << "ID: " << id << " 名称: " << station.name << std::endl;
            found = true;
        }
    }
    if (!found) {
        std::cout << "当前没有关闭的站点" << std::endl;
    }
}

// === 静态辅助方法 ===
bool StationQuery::containsIgnoreCase(const std::string& str, const std::string& substr) {
    auto it = std::search(str.begin(), str.end(),
        substr.begin(), substr.end(),
        [](char ch1, char ch2) {
            return std::tolower(ch1) == std::tolower(ch2);
        });
    return it != str.end();
}

std::vector<std::string> StationQuery::splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream ss(s);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }
    return tokens;
}