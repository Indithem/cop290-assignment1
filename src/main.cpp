#include "strategies/basic.cpp"
#include <iostream>
#include <string>
#include "csv_parser.cpp"

Strategy* get_args(int argc, char* argv[]){
    using namespace std;

    if (argc < 2){
        throw invalid_argument("No strategy provided");
    }
    
    string strategy_str = argv[1];
    Strategy* strat;
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
        strat = new BasicStrategy(n, x);

    } 
    else if (false){} // Add more strategies here
    else {
        throw invalid_argument("Invalid strategy");
    } 
    
    return strat;
}

int main(int argc, char* argv[]){
    /*Arguments will be <STRATEGY> ...*/
    using namespace std;
    Strategy* strat;
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
        Action action =  strat->get(price);
        switch (action)
        {
        case BUY:
            statistics.write_line(vector<string>{date, "BUY","1",line[1]});
            cash += price;
            position++;
            break;
        
        case SELL:
            statistics.write_line(vector<string>{date, "SELL","1",line[1]});
            cash -= price;
            position--;
            break;

        case HOLD:
            break;
        }
        cashflow.write_line(vector<string>{date, to_string(cash)});
    }

    if (position > 0){
        statistics.write_line(vector<string>{date, "SELL",to_string(position),to_string(price)});
        cash += price*position;
    } else if (position < 0){
        statistics.write_line(vector<string>{date, "BUY",to_string(-position),to_string(price)});
        cash -= price*position;
    }
    cashflow.write_line(vector<string>{date, to_string(cash)});

    delete strat;
}