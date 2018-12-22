#ifndef SOLUTION_H_INCLUDED
#define SOLUTION_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <time.h>

using namespace std;

class Solution{
protected:
    int faciltyNum , customerNum;
    vector<double>  demandVec ,                 //所有顾客的存放容量需求
                    facilityCapacity ,          //所有设施的最大容量
                    openFacilityCost;           //所有设施的打开花费
    vector<vector<double> > allocateCostVec;    //顾客分配到不同工厂的花费，[i][j],i为工厂号，j为顾客号
    clock_t start_time;
public:
    void run(string path ){
        ifstream is(path);
        is >> faciltyNum >> customerNum;
        demandVec.resize(customerNum);
        allocateCostVec.resize(faciltyNum , vector<double>(customerNum));
        facilityCapacity.resize(faciltyNum);
        openFacilityCost.resize(faciltyNum);
        for(int i = 0 ; i < faciltyNum; i++){
            double openCost , capacity;
            is >> facilityCapacity[i] >> openFacilityCost[i];
        }
        for(int i = 0 ; i < customerNum; i++){
            is >> demandVec[i];
        }
        for(int i = 0 ; i < faciltyNum; i++){
            for(int j = 0 ; j < customerNum; j++){
                is >> allocateCostVec[i][j] ;
            }
        }
        start_time = clock();
        cout << "solving " << path << endl;
    }
    //输出结果
    void printResult(vector<int> result , ofstream& os , string path){

        vector<bool> openFac(faciltyNum , false);
        os << path << " result: " << endl;
        os << "customer arrange: ";
        for(int i = 0 ; i < customerNum ; i ++){
            os << result[i] << " ";
            openFac[result[i] ] = true;
        }
        os << endl;
        os << "facility : ";
        for(int i = 0 ; i < faciltyNum ; i ++){
            os << openFac[i]  << " ";
        }
        os << endl;
        os << "cost is " << evalutate(result) << endl;
        os << "this case waste time is " << (clock() - start_time) * 1.0 / CLOCKS_PER_SEC << " s" << endl;
        os << "===================================================================" << endl;
    }
    //给定所有顾客的分配序列，算出结果，O(n+m)复杂度
    double evalutate(vector<int > result){
        double value = 0;
        vector<double> facilityRestCapacity = facilityCapacity;
        vector<bool> open(faciltyNum , false);

        for(int i = 0 ; i < customerNum ; i++){
            if(result[i] == -1){
                return INT_MAX;
            }
            value += allocateCostVec[result[i] ][i];
            facilityRestCapacity[ result[i] ] -= demandVec[i];
            if(facilityRestCapacity[result[i] ] < 0 ){
                return INT_MAX;
            }
            open[result[i]] = true;
        }

        for(int i = 0 ; i < faciltyNum ; i++){
            if(open[i] ){
                value += openFacilityCost[i];
            }
        }
        return value;
    }
};

#endif // SOLUTION_H_INCLUDED
