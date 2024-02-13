#include "handlers.h"

using namespace std;
void Pairs_Strategy_handler(int argc, char* argv[]){
    Strategies::PairsStrategy* strat;
    if (argc < 4){
        cerr<<"Not enough arguments for Pairs strategy";
        return;
    }
    int n, x;
    double threshold;
    try{
        n = stoi(get_argument(argc, argv, "n"));
        x = stoi(get_argument(argc, argv, "x"));
        threshold = stod(get_argument(argc, argv, "threshold"));
    } catch (exception& e){
        cerr<<"Arguments for Pairs strategy must be integers";
        return;
    }
    strat = new Strategies::RevertingPairStrategy(x,n ,threshold);

    util::CSV_reader historical_data1("history.csv");
    util::CSV_reader historical_data2("history2.csv");
    auto headers1 = historical_data1.get_next_line();
    auto headers2 = historical_data2.get_next_line();
    if (
        headers1[0] != "DATE" ||
        headers1[1] != "CLOSE" ||
        headers2[0] != "DATE" ||
        headers2[1] != "CLOSE"
    ) {
        cerr << "Expected headers: DATE CLOSE\n"
        << "Got headers1: ";
        for (auto &&i : headers1)
        {
            cerr << i << " ";
        }
        cerr << "\nGot headers2: ";
        for (auto &&i : headers2)
        {
            cerr << i << " ";
        }
        cerr << ("\nHeaders in csv file are not as expected");
        throw "Headers in csv file are not as expected";
    }
    vector<double> prices1, prices2;

    for (int i =0; i<n; i++){
        auto line1 = historical_data1.get_next_line();
        auto line2 = historical_data2.get_next_line();
        prices1.push_back(stod(line1[1]));
        prices2.push_back(stod(line2[1]));
    }

    strat->init_first_n_days(prices1, prices2);
    vector<Strategies::Action> actions;
    util::CSV_writer orders1("order_statistics_1.csv");
    util::CSV_writer orders2("order_statistics_2.csv");
    util::CSV_writer cashflow("daily_cashflow.csv");

    string date;
    double cash = 0, price1, price2;
    int position1 = 0, position2 = 0;

    orders1.write_line(vector<string>{"DATE", "ACTION", "QUANTITY", "PRICE"});
    orders2.write_line(vector<string>{"DATE", "ACTION", "QUANTITY", "PRICE"});
    cashflow.write_line(vector<string>{"DATE", "CASH"});

    for (vector<string> line1 = historical_data1.get_next_line(), line2 = historical_data2.get_next_line(); line1.size() > 0; line1 = historical_data1.get_next_line(), line2 = historical_data2.get_next_line()){
        price1 = stod(line1[1]);
        price2 = stod(line2[1]);
        date = line1[0];
        auto action = strat->get(price1, price2);
        switch (action.first)
        {
        case Strategies::BUY:
            if(position1 < x){
                cash -= price1;
                position1++;
                orders1.write_line(vector<string>{date, "BUY","1",line1[1]});
            }
            break;
        
        case Strategies::SELL:
            if(position1 > -x){
                cash += price1;
                position1--;
                orders1.write_line(vector<string>{date, "SELL","1",line1[1]});
            }
            break;

        case Strategies::HOLD:
            break;
        }  
        switch (action.second)
        {
        case Strategies::BUY:
            if(position2 < x){
                cash -= price2;
                position2++;
                orders2.write_line(vector<string>{date, "BUY","1",line2[1]});
            }
            break;
        case Strategies::SELL:
            if(position2 > -x){
                cash += price2;
                position2--;
                orders2.write_line(vector<string>{date, "SELL","1",line2[1]});
            }
            break;
        case Strategies::HOLD:
            break;
        }

        cashflow.write_line(vector<string>{date, to_string(cash)});  
    }

    cash += position1*price1 + position2*price2;
    util::CSV_writer final_cash("final_pnl.txt");
    final_cash.write_line(vector<string>{ to_string(cash)});

    delete strat;
}