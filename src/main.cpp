#include <iostream>
#include <string>
#include <vector>
#include "util/csv_parser.h"
#include "strategies/lib.h"
using namespace std;

void feed_init_days(Strategies::Strategy* s, util::CSV_reader& historical_data){
    vector<double> prices;
    for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        prices.push_back(stod(line[1]));
        if (prices.size() == s->n){
            break;
        }
    }
    s->init_first_n_days(prices);
}

Strategies::PairsStrategy* get_args1(int argc, char* argv[]){
    using namespace std;

    if (argc < 2){
        throw invalid_argument("No strategy provided");
    }
    string strategy_str = argv[1];
    Strategies::PairsStrategy* strat1;
    if (strategy_str == "PAIRS"){
        // The arguments should be n x
        if (argc < 7){
            throw invalid_argument("Not enough arguments for DMA strategy");
        }
        int n, x;
        double threshold,stop_loss_threshold;
        try{
            x = stoi(argv[3]);
            n = stoi(argv[4]);
            threshold = stod(argv[5]);
            stop_loss_threshold = stod(argv[6]);
  
        } catch (exception& e){
            throw invalid_argument("Arguments for PAIR strategy must be valid integers and doubles");
        }
        strat1 = new Strategies::StopLossPairStrategy{x=x,n=n,threshold=threshold,stop_loss_threshold=stop_loss_threshold};

    } 
    else if (strategy_str == "PAIRS"){
        // The arguments should be n x
        if (argc < 6){
            throw invalid_argument("Not enough arguments for revert_pairs strategy");
        }
        int n, x;
        double threshold;
        try{
            x = stoi(argv[3]);
            n = stoi(argv[4]);
            threshold = stod(argv[5]);
        } catch (exception& e){
            throw invalid_argument("Arguments for revert_pair strategy must be valid integers and doubles");
        }
        strat1 = new Strategies::RevertingPairStrategy{x=x,n=n,threshold=threshold};

    } 
    else if (false){} // Add more strategies here\

    else {
        throw invalid_argument("Invalid strategy");
    }
    return strat1;
}
Strategies:: Adx_Strategy* get_args2(int argc, char* argv[]){
    using namespace std;

    if (argc < 2){
        throw invalid_argument("No strategy provided");
    }
    string strategy_str = argv[1];
    Strategies::Adx_Strategy* strat2;
    if (strategy_str == "ADX"){
        // The arguments should be n x
        if (argc < 5){
            throw invalid_argument("Not enough arguments for ADX strategy");
        }
        int n, x;
        double adx_threshold;
        try{
            x = stoi(argv[2]);
            n = stoi(argv[3]);
            adx_threshold = stod(argv[4]);
        } catch (exception& e){
            throw invalid_argument("Arguments for ADX strategy must be integers and doubles");
        }
        strat2 = new Strategies::AdxStrategy{x=x,n=n,adx_threshold=adx_threshold};

    } 
    else if (false){} // Add more strategies here\

    else {
        throw invalid_argument("Invalid strategy");
    }
    return strat2;
}

Strategies::Strategy* get_args3(int argc, char* argv[]){
    /*Arguments will be <STRATEGY> ...*/
    using namespace std;

    if (argc < 2){
        throw invalid_argument("No strategy provided");
    }
    string strategy_str = argv[1];
    Strategies::Strategy* strat;
    if (strategy_str == "BASIC"){
        // The arguments should be n x
        if (argc < 4){
            throw invalid_argument("Not enough arguments for BASIC strategy");
        }
        int n, x;
        try{
            n = stoi(argv[2]);
            x = stoi(argv[3]);
        } catch (exception& e){
            throw invalid_argument("Arguments for BASIC strategy must be integers");
        }
        strat = new Strategies::BasicStrategy{x=x,n=n};

    } 
    else if (strategy_str == "DMA"){
        // The arguments should be n x
        if (argc < 5){
            throw invalid_argument("Not enough arguments for DMA strategy");
        }
        int n, x, p;
        try{
            n = stoi(argv[2]);
            x = stoi(argv[3]);
            p = stoi(argv[4]);
            
        } catch (exception& e){
            throw invalid_argument("Arguments for DMA strategy must be integers");
        }
        strat = new Strategies::DMAStrategy{x=x,n=n,p=p};

    } 
    else if (strategy_str == "DMA2"){
        // The arguments should be n x
        if (argc < 8){
            throw invalid_argument("Not enough arguments for DMA2 strategy");
        }
        int n, x, p, max_hold_days;
        double c1, c2;
        try{
            x = stoi(argv[2]);
            p = stoi(argv[3]);
            n = stoi(argv[4]);
            max_hold_days = stoi(argv[5]);
            c1 = stod(argv[6]);
            c2 = stod(argv[7]);
        } catch (exception& e){
            throw invalid_argument("Arguments for DMA2 strategy must be integers and doubles");
        }
        strat = new Strategies::DMA2Strategy{x=x,n=n,p=p,max_hold_days=max_hold_days,c1=c1,c2=c2};

    } 
    else if (strategy_str == "RSI"){
        if (argc < 6){
            throw invalid_argument("Not enough arguments for RSI strategy");
        }
        int n, x;
        double oversold, overbought;
        try{
            x = stoi(argv[2]);
            n = stoi(argv[3]);
            oversold = stod(argv[4]);
            overbought = stod(argv[5]);
        } catch (exception& e){
            throw invalid_argument("Arguments for RSI strategy must be integers and doubles");
        }
        strat = new Strategies::RsiStrategy{x=x,n=n,oversold=oversold,overbought=overbought};

    } 
    else if (strategy_str == "MACD"){
        // The arguments should be n x
        if (argc < 3){
            throw invalid_argument("Not enough arguments for MACD strategy");
        }
        int x;
        try{
            x = stoi(argv[2]);
        } catch (exception& e){
            throw invalid_argument("Arguments for MACD strategy must be integers");
        }
        strat = new Strategies::MacdStrategy{x=x};

    } 

  
    else if (false){} // Add more strategies here\

    else {
        throw invalid_argument("Invalid strategy");
    } 
    
    return strat;
}

int main(int argc, char* argv[]){
    using namespace std;
    Strategies::Strategy* strat;
    try{
        strat = get_args3(argc, argv);
    } catch (exception& e){
        cerr << e.what() << endl;
        return 1;
    }
    util::CSV_reader historical_data("history.csv");
    util::CSV_writer cashflow("daily_cashflow.csv");
    util::CSV_writer statistics("order_statistics.csv");
    double cash =0;
    int position = 0; 
    double price;
    string date;

    cashflow.write_line(vector<string>{"Date", "Cashflow"});
    statistics.write_line(vector<string>{"Date", "Order Direction","quantity","price"});
    historical_data.get_next_line(); // headers

    feed_init_days(strat, historical_data);

    for (vector<string> line = historical_data.get_next_line(); line.size() > 0; line = historical_data.get_next_line()){
        price = stod(line[1]);
        date = line[0];
        Strategies::Action action =  strat->get(price);
        switch (action)
        {
        case Strategies::BUY:
            if(position < strat->x){
                statistics.write_line(vector<string>{date, "BUY","1",line[1]});
                cash -= price;
                position++;
            }
            break;
        
        case Strategies::SELL:
            if(position > -strat->x){
                statistics.write_line(vector<string>{date, "SELL","1",line[1]});
                cash += price;
                position--;
            }
            break;

        case Strategies::HOLD:
            break;
        }
        cashflow.write_line(vector<string>{date, to_string(cash)});
    }

    cash += position*price;

        util::CSV_writer final_cash("final_pnl.txt");
        final_cash.write_line(vector<string>{ to_string(cash)});


    delete strat;
}