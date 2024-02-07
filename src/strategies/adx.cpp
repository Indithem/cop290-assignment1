#include <vector>
#include "base.h"


class AdxStrategy: public Strategy{
private:
    unsigned int x,n;
    double adx ;
public:
   AdxStrategy(unsigned int x,unsigned int n , double adx): Strategy(x),n(n),adx(adx){

   }
    ~AdxStrategy(){}

    Action get(double price){
        
    }

    
};