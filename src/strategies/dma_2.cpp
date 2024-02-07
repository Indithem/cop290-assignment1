#include <vector>
#include "base.h"


class dma_2_strategy: public strategy{
private:
    unsigned int x,n,days;
    // days= max hold days
    double p,c1,c2;
    // p = Percent change needed
public:
   dma_2_strategy( unsigned int x, double p,unsigned int n, unsigned int days,double c1,double c2): x(x),n(n),p(p),days(days),c1(c1),c2(c2){

   }
    ~dma_2_strategy(){}

    Action get(double price){
        
    }

    
};