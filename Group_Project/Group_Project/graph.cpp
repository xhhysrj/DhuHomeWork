#include "graph.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <unordered_set>
#include <algorithm>
#include <string>
#include <functional>
#include <set>
#include <unordered_map>
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

struct Path {
    int total_time;
    std::vector<int> path; // 存储节点ID序列
    std::unordered_set<int> node_set; // 快速检查节点是否在路径中

    Path(int start_id) : total_time(0) {
        path.push_back(start_id);
        node_set.insert(start_id);
    }

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

void findPaths(MotorGraph& graph, int start_id, int end_id, int option) {
    int k = (option == 1) ? 1 : 3; // 根据option确定k值
    std::priority_queue<Path, std::vector<Path>, PathCompare> pq;
    std::vector<std::vector<int>> results;

    // 初始化优先队列
    pq.push(Path(start_id));

    while (!pq.empty() && results.size() < k) {
        Path cur = pq.top();
        pq.pop();

        int last_node = cur.path.back();
        if (last_node == end_id) {
            results.push_back(cur.path);
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
        std::string output;
        Station& start_station = graph.stations[path[0]];
        output = start_station.name + "(" + start_station.line + ")";

        for (int i = 1; i < path.size(); ++i) {
            Station& prev = graph.stations[path[i - 1]];
            Station& curr = graph.stations[path[i]];

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
    }
}