#ifndef SASOLUTION_H_INCLUDED
#define SASOLUTION_H_INCLUDED

#include "greedySolution.h"
#include <stdlib.h>

using namespace std;

class SaSolution:public greedySolution{
private:
    double temperature = 0.02;
    const double alpha = 0.99;
    int outLimit = 1000 , inLimit = 2000;
public:
    int run(string filePath , ofstream& os ){
        Solution::run(filePath );
        //vector<int> saResult = startSa(startGreedy() );//注释掉下一句，取消本句的注释结果会更好。
        vector<int> saResult = startSa(simpleGreedyArrange(vector<bool>(faciltyNum , true) ) );
        double cost = evalutate( saResult );
        cout << "sa arrange cost "<<  cost << endl;
        printResult(saResult , os , filePath);
        return cost;
    }

    vector<int> startSa(vector<int> result ){
        vector<int> bestResult = result;
        double cost = evalutate( result );
        double bestCost = cost;
        for(int i = 0 ; i < outLimit ; i++){
            int lastOutLimitCost = cost;
            for(int j = 0 ; j < inLimit ; j++){
                double randPick = (double)rand() / RAND_MAX;
                if(randPick < 0.4){
                    cost = twoSwap(result , cost);
                }else if(randPick < 0.8) {
                    cost = simleAllocChange(result , cost);
                }else{
                    cost = greedyArrangeRandCustomer(result , cost);
                }

                if(cost < bestCost){
                    bestCost = cost;
                    bestResult = result;
                }
            }

            if( (double)rand() / RAND_MAX < 0.5){
                cost = closeRandFac(result , cost);
            }else{
                cost = openRandFac(result , cost);
            }

            if(cost < bestCost){
                bestCost = cost;
                bestResult = result;
            }

            if(lastOutLimitCost == cost){
                temperature /= pow(alpha , 5);
            }else{
                temperature *= alpha;
            }
        }

        return bestResult;
    }
    //双交换
    double twoSwap(vector<int>& result , double lastCost){
        int randCutomer1 = rand() % customerNum;
        int randCutomer2 = rand() % customerNum;

        swapTwoAlloc(result , randCutomer1 , randCutomer2);
        double nowCost = evalutate(result);
        if( canAccept(lastCost , nowCost) ){
            return nowCost;
        }else{
            swapTwoAlloc(result , randCutomer1 , randCutomer2);
            return lastCost;
        }

    }
    //三交换
    double threeSwap(vector<int>& result , double lastCost){
        int randCutomer1 = rand() % customerNum;
        int randCutomer2 = rand() % customerNum;
        int randCutomer3 = rand() % customerNum;

        swapTwoAlloc(result , randCutomer1 , randCutomer2);
        swapTwoAlloc(result , randCutomer1 , randCutomer3);
        double nowCost = evalutate(result);
        if( canAccept(lastCost , nowCost) ){
            return nowCost;
        }else{
            swapTwoAlloc(result , randCutomer1 , randCutomer3);
            swapTwoAlloc(result , randCutomer1 , randCutomer2);
            return lastCost;
        }
    }
    //选随机顾客，随机选一个设施处理该顾客请求
    double simleAllocChange(vector<int>& result , double lastCost){
        int randCutomer = rand() % customerNum;
        int newFacility = rand() % faciltyNum;
        while(newFacility == result[randCutomer] ){
            newFacility = rand() % faciltyNum;
        }
        int oldFacility = result[randCutomer];
        result[randCutomer] = newFacility;

        double nowCost = evalutate(result);
        if( canAccept(lastCost , nowCost) ){
            return nowCost;
        }else{
            result[randCutomer] = oldFacility;
            return lastCost;
        }
    }
    //随机关掉一个设施
    double closeRandFac(vector<int>& result , double lastCost){
        vector<bool> openFac(faciltyNum , false);
        vector<double> facilityRestCapacity = facilityCapacity;
        for(int i = 0 ; i < customerNum ; i++){
            openFac[result[i] ] = true;
            facilityRestCapacity[result[i] ] -= demandVec[i];
        }
        int randFacility = rand() % faciltyNum;
        while(!openFac[randFacility]  ){
            randFacility = rand() % faciltyNum;
        }
        openFac[randFacility] = false;
        vector<int> oldResult = result;
        for(int i = 0 ; i < customerNum ; i++){
            if(result[i] == randFacility ){
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
                    result = oldResult;
                    return lastCost;
                }
            }
        }
        //result = simpleGreedyArrange(openFac);
        double nowCost = evalutate(result);

        if( canAccept(lastCost , nowCost) ){
            return nowCost;
        }else{
            result = oldResult;
            return lastCost;
        }
    }
    //随机开一个设施
    double openRandFac(vector<int>& result , double lastCost){
        vector<bool> openFac(faciltyNum , false);
        bool haveCloseFac = false;
        for(int i = 0 ; i < customerNum ; i++){
            openFac[result[i] ] = true;
        }

        for(int i = 0 ; i < faciltyNum ; i++){
            if(!openFac[i] ) {
                haveCloseFac = true;
            }
        }

        if(!haveCloseFac){
            return lastCost;
        }

        int randFacility = rand() % faciltyNum;
        while(openFac[randFacility]  ){
            randFacility = rand() % faciltyNum;
        }
        openFac[randFacility] = true;
        vector<int> oldResult = result;

        double restCapacity = facilityCapacity[randFacility];
        for(int i = 0 ; i < customerNum ; i++){
            int bestFacility = -1 ;
            if( restCapacity >= demandVec[i]
                    && allocateCostVec[result[i]][i] > allocateCostVec[randFacility][i] ){
                restCapacity -= demandVec[i];
                result[i] = randFacility;
            }
        }
        double nowCost = evalutate(result);
        if( canAccept(lastCost , nowCost) ){
            return nowCost;
        }else{
            result = oldResult;
            return lastCost;
        }
    }
    //选一个顾客，贪心分配它的设施（与原设施不同）
    double greedyArrangeRandCustomer(vector<int>& result , double lastCost){

        int randCutomer = rand() % customerNum , bestFacility = -1;
        int oldFacility = result[randCutomer];
        int oldFacilityCustomer = 0;
        vector<double> facilityRestCapacity = facilityCapacity;
        vector<bool> openFac(faciltyNum , false);
        for(int i = 0 ; i < customerNum ; i++){
            if(i != randCutomer){
                facilityRestCapacity[result[i] ] -= demandVec[i];
            }
            if(result[i] == oldFacility){
                oldFacilityCustomer ++ ;
            }
            openFac[result[i] ] = true;
        }

        double nowArrangeCost = lastCost - allocateCostVec[oldFacility][randCutomer] ;

        if(oldFacilityCustomer == 1){
            nowArrangeCost -= openFacilityCost[oldFacility];
        }

        double bestCost = INT_MAX;

        for(int j = 0 ; j < faciltyNum ; j++){
            if(j == oldFacility){
                continue;
            }
            if( openFac[j] && facilityRestCapacity[j] >= demandVec[randCutomer]
                    && nowArrangeCost + allocateCostVec[j][randCutomer] <= bestCost ){
                bestCost = nowArrangeCost + allocateCostVec[j][randCutomer] ;
                bestFacility = j;
            }
        }

        if(bestFacility != -1 ){
            if(canAccept(lastCost , bestCost) ){
                result[randCutomer] = bestFacility;
                return bestCost;
            }else{
                return lastCost;
            }
        }else{
            return lastCost;
        }
    }


    void swapTwoAlloc(vector<int>& result , int customer1 , int customer2 ){
        int tmpAlocFac = result[customer1];
        result[customer1] = result[customer2];
        result[customer2] = tmpAlocFac;
    }

    //根据新旧两个值，判断是否退火接受
    bool canAccept(double oldValue , double newValue){
        if(oldValue > newValue){
            return true;
        }else{
            double energy = (newValue - oldValue) / oldValue;
            double pickPossibility =  exp(- energy / temperature) ;
            if( (double)rand() / RAND_MAX < pickPossibility ){
                return true;
            }else{
                return false;
            }
        }
    }
};

#endif // SASOLUTION_H_INCLUDED
