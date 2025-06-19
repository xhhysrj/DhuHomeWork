#pragma once
#ifndef GRAPH_H
#define GRAPH_H
#include<iostream>
#include<list>
#include<vector>
#include<string>
#include"station.h"
using namespace std;
class edge {
public:
	string line;
	int time;
	int next_station_id;
	edge(int next_station_id,string line,int time ):line(line),time(time),next_station_id(next_station_id){}
};
class Motor_graph {
public:
	int Vers;
	vector<Station> Station;
	vector<list<edge>>edges;
	Motor_graph();//默认构造函数，从文件中读取站点和路线信息
	void Motor_garph_reset();//用Stations文件重置站点状态
	void Motor_graph_update();//用update_station_status文件更新站点状态
	void get_Station_closed();//获取关闭的站点
};
#endif // !GRAPH_H
