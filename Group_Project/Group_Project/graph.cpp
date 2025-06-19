#pragma once
#include "graph.h"
#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<fstream>
using namespace std;
Motor_graph::Motor_graph() {
	//读取站点信息
	ifstream station_file("Station.csv");
	string line;
	Vers = 0;
	getline(station_file, line);
	while (getline(station_file, line)) {
		stringstream SS(line);
		vector<string>tokens;
		string token;
		while (getline(SS, token, ',')) {
			tokens.push_back(token);
		}
		Station.emplace_back(stoi(tokens[0]), tokens[1], tokens[2], tokens[3]);
		Vers++;
	}
	//读取路线信息
	ifstream edge_file("Station.csv");
	edges.resize(Vers);
	string edge_line;
	getline(edge_file, edge_line);
	while (getline(edge_file, edge_line)) {
		stringstream SS(edge_line);
		vector<string>tokens;
		string token;
		while (getline(SS, token, ',')) {
			tokens.push_back(token);
		}
		edges[stoi(tokens[0]) - 1001].emplace_back(stoi(tokens[1]), tokens[2], tokens[4]);
	}
}