#include <iostream>
#include <string>
#include <fstream>
#include "instance.h"

using namespace std;

Instance::Instance(string inputPath){
    //Constructor function will parse the input file and assign the values of the instance.
    ifstream fileStream;
    string line;

    fileStream.open(inputPath);
    if(fileStream.is_open()){
        cout << "Leyendo archivo " << inputPath << endl;

        while(getline(fileStream,line)){
            for(char& c: line){
                cout << c;
            }
            cout << endl;
        }
    }
    else cout << "No ha sido posible abrir el archivo";

}
