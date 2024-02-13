#include "handlers.h"
#include <omp.h>
using namespace std;

namespace best_of_all_helpers{
    const int glob_x = 5;
    const int total_n = 50;
    
    const int basic_n = 7;
    const int dma_n = 50;
    const int dma2_n = 14;
    const int adx_n = 14;
    const int rsi_n = 14;

    const int adx_threshold = 25;
    const int oversold_threshold = 30;
    const int overbought_threshold = 70;
    const int max_hold_days = 28;
    const double c1= 2;
    const double c2= 0.2;

    const double dma_p = 2;
    const double dma2_p = 5;

    const double linear_p = 2;

    pair<double, vector<Strategies::Action>> run_simple(Strategies::Strategy* strat, util::CSV_reader* historical_data){
        vector<double> prices;
        for (vector<string> line = historical_data->get_next_line(); line.size() > 0; line = historical_data->get_next_line()){
            prices.push_back(stod(line[1]));
        }
        strat->init_first_n_days(prices);

        vector<Strategies::Action> return_vector;
        double cash =0;
        int position = 0;
        double price;

        for (vector<string> line = historical_data->get_next_line(); line.size() > 0; line = historical_data->get_next_line()){
            price = stod(line[1]);
            auto action = strat->get(price);
            play_on_actions(action, cash, position, price, glob_x);
            return_vector.push_back(action);
        }

        cash += position*price;
        return make_pair(cash, return_vector);
    }

    void simple_strategy_skipper(util::CSV_reader* historical_data, int n){
        auto headers = historical_data->get_next_line(); // headers
        if (
            headers[0] != "DATE" ||
            headers[1] != "CLOSE" 
        ){
            cerr << "Expected headers: DATE CLOSE\n"
            << "Got headers: ";
            for (auto &&i : headers)
            {
                cerr << i << " ";
            }
            cerr << ("\nHeaders in csv file are not as expected");
            exit(1);
        }

        int skip_n = total_n-n;
        for (int i =0; i<skip_n; i++){
            historical_data->get_next_line()[0];
        } // skips
    }

    pair<double, vector<Strategies::Action>> run_basic(){
        auto strat = Strategies::BasicStrategy(glob_x,basic_n);
        util::CSV_reader historical_data("history.csv");
        simple_strategy_skipper(&historical_data, basic_n);
        return run_simple(&strat, &historical_data);
    }

    pair<double, vector<Strategies::Action>> run_dma(){
        auto strat = Strategies::DMAStrategy(glob_x,dma_n, dma_p);
        util::CSV_reader historical_data("history.csv");
        simple_strategy_skipper(&historical_data, dma_n);
        return run_simple(&strat, &historical_data);
    }

    pair<double, vector<Strategies::Action>> run_dma2(){
        auto strat = Strategies::DMA2Strategy(dma2_n, glob_x, dma2_p, max_hold_days, c1, c2);
        util::CSV_reader historical_data("history.csv");
        simple_strategy_skipper(&historical_data, dma2_n);
        return run_simple(&strat, &historical_data);
    }

    pair<double, vector<Strategies::Action>> run_rsi(){
        auto strat = Strategies::RsiStrategy(glob_x, rsi_n, oversold_threshold, overbought_threshold);
        util::CSV_reader historical_data("history.csv");
        simple_strategy_skipper(&historical_data, rsi_n);
        return run_simple(&strat, &historical_data);
    }

    pair<double, vector<Strategies::Action>> run_adx(){
        auto strat = Strategies::AdxStrategy(glob_x, adx_n, adx_threshold);
        util::CSV_reader historical_data("history.csv");
        auto headers = historical_data.get_next_line(); // headers
        if (
            headers[0] != "DATE" ||
            headers[1] != "CLOSE" ||
            headers[2] != "HIGH" ||
            headers[3] != "LOW" ||
            headers[4] != "PREV. CLOSE"
        ){
            cerr << "Expected headers: DATE CLOSE HIGH LOW PREV. CLOSE\n"
            << "Got headers: ";
            for (auto &&i : headers)
            {
                cerr << i << " ";
            }
            cerr << ("\nHeaders in csv file are not as expected")
            << "Headers in csv file are not as expected";

            exit(1);
        }

        int skip_n = total_n-adx_n;
        for (int i =0; i<skip_n; i++){
            historical_data.get_next_line()[0];
        } // skips

        vector<double> high_prices, low_prices;
        for (int i =0; i<adx_n; i++){
            auto line = historical_data.get_next_line();
            high_prices.push_back(stod(line[2]));
            low_prices.push_back(stod(line[3]));
        }
        strat.init_first_n_days(high_prices, low_prices);

        vector<Strategies::Action> actions;
        double cash = 0, price;
        int position = 0;
        for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
            price = stod(line[1]);
            double high = stod(line[2]);
            double low = stod(line[3]);
            double prev_close = stod(line[4]);
            Strategies::Action action = strat.get(high, low, prev_close);
            actions.push_back(action);
            play_on_actions(action, cash, position, price, glob_x);
        }

        cash += position*price;
        return make_pair(cash, actions);
    }

    pair<double, vector<Strategies::Action>> run_linear(){
        Strategies::LinearRegressionStrategy strat = Strategies::LinearRegressionStrategy(glob_x, linear_p);
        train_linear_reg_model(&strat);
        util::CSV_reader historical_data("history.csv");
        auto headers = historical_data.get_next_line(); // headers
        for (int i=0; i< total_n; i++){
            historical_data.get_next_line();
        } // skips
        return run_linear_regression_strategy(&strat, &historical_data);
    }

}

void Best_of_all_Strategy_handler(int argc, char* argv[]){
    
    pair<double, vector<Strategies::Action>> basic, dma, dma2, rsi, adx, linear;

     #pragma omp parallel sections
    {
        #pragma omp section
        basic = best_of_all_helpers::run_basic();

        #pragma omp section
        dma = best_of_all_helpers::run_dma();

        #pragma omp section
        dma2 = best_of_all_helpers::run_dma2();

        #pragma omp section
        rsi = best_of_all_helpers::run_rsi();

        #pragma omp section
        adx = best_of_all_helpers::run_adx();

        #pragma omp section
        linear = best_of_all_helpers::run_linear();
    }

    // gets maxiumun of those pairs based on the first value, which is a double
    pair<double, vector<Strategies::Action>> best = basic;
    int counter = 0, best_index = 0;
    for(auto &&i : {dma, dma2, rsi, adx, linear}){
        counter++;
        if (i.first > best.first){
            best = i;basic;
            best_index = counter;
        }
    }

    const char* names[] = {"Basic", "DMA", "DMA2", "RSI", "ADX", "Linear"};

    cout << "Best strategy is: "<< names[best_index] << " with a total of: " << best.first << " cash" << endl;

    write_to_csv_files_simple(best.second, best_of_all_helpers::glob_x, best_of_all_helpers::total_n);

}