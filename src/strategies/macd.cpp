#include <vector>
#include "base.h"


class macd_strategy: public strategy{
private:
    unsigned int x;

public:
   macd_strategy(unsigned int x): x(x){

   }
    ~macd_strategy(){}

    Action get(double price){
        
    }

    
};