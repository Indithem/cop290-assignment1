#include "simple.cpp"
#include "pairs.cpp"
#include "best.cpp"
#include "adx.cpp"
#include "linear.cpp"

// assumes arguments are given as arg_name=value 
string get_argument(vector<string> argv, string arg_name){
    for (string& arg : argv){
        auto match = mismatch(arg_name.begin(), arg_name.end(), arg.begin());
        if (match.first == arg_name.end()){
            return arg.substr(arg.find("=")+1);
        }
    }
    return "";
}

void play_on_actions(Strategies::Action action, double& cash, int& position, double price, int x){
    switch (action)
    {
    case Strategies::BUY:
        if(position < x){
            cash -= price;
            position++;
        }
        break;
    
    case Strategies::SELL:
        if(position > x){
            cash += price;
            position--;
        }
        break;

    case Strategies::HOLD:
        break;
    }
}