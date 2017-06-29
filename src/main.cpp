#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include <limits>
#include <algorithm>
#include <time.h>
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

void generate_output_file(string filename, evacuationPlan answer, vector<int> busCost, vector<int> rescued){
    ofstream myfile;

    filename.replace(filename.end()-3, filename.end(), "output");

    myfile.open(filename);

    for (int i = 0; i < answer.schedule.size(); i++){
        vector<tuple<int,int>> route = answer.schedule.at(i);
        myfile << i+1 << " " << busCost.at(i) << " ";
        for (tuple<int,int> arc : route){
            myfile << "(" << get<0>(arc)+1 << "," << get<1>(arc)+1 << ")";
        }
        myfile << endl;
    }

    int maxSchedule = *max_element(busCost.begin(), busCost.end());

    for(int i : busCost){
        if (i == maxSchedule){
            myfile << i << " ";
        }
    }
    myfile << endl;

    for (int rescue : rescued){
        myfile << rescue << endl;
    }

    myfile.close();
}

int main(int argc, char *argv[] ){
    if (argc == 1){
        cout << "No parameters pased" << endl;
        cout << "Usage: \n ./bep <path to BEP instance file>\n bep <path to BEP instance file> <number of restarts>" << endl;
        cout << "Using test instance... " << endl;
        Instance instance = Instance("inputs/InstanceBEP-1-3-3-3.txt");
        print_solution(instance.solve().schedule);
    }
    else if(argc == 2){
        cout << argv[1] << endl;
        Instance instance(argv[1]);

        evacuationPlan answer = instance.solve();
        cout << "Solution Time: " << answer.time << "\n" << "Schedule:" << endl; 
        print_solution(answer.schedule);
    }
    else if(argc == 3){
        cout << "Solving the input file with "<< argv[2] <<"restarts" << endl;
        int restarts = atoi(argv[2]);
        Instance instance(argv[1]);
        
        evacuationPlan bestAnswer;
        bestAnswer.time = numeric_limits<int>::max();
        
        time_t startTime = clock();
        cout << endl;
        for(int i = 0; i < restarts; i++){
            cout << "\r"<< "Restarting (" << i << ")";
            evacuationPlan answer = instance.solve();
            if (answer.time < bestAnswer.time){
                bestAnswer = answer;
            }
        }
        cout << endl;
        time_t endTime = clock();
        cout << "-------------------------------------------------------------------------------------------------"<<endl;
        cout << "Best Solution Found!" << endl;
        cout << "Solution Time: " << bestAnswer.time << "\n" << "Schedule:" << endl; 
        print_solution(bestAnswer.schedule);
        cout << "Generated in " << (endTime-startTime)/double(CLOCKS_PER_SEC) << "[s]" << endl;
        generate_output_file(argv[1], bestAnswer, instance.get_buses_costs(bestAnswer.schedule), instance.get_total_sink(bestAnswer.schedule));
    }
    return 0;
}

