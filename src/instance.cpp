#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include "instance.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

vector<int> Instance::convert_vector(string inputString){
    //Converts a space separated numbers string to a list of integers
    vector<string> inputVector;
    vector<int> outVector;

    boost::trim(inputString);
    boost::split(inputVector, inputString, boost::is_any_of(" "));
    //for (int i = 0; i < inputVector.size(); i++) cout << inputVector[i] << ","; cout << endl;
    for(int i = 0; i < inputVector.size(); i++){
        outVector.push_back(stoi(inputVector.at(i)));
    }

    return outVector;
}

void Instance::print(){
    cout << "Total Buses        : " << totalBuses << endl;
    cout << "Bus Capacity       : " << busesCapacity << endl;
    cout << "Total Sources      : " << totalSources << endl;
    cout << "Total Sinks        : " << totalSink << endl;
    cout << "Total Stations     : " << totalStations << endl;
    cout << "Sink Capacity      : " ; for (int i=0; i<sinkCapacity.size();i++) cout<< sinkCapacity.at(i) <<","; cout << endl;
    cout << "People in Source   : " ; for (int i=0; i<sourcePeople.size();i++) cout<< sourcePeople.at(i) <<","; cout << endl;
    cout << "Buses in Station   : " ; for (int i=0; i<stationBuses.size();i++) cout<< stationBuses.at(i) <<","; cout << endl;

}

Instance::Instance(string inputPath){
    //Constructor function will parse the input file and assign the values of the instance.
    ifstream fileStream;
    string line;

    fileStream.open(inputPath);
    if(fileStream.is_open()){
        vector<string> splitLine;
        
        cout << "Leyendo archivo " << inputPath << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalBuses = stoi(splitLine.at(0));
        busesCapacity = stoi(splitLine.at(1));

        //cout << "Buses totales: " << totalBuses << endl;
        //cout << "Capacidad Buses: " << busesCapacity << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalStations = stoi(splitLine.at(0));
        stationBuses = convert_vector(splitLine.at(1));

        //cout << "Estaciones totales: "<< totalStations << endl;
        //cout << "Numero de estaciones: " << stationBuses.size() << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalSources = stoi(splitLine.at(0));
        sourcePeople = convert_vector(splitLine.at(2));

        //cout << "Cantidad de puntos de colection"<< totalSources << " o " << sourcePeople.size() << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalSink = stoi(splitLine.at(0));
        sinkCapacity = convert_vector(splitLine.at(2));
        
        //cout << "refugios totales " << totalSink << ": "; for (int i=0; i<sinkCapacity.size();i++) cout<< sinkCapacity.at(i) <<","; cout << endl;

        
        getline(fileStream, line);
        for(int i = 0; i<totalStations; i++){
            getline(fileStream, line);

            boost::split(splitLine, line, boost::is_any_of(":"));
            vector<int> tmpVector = convert_vector(splitLine.at(1));
            stationSourceDistance.push_back(tmpVector);
        }

        getline(fileStream,line);
        for(int i = 0; i<totalSources; i++){
            getline(fileStream, line);

            boost::split(splitLine, line, boost::is_any_of(":"));
            vector<int> tmpVector = convert_vector(splitLine.at(1));
            sourceSinkDistance.push_back(tmpVector);
        }


    }
    else cout << "No ha sido posible abrir el archivo";

}

solution Instance::generate_random_solution(){ 
    //generate solution for a single bus, then distribute it to the total amount of buses
    vector<int> peopleLeft = sourcePeople;
    vector<int> shelterCapacity = sinkCapacity;
    int totalSources = sourcePeople.size();
    int totalSinks = sinkCapacity.size();

    default_random_engine generator;
    uniform_int_distribution<int> sourceDist(0,totalSources-1);
    uniform_int_distribution<int> sinkDist(0,totalSinks-1);

    vector<tuple<int,int>> flatSolution;

    while(peopleLeft[min_element(peopleLeft.begin(), peopleLeft.end())-peopleLeft.begin()] != 0){
        //feasible indexes 
        int i = sourceDist(generator);  //source (collection point) index
        int j = sinkDist(generator);    //sink (shelter) index

        while(peopleLeft.at(i) != 0){
            i = sourceDist(generator);
        }
        while(shelterCapacity.at(j) != 0){
            j = sinkDist(generator);
        }
        
        int evacuatedPeople = min(min(shelterCapacity.at(j), peopleLeft.at(i)), busesCapacity);

        shelterCapacity[j] -= evacuatedPeople;
        peopleLeft[i] -= evacuatedPeople;
        
        tuple<int,int> tmpTuple(i,j);
        flatSolution.push_back(tmpTuple);
    }

    solution fullSolution;
    fullSolution.push_back(flatSolution);

    return fullSolution;
}

solution Instance::solve(){
    return generate_random_solution();
}
