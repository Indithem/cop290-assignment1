#include <vector>
#include "base.h"
#include <deque>
#include <cmath>
using namespace std;

class MACDStrategy : public Strategy {
private:
    deque<double> prices;
    double short_EWM;
    double long_EWM;
    double MACD;
    double signal_line;

public:
    MACDStrategy(int x) : Strategy(x), short_EWM(0), long_EWM(0), MACD(0), signal_line(0) {}

    ~MACDStrategy() {}

    void init_first_n_days(std::vector<double> days) {
        for (auto &&i : days) {
            prices.push_back(i);
        }
    }

    Action get(double price) {
        Action action = HOLD;

        // Update prices deque
        prices.pop_front();
        prices.push_back(price);

        // Calculate Short EWM
        short_EWM = (2.0 / 13) * (price - short_EWM) + short_EWM;

        // Calculate Long EWM
        if (prices.size() >= 26) {
            long_EWM = (2.0 / 27) * (price - long_EWM) + long_EWM;

        }
        // Calculate MACD
        MACD = short_EWM - long_EWM;

        // Calculate Signal Line
        if (prices.size() >= 9) {
            signal_line = (2.0 / 10) * (MACD - signal_line) + signal_line;

            // Generate trading signals
            if (MACD > signal_line) {
                action = BUY;
            } else if (MACD < signal_line) {
                action = SELL;
            }
        }

        return action;
    }
};
