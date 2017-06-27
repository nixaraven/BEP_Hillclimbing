#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include "instance.hpp"

using namespace std;

void print_solution(solution inputSol){
    for(vector<tuple<int,int>> vector: inputSol){
        cout <<"{";
        for(tuple<int,int> t: vector){
            cout << "("<<get<0>(t)+1<<","<<get<1>(t)+1<<")";
        }
        cout <<"}"<< endl;
    }
}

int main(int argc, char *argv[] ){
    if (argc == 1){
        cout << "No parameters pased" << endl;
        cout << "Usage: ./bep <path to BEP instance file>" << endl;
        cout << "Using test instance... " << endl;
        Instance instance = Instance("inputs/InstanceBEP-1-3-3-3.txt");
        print_solution(instance.solve());
    }
    else if(argc == 2){
        cout << argv[1] << endl;
        Instance instance(argv[1]);
        print_solution(instance.solve());
    }
    else if(argc == 3){
        //argc 3 == testing!
        Instance instance = Instance("inputs/InstanceBEP-1-3-3-3.txt");
        solution sol = instance.solve();
        print_solution(sol);
        for(int i = 0; i < sol.size(); i++){
            solution tempSol = sol;
            vector<tuple<int,int>> schedule = tempSol.at(i);
            for(int j = 0; j < schedule.size(); j++){
                tuple<int,int> arc = schedule.at(0);
                schedule.erase(schedule.begin());
                schedule.push_back(arc);
                cout << "--------------" << endl;
                
                solution newSol = sol;
                vector<tuple<int,int>> newSchedule = schedule;
                newSol.at(i) = newSchedule;

                print_solution(newSol);
            }
        }
    }
    
    
    return 0;
}
