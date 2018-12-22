#include <dirent.h>
#include <iostream>
#include <cstdio>
#include "greedySolution.h"
#include "SaSolution.h"

using namespace std;

int main() {
	// read image filenames
	string srcFileDir = "../Instances/" ;
	string greedyResultPath = "../result/greedyResult.txt" ;
    string saResultPath = "../result/saResult.txt" ;

    int startIndex = 1 , endIndex = 71;
    //int startIndex = 1 , endIndex = 71;
    double wholeResult = 0;

    ofstream os(greedyResultPath);
    for(int i = startIndex ; i <= endIndex ; i ++){
        char filePath[200] ;
        sprintf(filePath , "%sp%d" , srcFileDir.c_str() , i);
        if( opendir(filePath) == NULL ){
            greedySolution greedySol;
            wholeResult += greedySol.run( (string)filePath , os);
        }
    }
    os << "71 cases cost sum is " << wholeResult << endl;
    wholeResult = 0;
    os.close();
    os.open(saResultPath);
    for(int i = startIndex ; i <= endIndex ; i ++){
        char filePath[200] ;
        sprintf(filePath , "%sp%d" , srcFileDir.c_str() , i);
        if( opendir(filePath) == NULL ){
            SaSolution saSol;

            wholeResult += saSol.run( (string)filePath , os);
        }
    }
    os << "71 cases cost sum is " << wholeResult << endl;
    os.close();


    return 0;
}
