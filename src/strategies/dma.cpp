#include <vector>
#include "base.h"


class DMAStrategy: public Strategy{
private:
    unsigned int x,n;
    double p;
    // p = standard deviation threshold
public:
   DMAStrategy(unsigned int n, unsigned int x, double p): Strategy(x),n(n),p(p){

   }
    ~DMAStrategy(){}

    Action get(double price){
        
    }

    
};