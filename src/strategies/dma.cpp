#include <vector>
#include "base.h"
#include <deque>
#include <cmath>
// #include <iostream>
using namespace std;
class DMAStrategy: public Strategy{
private:
    int p;
    deque<double> n_days;
    // p = standard deviation threshold
public:
   DMAStrategy( int n,  int x,  int p): Strategy(n,x),p(p){}
    ~DMAStrategy(){}
    void init_first_n_days(std::vector<double> days){

        for (auto &&i : days)
        {
            n_days.push_back(i);
        }    
    }

    Action get(double price){

        double avg_price = 0;
        double sum = 0;
        double sum_of_squares = 0;
        n_days.pop_front();
        n_days.push_back(price);
       
        for (auto &&i : n_days) {
            avg_price += i;
            sum += i;
            sum_of_squares += i * i;
        }


        avg_price /= n;

        // Calculate standard deviation
        double mean_of_squares = sum_of_squares / n;
        double variance = mean_of_squares - (sum / n) * (sum / n);
        double sd = sqrt(variance);

      

        if ((price - avg_price) >= p * sd) {
            return BUY;
        } else if ((avg_price - price) >= p * sd) {
            return SELL;
        }
        return HOLD;   
    } 
    
};