// station.h
#pragma once
#include <string>
#include <vector>
using namespace std;
struct Station {
    int id;
    string name;
    string line;
    std::vector<std::string> lines;
    string status;  // "active"»ò"inactive"
    Station() = default;
    Station(int id, string name, string line, string status);

};
