#include <iostream>
#include <string>
#include "csv_parser.cpp"

// namespace Strategies
// {
    #include "strategies/basic.cpp"
    #include "strategies/dma.cpp"

//} // namespace Strategies
#define Strategies


Strategies::Strategy* get_args(int argc, char* argv[]){
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
        int n, x,p;
        try{
            n = stoi(argv[2]);
            x = stoi(argv[3]);
            p = stoi(argv[4]);
            
        } catch (exception& e){
            throw invalid_argument("Arguments for DMA strategy must be integers");
        }
        strat = new Strategies::DMAStrategy{x=x,n=n,p=p};

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
        strat = get_args(argc, argv);
    } catch (exception& e){
        cerr << e.what() << endl;
        return 1;
    }
    CSV_reader historical_data("history.csv");
    CSV_writer cashflow("daily_cashflow.csv");
    CSV_writer statistics("order_statistics.csv");
    double cash =0;
    int position = 0; 
    double price;
    string date;

    cashflow.write_line(vector<string>{"Date", "Cashflow"});
    statistics.write_line(vector<string>{"Date", "Order Direction","quantity","price"});
    historical_data.get_next_line(); // headers
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

        CSV_writer final_cash("final_pnl.txt");
        final_cash.write_line(vector<string>{ to_string(cash)});


    delete strat;
}