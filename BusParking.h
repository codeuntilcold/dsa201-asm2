#ifndef _BUS_PARK_H_
#define _BUS_PARK_H_

#include "D:\1_UNIVERSITY\HK201\4_DataStructure_n_Algorithms\ASM\ASM_2\BKUTree.h"
#include <algorithm>

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

int minPark(vector<Interval> lInterval) 
{
    // Split into arrival time and departure time
    // Then sort the two (sorting using Quicksort - NlogN)
    // Then record the highest parking slot required
    vector<int> ls, lt;
    for (Interval i : lInterval) {
        ls.push_back(i.start);
        lt.push_back(i.end);
    }
    sort(ls.begin(), ls.end());
    sort(lt.begin(), lt.end());

    int maxOcp = 0;
    int curOcp = 0;
    vector<int>::iterator is, it;
    is = ls.begin();
    it = lt.begin();
    for (int time = ls.front(); time < lt.back(); time++) {
        while (*is == time) {
            curOcp++;
            is++;
        }
        while (*it == time) {
            curOcp--;
            it++;
        }
        maxOcp = (maxOcp > curOcp) ? maxOcp : curOcp;
    }
    return maxOcp;
}

/*
 *      REQUIREMENT 2:
 *          Implement a class that stores intervals
 *          (recommend using balanced tree)
 *          add() and remove():     O(logN)
 *          minPark():              O(1)
 */

class BusParking {
protected:
    class Interval;
    BKUTree<int, int>::AVLTree hallo;

public:
    void add(int s, int t) 
    {
        
    }
    
    void remove(int s, int t) 
    {
        
    }
    
    int minPark() 
    {
        return 3;
    }


protected:
    class Interval {
    public:
        int start;
        int end;
        
        Interval(int start = 0, int end = 0) {
            this->start = start;
            this->end = end;
        }
    };
};

#endif