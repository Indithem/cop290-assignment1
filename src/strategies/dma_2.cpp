#include "lib.h"
#include <cmath>
using namespace std;

namespace Strategies{
    void DMA2Strategy::init_first_n_days(std::vector<double> days) {
        for (auto &&i : days) {
            n_days.push_back(i);
        }
    }

    Action DMA2Strategy::get(double price) {
        Action action = HOLD;

        n_days.pop_front();
        n_days.push_back(price);
        double ER = 0.0;
        double price_change = price - n_days.front();
        double sum_abs_price_change = 0.0;
        double prev_price = n_days.front();

        for (auto &&i : n_days) {
            sum_abs_price_change += abs(i - prev_price);
            prev_price = i;
        }

        if (sum_abs_price_change != 0.0) {
            ER = abs(price_change) / sum_abs_price_change;
        }

        if(count==0){
            SF=0.5;
            AMA=price;
            count++;
        }
        else{
            double num =(2 * ER) / (1 + c2) - 1;
            double den =(2 * ER) / (1 + c2) + 1;
            SF = SF + c1 * ((num/den) - SF);
            AMA = AMA + SF * (price - AMA);
        }

        if (price >= (1 + p / 100.0) * AMA) {
            action = BUY;
        } else if (price <= (1 - p / 100.0) * AMA) {
            action = SELL;
        }

        // Increment days held
        if (action != HOLD) {
            days_held++;
        }

        // Check for stop-loss
        if (days_held >= max_hold_days) {
            action = SELL;
            days_held = 0; // Reset days after selling
        }

        return action;
    }
};
