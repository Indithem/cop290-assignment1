#include <vector>
#include "base.h"


class RSIStrategy: public Strategy{
private:
    unsigned int n;
    double oversold , overbought;
public:
   RSIStrategy(unsigned int x,unsigned int n , double oversold,double overbought): Strategy(x),n(n),oversold(oversold),overbought(overbought){

   }
    ~RSIStrategy(){}

    Action get(double price){
        
    }

    
};