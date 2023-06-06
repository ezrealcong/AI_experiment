#include"Header.h"
#include <iostream>
#include <cmath>
#include <random>
#include <vector>

#include"DE.h"
using namespace std;

class DECCFR : private DE
{

    //add a contribution value list for each group
    vector<int> contribution;

};