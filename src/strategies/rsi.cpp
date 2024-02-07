#include <vector>
#include "base.h"


class rsi_strategy: public strategy{
private:
    unsigned int x,n;
    double oversold , overbought;
public:
   rsi_strategy(unsigned int x,unsigned int n , double oversold,double overbought): x(x),n(n),oversold(oversold),overbought(overbought){

   }
    ~rsi_strategy(){}

    Action get(double price){
        
    }

    
};