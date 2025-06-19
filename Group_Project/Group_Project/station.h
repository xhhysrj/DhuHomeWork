// station.h
#pragma once
#include <string>
#include <vector>
using namespace std;
struct Station {
    int id;
    string name;
    vector<string> lines;
    string status;  // "open"»ò"closed"
};
