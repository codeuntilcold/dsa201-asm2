// #include "D:\1_UNIVERSITY\HK201\4_DataStructure_n_Algorithms\ASM\ASM_2\BKUTree.h"
#include "D:\1_UNIVERSITY\HK201\4_DataStructure_n_Algorithms\ASM\ASM_2\BusParking.h"

int main()
{


    vector<Interval> intervals;
    intervals.push_back(Interval(1, 5));
    intervals.push_back(Interval(2, 5));
    intervals.push_back(Interval(3, 5));
    // intervals.add(Interval(1, 5));
    // intervals.add(Interval(2, 5));
    // intervals.add(Interval(3, 5));
    cout << minPark(intervals);

    // BusParking* b = new BusParking();
    // b->add(1, 5);
    // b->add(2, 5);
    // b->add(3, 5);
    // cout << b->minPark();

    // OUTPUT: 3
}

