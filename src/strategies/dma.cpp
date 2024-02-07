#include <vector>
#include "base.h"


class dma_strategy: public strategy{
private:
    unsigned int x,n;
    double p;
    // p = standard deviation threshold
public:
   dma_strategy(unsigned int n, unsigned int x, double p): x(x),n(n),p(p){

   }
    ~dma_strategy(){}

    Action get(double price){
        
    }

    
};