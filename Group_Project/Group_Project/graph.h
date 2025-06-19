#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <list>
#include "station.h"

class Edge {
public:
    std::string line;
    int time;
    int next_station_id;
    Edge(int to_id, const std::string& line_name, int travel_time);
};

class MotorGraph {
public:
    int Vers;
    std::vector<Station> stations;
    std::vector<std::list<Edge>> edges;

    MotorGraph();  // 默认构造函数
    void reset();  // 重置站点状态
    void update(); // 更新站点状态
    void getClosedStations(); // 获取关闭站点

};

#endif // GRAPH_H