#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <vector>
#include "instance.hpp"

using namespace std;

void printSolution(solution inputSol){
    for(vector<tuple<int,int>> vector: inputSol){
        for(tuple<int,int> t: vector){
            cout << "("<<get<0>(t)<<","<<get<1>(t)<<")";
        }
        cout << endl;
    }
}

int main(int argc, char *argv[] ){
    cout << "Faltan Parametros, utilizando instancia 1-4-2-4" << endl;
    Instance instance = Instance("inputs/InstanceBEP-1-3-3-3.txt");

    instance.print();
    
    printSolution(instance.solve());

    return 0;
}
