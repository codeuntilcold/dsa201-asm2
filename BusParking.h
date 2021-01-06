#ifndef _BUS_PARK_H_
#define _BUS_PARK_H_

#include "D:\1_UNIVERSITY\HK201\4_DataStructure_n_Algorithms\ASM\ASM_2\BKUTree.h"

/*
 *      REQUIREMENT 1:
 *          Implement a function that find the minimum
 *          parking slot required
 *          Complexity:     O(N logN)
 */

class Interval 
{
public:
    int start;
    int end;
    
    Interval(int start = 0, int end = 0) {
        this->start = start;
        this->end = end;
    }
};

int minPark(vector<Interval> lInterval) {

}

/*
 *      REQUIREMENT 2:
 *          Implement a class that stores intervals
 *          (recommend using balanced tree)
 *          add() and remove():     O(logN)
 *          minPark():              O(1)
 */

class BusParking {
public:
    void add(int s, int t) {
        
    }
    
    void remove(int s, int t) {
        
    }
    
    int minPark() {
        return 3;
    }
};

#endif