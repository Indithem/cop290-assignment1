#include "lib.h"
#include <cmath>
using namespace std;
namespace Strategies{
     void StopLossPairStrategy::init_first_n_days(std::vector<double> prices_1 , std::vector<double> prices_2){

        for (auto &&i :prices_1)
        {
            n_days1.push_back(i);
        }    
        for (auto &&i :prices_2)
        {
            n_days2.push_back(i);
        }
    }

    pair <Action,Action> StopLossPairStrategy::get(double p1 , double p2){
    
        double spread = p1 - p2;
        double diff = 0;
        double rolling_mean = 0;
        double sum_of_squares_diff = 0;
        double variance = 0;
        double sd=0;
        double z_score=0;
        n_days1.pop_front();
        n_days1.push_back(p1);
        n_days2.pop_front();
        n_days2.push_back(p2);
        for (int i=0;i<n;i++) {
            diff+= (n_days1[i] - n_days2[i]);
            sum_of_squares_diff += (n_days1[i] - n_days2[i]) * (n_days1[i] - n_days2[i]);
        }
        rolling_mean = diff / n;
        variance = (sum_of_squares_diff / n) - (rolling_mean * rolling_mean);
        sd = sqrt(variance);
        z_score = (spread - rolling_mean) / sd;
        if (z_score > threshold){
            return make_pair(SELL,BUY);
        }
        else if (z_score < -threshold){
            return make_pair(BUY,SELL);
        }
        // if stop
        return make_pair(HOLD,HOLD);
    }

};