// station.h
#pragma once
#include <string>
#include <vector>
using namespace std;
struct Station {
    int id;
    string name;
    string line;
    string status;  // "active"»ò"inactive"
    Station(int id, string name, string line, string status);
};
