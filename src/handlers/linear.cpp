#include "handlers.h"
using namespace std;

std::pair<double, std::vector<Strategies::Action>> run_linear_regression_strategy(Strategies::LinearRegressionStrategy* strat){
    util::CSV_reader train_data("history2.csv");
    auto headers = train_data.get_next_line(); // headers
    if (
        headers[0] != "DATE" ||
        headers[1] != "CLOSE" ||
        headers[2] != "HIGH" ||
        headers[3] != "LOW" ||
        headers[5] != "OPEN" ||
        headers[6] != "VWAP" ||
        headers[7] != "NO OF TRADES"
    ){
        cerr << "Expected headers: DATE CLOSE HIGH LOW * OPEN VWAP NO_OF_TRADES\n"
        << "Got headers: ";
        for (auto &&i : headers)
        {
            cerr << i << " ";
        }
        throw "\nHeaders in csv file are not as expected";
    }

    vector<vector<double>> x_matrix, y_matrix;
    double prev_close, prev_open, prev_high, prev_low, prev_vwap, prev_no_of_trades;
    auto line = train_data.get_next_line();
    {
        prev_close = stod(line[1]);
        prev_open = stod(line[5]);
        prev_high = stod(line[2]);
        prev_low = stod(line[3]);
        prev_vwap = stod(line[6]);
        prev_no_of_trades = stod(line[7]);
    }
    for (line = train_data.get_next_line(); line.size() > 0; line = train_data.get_next_line()){
        vector<double> x_row = {
            1,
            prev_close,
            prev_open,
            prev_vwap,
            prev_low,
            prev_high,
            prev_no_of_trades
        };        
        
            prev_close = stod(line[1]);
            prev_open = stod(line[5]);
            prev_high = stod(line[2]);
            prev_low = stod(line[3]);
            prev_vwap = stod(line[6]);
            prev_no_of_trades = stod(line[7]);
        
        x_row.push_back(prev_open);
        x_matrix.push_back(x_row);
        y_matrix.push_back({prev_close});
    }

    strat->train_data(x_matrix, y_matrix);

    util::CSV_reader historical_data("history.csv");
    historical_data.get_next_line(); // headers
    vector<Strategies::Action> actions;
    double cash = 0, price;
    int position = 0;
    int x = strat->x;
    for (line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        price = stod(line[1]);
        double high = stod(line[2]);
        double low = stod(line[3]);
        double open = stod(line[5]);
        double vwap = stod(line[6]);
        double no_of_trades = stod(line[7]);
        Strategies::Action action = strat->get(price, price, open, high, low, vwap, no_of_trades);
        actions.push_back(action);
        play_on_actions(action, cash, position, price, x);
    }

    cash += position*price;
    return make_pair(cash, actions);
}


Strategies::LinearRegressionStrategy* construct_linear_regression_strategy(int argc, char* argv[]){
    // args are x p 
    if (argc < 3){
        throw "Not enough arguments for Linear Regression strategy";
    }
    int x;
    double p;
    try{
        x = stoi(get_argument(argc, argv, "x"));
        p = stod(get_argument(argc, argv, "p"));
    } catch (exception& e){
        throw "Arguments for Linear Regression strategy must be integers";
    }
    return new Strategies::LinearRegressionStrategy (x,p);
}

void Linear_Regression_Strategy_handler(int argc, char* argv[]){
    
    Strategies::LinearRegressionStrategy* strat;
    try{
        strat = construct_linear_regression_strategy(argc, argv);
    } catch (exception& e){
        cerr<<e.what();
        return;
    } 

    vector<Strategies::Action> actions;
    try{
        actions = run_linear_regression_strategy(strat).second;
    } catch (exception& e){
        cerr<<e.what();
        return;
    }
    
    write_to_csv_files_simple(actions, strat->x, 0);
    delete strat;
}