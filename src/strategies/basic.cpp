#include <vector>
#include "base.h"
using namespace std;

class basic_strategy: public strategy{
private:
    unsigned int x,n;
    double prev_price;
    bool first_day=true;
    unsigned int count=0;
    
public:
   basic_strategy(unsigned int x, unsigned int n): x(x),n(n){}
    ~basic_strategy(){}

    Action get(double price){

        if(first_day){
            prev_price=price;
            first_day=false;
        }
        else{
            if (price>prev_price){
                if (count>=0 && count<x){
                    count++;
                    prev_price=price;
                    return HOLD;
                }
                else if(count>=x){
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
                if (count<=0 && count > -x){
                    count--;
                    prev_price=price;
                    return HOLD;
                }
                else if(count<= -x){
                    return SELL;
                }
                else {
                    count= -1;
                    prev_price=price;
                    return HOLD;
                }
            }
        }    
    }
};