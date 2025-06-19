// station.h
#pragma once
#include <string>
#include <vector>
#include <utility> // 用于pair

// 站点基础信息（完全匹配Station.csv格式）
struct Station {
    int id;                  // station_id
    std::string name;        // station_name
    std::string line;        // 主线路（如"1号线"）
    std::vector<std::string> lines; // 该站点所属的所有线路（从line字段解析，用'|'分隔）
    std::string status;      // "open"或"closed"

    // 构造函数（可选，便于初始化）
    Station(int id = -1, std::string name = "", std::string line = "", std::string status = "open")
        : id(id), name(std::move(name)), line(std::move(line)), status(std::move(status)) {
        if (!this->line.empty()) {
            lines.push_back(this->line); // 至少包含主线路
        }
    }
};

// 连接边信息（完全匹配Edge.csv格式）
struct Edge {
    int startId;            // start_station_id
    int endId;              // end_station_id
    std::string line;       // 所属线路（如"1号线"）
    std::string direction;  // 方向（如"上行"）
    int time;               // 时间（分钟）

    // 构造函数（可选，便于初始化）
    Edge(int start = -1, int end = -1, std::string line = "",
        std::string dir = "", int t = 0)
        : startId(start), endId(end), line(std::move(line)),
        direction(std::move(dir)), time(t) {
    }
};

// 线路信息扩展（可选，用于高级查询）
struct LineInfo {
    std::string name;       // 线路名
    std::vector<int> stationIds; // 线路包含的站点ID（按顺序）
    std::vector<std::pair<int, int>> segments; // 连接段（startId, endId）
};