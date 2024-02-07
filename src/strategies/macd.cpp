#include <vector>
#include "base.h"


class MACDStrategy: public Strategy{
private:
    unsigned int x;

public:
   MACDStrategy(unsigned int x): Strategy(x){

   }
    ~MACDStrategy(){}

    Action get(double price){
        
    }

    
};