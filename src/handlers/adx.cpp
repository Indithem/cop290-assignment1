#include "handlers.h"
using namespace std;

void Adx_Strategy_handler(int argc, char* argv[]){
    // args are n,x adx_threshold
    if (argc < 4){
        cerr<<"Not enough arguments for ADX strategy";
        return;
    }
    int n, x;
    double adx_threshold;
    try{
        n = stoi(argv[2]);
        x = stoi(argv[3]);
        adx_threshold = stod(argv[4]);
    } catch (exception& e){
        cerr<<"Arguments for ADX strategy must be integers";
        return;
    }
    Strategies::AdxStrategy strat(n,x,adx_threshold);

    util::CSV_reader historical_data("history.csv");
    vector<double> high_prices, low_prices;
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
        cerr << ("\nHeaders in csv file are not as expected");
        return;
    }
    

    for (int i =0; i<n; i++){
        auto line = historical_data.get_next_line();
        high_prices.push_back(stod(line[2]));
        low_prices.push_back(stod(line[3]));
    }
    strat.init_first_n_days(high_prices, low_prices);
    vector<Strategies::Action> actions;

    for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        double high = stod(line[2]);
        double low = stod(line[3]);
        double prev_close = stod(line[4]);
        actions.push_back(strat.get(high, low, prev_close));
    }

    write_to_csv_files_simple(actions, x, n);
}