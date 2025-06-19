
#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include "station.h" 


class StationQuery {
private:
    std::unordered_map<int, Station> stations_; // ´æ´¢Õ¾µãÊý¾Ý

public:
    explicit StationQuery(const std::string& filePath = "Station.csv");
    void loadData(const std::string& filePath);
    std::string getStationName(int id) const;
    std::string getStationStatus(int id) const;
    std::vector<std::string> getLinesByStation(int id) const;
    std::vector<Station> fuzzySearch(const std::string& keyword) const;
    std::vector<Station> searchByName(const std::string& name) const;
    void printAllStations() const;
    void printClosedStations() const;

private:
    static bool containsIgnoreCase(const std::string& str, const std::string& substr);
    static std::vector<std::string> splitString(const std::string& s, char delimiter);
};