#pragma once
#include "actions.h"

class Strategy{

public:
   unsigned int x;
   Strategy(unsigned int x): x(x){}
   virtual Action get(double price)=0;
};