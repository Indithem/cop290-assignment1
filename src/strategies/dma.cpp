#include <vector>
#include "base.h"
#include <deque>
using namespace std;
class DMAStrategy: public Strategy{
private:
    int p;
    deque<double> n_days;
    // p = standard deviation threshold
public:
   DMAStrategy( int n,  int x,  int p): Strategy(x,n),p(p){}
    ~DMAStrategy(){}
    void init_first_n_days(std::vector<double> days){
        for (auto &&i : days)
        {
            n_days.push_back(i);
        }    
    }

    Action get(double price){
        double avg_price=0;

        for (auto &&i : n_days)
        {
            avg_price+=i;      
        }
        n_days.pop_front();
        n_days.push_back(price);

        if((price-avg_price)>=p){
            return BUY;
        }
        else if((avg_price-price)>=p){
            return SELL;
        }
        return HOLD;   
    }

    
};