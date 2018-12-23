#ifndef GREEDYSOLUTION_H_INCLUDED
#define GREEDYSOLUTION_H_INCLUDED

#include "Solution.h"

using namespace std;

class greedySolution:public Solution{

protected:
    double stopThresold = 1.05;
public:
    int run(string filePath ,ofstream& os ){
        Solution::run(filePath );
        vector<int> result = greedyArrange();
        double cost = evalutate( result );
        cout << "greedy arrange cost "<<  cost << endl;
        printResult(result , os, filePath);
        return cost;
    }


    vector<int> greedyArrange(){
        vector<bool> openFac(faciltyNum , true);
        vector<bool> bestOpenFac = openFac;
        vector<int> initResult = simpleGreedyArrange(openFac );
        double nowValue = evalutate(initResult);
        double bestValue = nowValue;

        while(true){
            int bestReduceFacility = -1;
            double minCost = INT_MAX;
            for(int i = 0 ; i < faciltyNum ; i++){
                if(openFac[i]){
                    openFac[i] = false;
                    vector<int> tmpResult = simpleGreedyArrange(openFac );
                    double tmpCost = evalutate(tmpResult);
                    if(tmpCost < minCost){
                        minCost = tmpCost;
                        bestReduceFacility = i;
                    }
                    openFac[i] = true;
                }
            }
            if(minCost < bestValue * stopThresold ){
                openFac[bestReduceFacility] = false;
                if(minCost < bestValue){
                    bestOpenFac = openFac;
                    bestValue = minCost;
                }
            }else{
                break;
            }
        }
        return simpleGreedyArrange(bestOpenFac);
    }

    vector<int> simpleGreedyArrange(vector<bool> openFac ){
        vector<double> facilityRestCapacity = facilityCapacity;
        vector<int > result(customerNum , -1);
        double nowGreedyCost = 0;
        for(int i = 0 ; i < customerNum ; i++){
            int bestFacility = -1 , minAllocCost = INT_MAX;
            for(int j = 0 ; j < faciltyNum ; j++){
                if(openFac[j] && facilityRestCapacity[j] >= demandVec[i]
                        && minAllocCost > allocateCostVec[j][i] ){
                    bestFacility = j ;
                    minAllocCost = allocateCostVec[j][i];
                }
            }
            if(bestFacility != -1){
                facilityRestCapacity[bestFacility] -= demandVec[i] ;
                result[i] = bestFacility;
            }else{
                //cout << "greedyArrange Error" << endl;
                return result ;
            }
        }
        return result;
     }

};

#endif // GREEDYSOLUTION_H_INCLUDED
