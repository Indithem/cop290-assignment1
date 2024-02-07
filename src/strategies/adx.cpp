#include <vector>
#include "base.h"


class adx_strategy: public strategy{
private:
    unsigned int x,n;
    double adx ;
public:
   adx_strategy(unsigned int x,unsigned int n , double adx): x(x),n(n),adx(adx){

   }
    ~adx_strategy(){}

    Action get(double price){
        
    }

    
};