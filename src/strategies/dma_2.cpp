#include <vector>
#include "base.h"
#include <deque>
#include <cmath>
using namespace std;

class DMA2Strategy : public Strategy {
private:
    int p;
    int max_hold_days;
    double c1, c2;
    deque<double> n_days;
    double SF, AMA;
    int days_held;

public:
    DMA2Strategy(int n, int x, int p, int max_hold_days, double c1, double c2) : Strategy(n, x), p(p), max_hold_days(max_hold_days), c1(c1), c2(c2), SF(0.5), AMA(0), days_held(0) {}

    ~DMA2Strategy() {}

    void init_first_n_days(std::vector<double> days) {
        for (auto &&i : days) {
            n_days.push_back(i);
        }
    }

    Action get(double price) {
        Action action = HOLD;

        n_days.pop_front();
        n_days.push_back(price);
        double ER = 0;
        double price_change = price - n_days.front();
        double sum_abs_price_change = 0;
        double prev_price = n_days.front();

        for (auto &&i : n_days) {
            sum_abs_price_change += abs(i - prev_price);
            prev_price = i;
        }

        if (sum_abs_price_change != 0) {
            ER = abs(price_change) / sum_abs_price_change;
        }
        double num =(2 * ER) / (1 + c2) - 1;
        double den =(2 * ER) / (1 + c2) + 1;
        SF = SF + c1 * ((num/den) - SF);

        AMA = AMA + SF * (price - AMA);

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
