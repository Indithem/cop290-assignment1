#include <vector>
#include "base.h"
#include <deque>    
#include <cmath>
using namespace std;
class RSIStrategy : public Strategy {
private:
    double oversold, overbought;
    std::vector<double> prices;
    deque<double> n_days;
public:
    RSIStrategy(int x,  int n, double oversold, double overbought) : Strategy(x,n),  oversold(oversold), overbought(overbought) {}

    ~RSIStrategy() {}
    void init_first_n_days(std::vector<double> days) {
        for (auto &&i : days) {
            n_days.push_back(i);
        }
    }

    Action get(double price) {

        n_days.pop_front();
        n_days.push_back(price);
 
        double avg_gain = 0, avg_loss = 0;

        for (int i = 1; i < n; i++) {
            double diff = n_days[i] - n_days[i - 1];
            if (diff > 0) {
                avg_gain += diff;
            } else {
                avg_loss -= diff;
            }
        }
        avg_gain /= n;
        avg_loss /= n;

        //  Relative Strength 
        double rs = avg_gain / avg_loss;

        //  RSI
        double rsi = 100.0 -((100.0)/ (1.0 + rs));

        if (rsi < oversold) {
            return BUY;
        } else if (rsi > overbought) {
            return SELL;
        }

        return HOLD;
    }
};
