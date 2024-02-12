#include "simple.cpp"
#include "pairs.cpp"
#include "best.cpp"
#include "adx.cpp"
#include "linear.cpp"

// assumes arguments are given as arg_name=value 
string get_argument(int argc, char* argv[], string arg_name){
    for (int i = 0; i < argc; i++){
        string arg = argv[i];
        if (arg.find(arg_name) != string::npos){
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