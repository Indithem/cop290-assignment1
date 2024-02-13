#include "handlers.h"
using namespace std;

void write_to_csv_files_simple(vector<Strategies::Action> actions, int x, int n){
    util::CSV_reader historical_data("history.csv");
    util::CSV_writer cashflow("daily_cashflow.csv");
    util::CSV_writer statistics("order_statistics.csv");
    double cash =0;
    int position = 0; 
    double price;
    string date;


    cashflow.write_line(vector<string>{"Date", "Cashflow"});
    statistics.write_line(vector<string>{"Date", "Order Direction","quantity","price"});
    historical_data.get_next_line()[0]; // headers
    for (int i =0; i<n; i++){
        historical_data.get_next_line()[0];
    } // skip first n lines

    size_t i = 0;
    for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        price = stod(line[1]);
        date = line[0];
        auto action = actions[i];
        switch (action)
        {
        case Strategies::BUY:
            if(position < x){
                statistics.write_line(vector<string>{date, "BUY","1",line[1]});
                cash -= price;
                position++;
            }
            break;
        
        case Strategies::SELL:
            if(position > -x){
                statistics.write_line(vector<string>{date, "SELL","1",line[1]});
                cash += price;
                position--;
            }
            break;

        case Strategies::HOLD:
            break;
        }
        cashflow.write_line(vector<string>{date, to_string(cash)});
        i++;
    }

    cash += position*price;

    util::CSV_writer final_cash("final_pnl.txt");
    final_cash.write_line(vector<string>{ to_string(cash)});
}

pair<double, vector<Strategies::Action>> run_simple_strategy(Strategies::Strategy* strat){
    util::CSV_reader historical_data("history.csv");
    vector<double> prices;
    auto headers = historical_data.get_next_line(); // headers
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
    int n = strat->n;
    int x = strat->x;
    for (int i =0; i<n; i++){
        auto line = historical_data.get_next_line();
        prices.push_back(stod(line[1]));
    }
    strat->init_first_n_days(prices);

    double cash =0;
    int position = 0; 
    double price;
    vector<Strategies::Action> return_vector;
    for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        price = stod(line[1]);
        Strategies::Action action =  strat->get(price);
        play_on_actions(action, cash, position, price, x);
        return_vector.push_back(action);
    }

    cash += position*price;
    return {cash, return_vector};
}

Strategies::Strategy* construct_simple_strategy(int argc, char* argv[], string strategy_str){
    if (strategy_str == "BASIC"){
        if (argc < 4){
            throw invalid_argument("Not enough arguments for BASIC strategy");
        }
        int n, x;
        try{
            n = stoi(get_argument(argc, argv, "n"));
            x = stoi(get_argument(argc, argv, "x"));
        } catch (exception& e){
            throw invalid_argument("Arguments for BASIC strategy must be integers");
        }
        return new Strategies::BasicStrategy{x=x,n=n};
    } else if (strategy_str == "DMA"){
        // args are n, x, p
        if (argc < 5){
            throw invalid_argument("Not enough arguments for DMA strategy");
        }
        int n, x, p;
        try{
            n = stoi(get_argument(argc, argv, "n"));
            x = stoi(get_argument(argc, argv, "x"));
            p = stoi(get_argument(argc, argv, "p"));
        } catch (exception& e){
            throw invalid_argument("Arguments for DMA strategy must be integers");
        }
        return new Strategies::DMAStrategy(x,n,p);
    } else if (strategy_str == "DMA++"){
        // args are n, x, p, max_hold_days ,c1, c2
        if (argc < 8){
            throw invalid_argument("Not enough arguments for DMA++ strategy");
        }
        int n, x, p, max_hold_days;
        double c1, c2;
        try{
            n = stoi(get_argument(argc, argv, "n"));
            x = stoi(get_argument(argc, argv, "x"));
            p = stoi(get_argument(argc, argv, "p"));
            max_hold_days = stoi(get_argument(argc, argv, "max_hold_days"));
            c1 = stod(get_argument(argc, argv, "c1"));
            c2 = stod(get_argument(argc, argv, "c2"));
        } catch (exception& e){
            throw invalid_argument("Arguments for DMA++ strategy must be integers");
        }
        return new Strategies::DMA2Strategy(n,x,p,max_hold_days,c1,c2);
    } else if (strategy_str == "MACD"){
        // args are x
        if (argc < 3){
            throw invalid_argument("Not enough arguments for MACD strategy");
        }
        int x;
        int n;
        try{
            x = stoi(get_argument(argc, argv, "x"));
        } catch (exception& e){
            throw invalid_argument("Arguments for MACD strategy must be integers");
        }
        return new Strategies::MacdStrategy(x);
    } else if (strategy_str == "RSI"){
        // args are x, n, oversold, overbought
        if (argc < 6){
            throw invalid_argument("Not enough arguments for RSI strategy");
        }
        int x, n;
        double oversold, overbought;
        try{
            x= stoi(get_argument(argc, argv, "x"));
            n= stoi(get_argument(argc, argv, "n"));
            oversold = stod(get_argument(argc, argv, "oversold_threshold"));
            overbought = stod(get_argument(argc, argv, "overbought_threshold"));
        } catch (exception& e){
            throw invalid_argument("Arguments for RSI strategy must be integers");
        }
        return new Strategies::RsiStrategy(x,n,oversold,overbought);
    }
    throw invalid_argument("Invalid strategy name");
}

void Simple_Strategy_handler(int argc, char* argv[]){
    Strategies::Strategy* strat;
    try{
        strat = construct_simple_strategy(argc, argv, string(argv[1]));
    } catch (exception& e){
        cout << e.what() << endl;
        return;
    }
    auto m = run_simple_strategy(strat);
    write_to_csv_files_simple(m.second, strat->x, strat->n);
    delete strat;
}