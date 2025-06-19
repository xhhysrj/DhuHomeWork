
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <stdexcept>
using namespace std;

class StationInfo {
public:
   
    explicit StationInfo(const string& filePath = "Station.csv");

    
    string getStationName(int station_id) const;
    string getStationStatus(int station_id) const;
    vector<string> getLinesByStation(int station_id) const;

    
    void printAllStations() const;
    void printClosedStations() const;

private:
    struct StationData {
        int id;
        string name;
        vector<std::string> lines;
        string status; 
    };

    unordered_map<int, StationData> stations_;

    void loadData(const string& filePath);
    vector<std::string> splitLine(const string& line, char delimiter) const;
};

#endif // STATION_INFO_H