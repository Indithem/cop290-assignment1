#include "lib.h"
#include "../util/matrices.h"

namespace Strategies{
    using namespace std;
    void LinearRegressionStrategy::train_data(std::vector<std::vector<double>> x_matrix, std::vector<vector<double>> y_matrix){
        auto transpose_x = util::transpose_matrix(x_matrix);
        auto x_transpose_x = util::multiply_matrices(transpose_x, x_matrix);
        auto inv = util::inverse_matrix(x_transpose_x);
        auto x_transpose_y = util::multiply_matrices(transpose_x, y_matrix);
        auto ans = util::multiply_matrices(inv, x_transpose_y);

        b0 = ans[0][0];
        b1 = ans[1][0];
        b2 = ans[2][0];
        b3 = ans[3][0];
        b4 = ans[4][0];
        b5 = ans[5][0];
        b6 = ans[6][0];
        b7 = ans[7][0];

    };

    Action LinearRegressionStrategy::get(double price, double close, double open, double high, double low, double vwap, double no_of_trades){
        if (first_day){
            prev_close = close; prev_open = open; prev_high = high; prev_low = low; prev_vwap = vwap; prev_no_of_trades = no_of_trades;
            first_day = false;
            return Action::HOLD;
        }
        
        double predicted_price = b0 + b1*prev_close + b2*prev_open + b3*prev_vwap + b4*prev_low + b5*prev_high + b6*prev_no_of_trades + b7*open;
        prev_close = close; prev_open = open; prev_high = high; prev_low = low; prev_vwap = vwap; prev_no_of_trades = no_of_trades;
        if ((predicted_price - price)/price >= p){
            return Action::BUY;
        } else if ((price - predicted_price)/price >= p){
            return Action::SELL;
        } else {
            return Action::HOLD;
        }
    }
}