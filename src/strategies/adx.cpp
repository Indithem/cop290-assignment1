#include "lib.h"
#include <cmath>
#include <deque>
using namespace std;

namespace Strategies
{
    void AdxStrategy::init_first_n_days(std::vector<double> high_price, std::vector<double> low_price) {
        for (auto &&i : high_price) {
            n_high.push_back(i);
        }
        for (auto &&i : low_price) {
            n_low.push_back(i);
        }
    }

    Action AdxStrategy::get(double high, double low, double prev_close)
    {
        double dx=0.0;
        Action action = HOLD;
        if(count==0){

            prev_high = n_high.back();
            prev_low = n_low.back();
        }

        double temp= max(abs(high - prev_close), abs(low - prev_close));
        double truerange = max(high - low, temp);
        double dmplus = max(0.0,high - prev_high);
        double dmminus = max(0.0,prev_low - low);
       
        if(count==0){
            ATR = truerange;
            DIplus = dmplus/ATR;
            DIminus = dmminus/ATR;
        }
        else{
            ATR = (2.0/n+1)*(truerange-ATR) + ATR;
            DIplus = (2.0/n+1)*(dmplus-DIplus) + DIplus;
            DIminus = (2.0/n+1)*(dmminus-DIminus) + DIminus;
        }
        if(DIplus+DIminus==0){
            dx=0.0;
        }
        else{
            dx = 100 * ((DIplus - DIminus) / (DIplus + DIminus));
        }
        if(count==0){
            adx=dx;
        }
        else{
            adx = (2.0/n+1)*(dx-adx) + adx;
        }
        if(adx > adx_threshold){
                action = BUY;
        }
        else if(adx < adx_threshold){
                action = SELL;
        }

        count++;
        prev_high = high;
        prev_low = low;

        return action;

    }
};