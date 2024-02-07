#include <vector>
#include "base.h"

class BasicStrategy: public Strategy{
private:
    unsigned int n;
    double prev_price;
    bool first_day=true;
    unsigned int count=0;
    
public:
   BasicStrategy(unsigned int x, unsigned int n): Strategy(x),n(n){}
    ~BasicStrategy(){}

    Action get(double price){

        if(first_day){
            prev_price=price;
            first_day=false;
        }
        else{
            if (price>prev_price){
                if (count>=0 && count<n){
                    count++;
                    prev_price=price;
                    return HOLD;
                }
                else if(count>=n){
                    return BUY;
                }
                else {
                    count=1;
                    prev_price=price;
                    return HOLD;
                }
            }
            else if(price==prev_price){
                count=0;
                return HOLD;
            }
            else{
                if (count<=0 && count > -n){
                    count--;
                    prev_price=price;
                    return HOLD;
                }
                else if(count<= -n){
                    return SELL;
                }
                else {
                    count= -1;
                    prev_price=price;
                    return HOLD;
                }
            }
        }    
        return HOLD;
    }
};