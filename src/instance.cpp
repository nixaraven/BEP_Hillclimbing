#include <iostream>
#include <string>
#include <fstream>
#include "instance.hpp"
#include <boost/algorithm/string.hpp>

using namespace std;

vector<int> Instance::convertVector(string inputString){
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

        cout << "Buses totales: " << totalBuses << endl;
        cout << "Capacidad Buses: " << busesCapacity << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalStations = stoi(splitLine.at(0));
        stationBuses = convertVector(splitLine.at(1));

        cout << "Estaciones totales: "<< totalStations << endl;
        cout << "Numero de estaciones: " << stationBuses.size() << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalSources = stoi(splitLine.at(0));
        sourcePeople = convertVector(splitLine.at(2));

        cout << "Cantidad de puntos de colection"<< totalSources << " o " << sourcePeople.size() << endl;

        getline(fileStream, line);
        boost::split(splitLine, line, boost::is_any_of(":"));
        totalSink = stoi(splitLine.at(0));
        sinkCapacity = convertVector(splitLine.at(2));
        
        cout << "refugios totales " << totalSink << ": "; for (int i=0; i<sinkCapacity.size();i++) cout<< sinkCapacity.at(i) <<","; cout << endl;

        
        getline(fileStream, line);
        for(int i = 0; i<totalStations; i++){
            getline(fileStream, line);

            boost::split(splitLine, line, boost::is_any_of(":"));
            vector<int> tmpVector = convertVector(splitLine.at(1));
            stationSourceDistance.push_back(tmpVector);
        }

        getline(fileStream,line);
        for(int i = 0; i<totalSources; i++){
            getline(fileStream, line);

            boost::split(splitLine, line, boost::is_any_of(":"));
            vector<int> tmpVector = convertVector(splitLine.at(1));
            sourceSinkDistance.push_back(tmpVector);
        }


        //while(getline(fileStream,line)){
        //    vector<string> splitLine;
        //    boost::split(splitLine, line, boost::is_any_of(": "));
        //    cout << "largo del vector" << splitLine.size() << endl;
        //}
    }
    else cout << "No ha sido posible abrir el archivo";

}
