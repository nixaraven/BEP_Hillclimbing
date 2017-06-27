#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
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

    //literally random stuff
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    default_random_engine generator(seed);
    uniform_int_distribution<int> sourceDist(0,totalSources-1);
    uniform_int_distribution<int> sinkDist(0,totalSinks-1);

    vector<tuple<int,int>> flatSolution;

    //Generating feasible solution asuming a single bus
    while(peopleLeft[max_element(peopleLeft.begin(), peopleLeft.end())-peopleLeft.begin()] != 0){
        //feasible indexes
        int i = sourceDist(generator);  //source (collection point) index
        int j = sinkDist(generator);    //sink (shelter) index

        while(peopleLeft.at(i) == 0){
            i = sourceDist(generator);
        }
        while(shelterCapacity.at(j) == 0){
            j = sinkDist(generator);
        }
        
        int evacuatedPeople = min(min(shelterCapacity.at(j), peopleLeft.at(i)), busesCapacity);


        shelterCapacity[j] -= evacuatedPeople;
        peopleLeft[i] -= evacuatedPeople;
        
        
        tuple<int,int> tmpTuple(i,j);
        flatSolution.push_back(tmpTuple);
    }
    
    //Distributing solution equally increasing bus parallelism that reduces total time.
    solution fullSolution;

    int delta = flatSolution.size()/totalBuses;
    int i;
    for(i=0; i<totalBuses-1; i++){
        vector<tuple<int,int>> tmpVector(flatSolution.begin()+delta*i,flatSolution.begin()+delta*(i+1));
        fullSolution.push_back(tmpVector);
    }

    i += 1;
    vector<tuple<int,int>> tmpVector(flatSolution.begin() + delta * i, flatSolution.end());
    fullSolution.push_back(tmpVector);

    return fullSolution;
}

vector<solution> Instance::get_hood(solution initialSol){
    //generating neighborhood using movement over the initial solution
    vector<solution> neighborhood;
    int solutionSize = initialSol.size();
    
    for(int i = 0; i < initialSol.size(); i++){
        for(int j=0; j < initialSol.size(); j++){
            if(i != j){
                solution swappedSolution = initialSol;
                swappedSolution.at(i).swap(swappedSolution.at(j));
                neighborhood.push_back(swappedSolution);
            }
        }
    }
    return neighborhood;
}

vector<solution> Instance::get_neighborhood(solution initialSol){
    vector<solution> neighborhood;
    
    for(int i = 0; i < initialSol.size(); i++){
        vector<tuple<int,int>> schedule = initialSol.at(i);
        for(int j = 0; j < schedule.size(); j++){
            tuple<int,int> arc = schedule.at(0);
            schedule.erase(schedule.begin());
            schedule.push_back(arc);
            cout << "--------------" << endl;
            
            solution newSol = initialSol;
            vector<tuple<int,int>> newSchedule = schedule;
            newSol.at(i) = newSchedule;
            
            neighborhood.push_back(newSol);
        }
    }
    

    return neighborhood;
}

int Instance::evaluate_solution(solution inputSol){
    vector<int> busesLeft = stationBuses;
    int currentStation = 0;
    int bestTime = 0;

    for (vector<tuple<int,int>> busPath : inputSol){
        int totalTime = 0;
        for(int a = 0 ; a < busPath.size() ; a++){
            tuple<int,int> currentArc = busPath.at(a);
            if(a==0) totalTime += stationSourceDistance.at(currentStation).at(get<0>(currentArc));
            else totalTime += sourceSinkDistance.at(get<0>(currentArc)).at(get<1>(busPath.at(a-1))); 
            totalTime += sourceSinkDistance.at(get<0>(currentArc)).at(get<1>(currentArc));
        }
        if (totalTime >= bestTime) bestTime = totalTime;
        
        busesLeft.at(currentStation) -= 1;
        if (busesLeft.at(currentStation) == 0) currentStation += 1;
    }
    return bestTime;
}

solution Instance::solve(){
    //Procedure hill-climbing
    //local ← FALSE
        //sc ← select a point at random
        //Repeat
            //select sn the best quality point in N (sc )
            //If f (sn) is better than f (sc ) Then
                //sc ← sn
            //Else
                //local ← TRUE
        //Until local
    //End
    //1. Generating a starting solution
    solution bestSolution = generate_random_solution();
    int bestEvaluation = evaluate_solution(bestSolution);
    //2. Generating neighborhood moving the current solution
    vector<solution> neighborhood = get_neighborhood(bestSolution);
    //3. select neighbor and compare it to the current solution, if there is not the algorithm found the best local solution.
    while(true){
        bool localSolution = true;
        for(solution neighbor : neighborhood){
            int currentEvaluation = evaluate_solution(neighbor);
            cout <<"actual: "<< currentEvaluation << " mejor: " << bestEvaluation << endl;
            if(currentEvaluation < bestEvaluation){
                localSolution = false;
                bestEvaluation = currentEvaluation;
                bestSolution = neighbor;
            }
        }
        if(localSolution) break;
    }

    return bestSolution;
}
