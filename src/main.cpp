#include <iostream>
#include <string>
#include "handlers/handlers.h"
using namespace std;


enum class Strategy_Types{
    SIMPLE,
    PAIRS,
    BEST_OF_ALL,
    ADX
};

Strategy_Types get_strategy_type(string strategy_str){
    if (strategy_str == "BASIC"){return Strategy_Types::SIMPLE;}
    else if (strategy_str == "DMA"){return Strategy_Types::SIMPLE;}
    else if (strategy_str == "DMA++"){return Strategy_Types::SIMPLE;}
    else if (strategy_str == "MACD"){return Strategy_Types::SIMPLE;}
    else if (strategy_str == "RSI"){return Strategy_Types::SIMPLE;}
    else if (strategy_str == "ADX"){return Strategy_Types::ADX;}
    else if (strategy_str == "LINEAR_REGRESSION"){return Strategy_Types::SIMPLE;}
    else if (strategy_str == "BEST_OF_ALL"){return Strategy_Types::BEST_OF_ALL;}
    else if (strategy_str == "PAIRS"){return Strategy_Types::PAIRS;}
    else {
        throw invalid_argument("Invalid strategy");
    }
}

int main(int argc, char* argv[]){
    if (argc < 2){
        throw invalid_argument("No strategy provided");
    }
    string strategy_str = argv[1];
    Strategy_Types strategy_type;
    try
    {
        strategy_type = get_strategy_type(strategy_str);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    switch (strategy_type)
    {
        case Strategy_Types::SIMPLE:
            Simple_Strategy_handler(argc, argv);
            break;
        case Strategy_Types::PAIRS:
            Pairs_Strategy_handler(argc, argv);
            break;
        case Strategy_Types::BEST_OF_ALL:
            Best_of_all_Strategy_handler(argc, argv);
            break;
        case Strategy_Types::ADX:
            Adx_Strategy_handler(argc, argv);
            break;
    }
    
}