#include <vector>
#include "base.h"


class DMA2Strategy: public Strategy{
private:
    unsigned int x,n,days;
    // days= max hold days
    double p,c1,c2;
    // p = Percent change needed
public:
   DMA2Strategy( unsigned int x, double p,unsigned int n, unsigned int days,double c1,double c2): Strategy(x),n(n),p(p),days(days),c1(c1),c2(c2){

   }
    ~DMA2Strategy(){}

    Action get(double price){
        
    }

    
};