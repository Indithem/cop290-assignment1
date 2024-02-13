#include "handlers.h"
using namespace std;


std::pair<double, std::vector<Strategies::Action>> run_adx_strategy(Strategies::AdxStrategy* strat){
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
        cerr << ("\nHeaders in csv file are not as expected");
        throw "Headers in csv file are not as expected";
    }
    
    int n = strat->n;
    int x = strat->x;

    vector<double> high_prices, low_prices;
    for (int i =0; i<n; i++){
        auto line = historical_data.get_next_line();
        high_prices.push_back(stod(line[2]));
        low_prices.push_back(stod(line[3]));
    }
    strat->init_first_n_days(high_prices, low_prices);
    vector<Strategies::Action> actions;
    double cash = 0, price;
    int position = 0;
    for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        price = stod(line[1]);
        double high = stod(line[2]);
        double low = stod(line[3]);
        double prev_close = stod(line[4]);
        Strategies::Action action = strat->get(high, low, prev_close);
        actions.push_back(action);
        play_on_actions(action, cash, position, price, x);
    }

    cash += position*price;
    return make_pair(cash, actions);
}

Strategies::AdxStrategy* construct_adx_strategy(vector<string> argv){
    // args are n,x adx_threshold
    int argc = argv.size();
    if (argc < 4){
        throw ("Not enough arguments for ADX strategy");
    }
    int n, x;
    double adx_threshold;
    try{
        n = stoi(get_argument(argv, "n"));
        x = stoi(get_argument(argv, "x"));
    } catch (exception& e){
        throw("Arguments for ADX strategy must be integers");
    }
    return new Strategies::AdxStrategy (n,x,adx_threshold);
}

void Adx_Strategy_handler(int argc, char* argv[]){
    Strategies::AdxStrategy* strat;
    vector<string> args;
    for (int i=0; i<argc; i++){
        args.push_back(argv[i]);
    }
    try{
        strat = construct_adx_strategy(args);
    } catch (const char* e){
        cerr<<e;
        return;
    }

    int x = strat->x;
    int n = strat->n;
    vector<Strategies::Action> actions;
    try{
        actions = run_adx_strategy(strat).second;
    } catch (const char* e){
        cerr<<e;
        return;
    }

    delete strat;
    write_to_csv_files_simple(actions, x, n);
}