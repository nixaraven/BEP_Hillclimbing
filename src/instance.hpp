#ifndef INSTANCE
#define INSTANCE 
#include <vector>
#include <string>

using namespace std;

class Instance{
    public:
        int totalBuses;        //Total amount of buses avaliable
        int busesCapacity;     //Max amount of people that a bus can carry
        int totalSources;      //Total amount of source points
        int totalSink;         //Total amount of sink points
        int totalStations;     //Total amount of bus stations

        vector<vector<int>> sourceSinkDistance;       //Distance between sink and source points
        vector<vector<int>> stationSourceDistance;    //Distance between bus stations and source points

        vector<int> sourcePeople;      //Amount of people in each source (or collection point)
        vector<int> stationBuses;      //Amount of buses available for each station
        vector<int> sinkCapacity;      //Sink (or shelter) capacity

        Instance(string inputPath);
    private:
        vector<int> convertVector(string inputString);
};
#endif
