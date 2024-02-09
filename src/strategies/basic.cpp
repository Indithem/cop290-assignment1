#include <vector>
#include "base.h"

class BasicStrategy: public Strategy{
private:
    double prev_price;
    bool first_day=true;
    int count=0;

    
public:
   BasicStrategy( int x,  int n): Strategy(x,n){}
    ~BasicStrategy(){}

    void init_first_n_days(::std::vector<double> days){
        for (int i=0;i<n;i++){
            get(days[i]);
        }
    }

    Action get(double price){

        if(first_day){
            prev_price=price;
            first_day=false;
        }
        else{
            if (price>prev_price){
                if (count>=0 && count<n-1){
                    count++;
                    prev_price=price;
                    return HOLD;
                }
                else if(count>=n-1){
                    prev_price=price;
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
                if (count<=0 && count > -(n-1)){
                    count--;
                    prev_price=price;
                    return HOLD;
                }
                else if(count<= -(n-1)){
                    prev_price=price;
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