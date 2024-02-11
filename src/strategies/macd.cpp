#include "lib.h"
#include <cmath>
using namespace std;

namespace Strategies {
    void MacdStrategy::init_first_n_days(std::vector<double> days) {
        for (auto &&i : days) {
            prices.push_back(i);
        }
    }

    Action MacdStrategy::get(double price) {
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
