#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>

// === Edge类实现 ===
Edge::Edge(int to_id, const std::string& line_name, int travel_time)
    : next_station_id(to_id), line(line_name), time(travel_time) {
}

// === MotorGraph类实现 ===

// 构造函数：从文件加载数据
MotorGraph::MotorGraph() : Vers(0) {
    // 加载站点数据
    std::ifstream station_file("Station.csv");
    if (!station_file) {
        throw std::runtime_error("无法打开Station.csv文件");
    }

    std::string line;
    std::getline(station_file, line); // 跳过标题行

    while (std::getline(station_file, line)) {
        std::istringstream ss(line);
        Station station;
        std::string field;

        // 解析ID
        std::getline(ss, field, ',');
        station.id = std::stoi(field);

        // 解析名称
        std::getline(ss, station.name, ',');

        // 解析线路
        std::string linesStr;
        std::getline(ss, linesStr, ',');
        std::istringstream linesStream(linesStr);
        std::string lineName;
        while (std::getline(linesStream, lineName, '|')) {
            station.lines.push_back(lineName);
        }

        // 解析状态
        std::getline(ss, station.status, ',');

        stations.push_back(station);
        Vers++;
    }

    // 初始化邻接表
    edges.resize(Vers);

    // 加载边数据
    std::ifstream edge_file("Edges.csv");
    if (!edge_file) {
        throw std::runtime_error("无法打开Edges.csv文件");
    }

    std::getline(edge_file, line); // 跳过标题行

    while (std::getline(edge_file, line)) {
        std::istringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;

        while (std::getline(ss, token, ',')) {
            tokens.push_back(token);
        }

        if (tokens.size() < 5) continue;

        int from_id = std::stoi(tokens[0]) - 1001; // 假设ID从1001开始
        int to_id = std::stoi(tokens[1]);
        std::string line_name = tokens[2];
        int time = std::stoi(tokens[4]); // 跳过tokens[3]

        if (from_id >= 0 && from_id < Vers) {
            edges[from_id].emplace_back(to_id, line_name, time);
        }
    }
}

// 重置站点状态
void MotorGraph::reset() {
    std::ifstream station_file("Station.csv");
    if (!station_file) return;

    std::string line;
    std::getline(station_file, line); // 跳过标题行

    while (std::getline(station_file, line)) {
        std::istringstream ss(line);
        Station temp;
        std::string field;

        std::getline(ss, field, ',');
        int id = std::stoi(field);

        // 查找对应站点
        for (auto& station : stations) {
            if (station.id == id) {
                // 跳过名称和线路
                std::getline(ss, field, ','); // name
                std::getline(ss, field, ','); // lines
                std::getline(ss, station.status, ','); // 更新状态
                break;
            }
        }
    }
}

// 更新站点状态
void MotorGraph::update() {
    std::ifstream update_file("update_station_status.csv");
    if (!update_file) return;

    std::string line;
    std::getline(update_file, line); // 跳过标题行

    while (std::getline(update_file, line)) {
        std::istringstream ss(line);
        std::string station_name, line_name, new_status;

        std::getline(ss, station_name, ',');
        std::getline(ss, line_name, ',');
        std::getline(ss, new_status, ',');

        for (auto& station : stations) {
            if (station.name == station_name) {
                // 检查线路是否匹配
                for (const auto& line : station.lines) {
                    if (line == line_name) {
                        station.status = new_status;
                        break;
                    }
                }
            }
        }
    }
}

// 获取关闭的站点
void MotorGraph::getClosedStations() {
    std::cout << "\n=== 关闭站点列表 ===" << std::endl;
    bool found = false;

    for (const auto& station : stations) {
        if (station.status == "closed") {
            std::cout << "ID: " << station.id
                << " 名称: " << station.name
                << " 线路: ";
            for (const auto& line : station.lines) {
                std::cout << line << " ";
            }
            std::cout << std::endl;
            found = true;
        }
    }

    if (!found) {
        std::cout << "当前没有关闭的站点" << std::endl;
    }
}