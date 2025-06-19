// include/dataloader/station_loader.h
#pragma once
#include "station.h"
#include <vector>

class StationLoader {
public:
    explicit StationLoader(const std::string& filepath);
    

private:
    std::vector<Station> stations_;
};