#pragma once
#include <vector>

namespace Strategies{
#include "actions.h"

class Strategy{

public:
   int x,n;
   Strategy( int x, int n): x(x),n(n){}
   virtual Action get(double price)=0;
   virtual void init_first_n_days(std::vector<double>)=0;
};

}