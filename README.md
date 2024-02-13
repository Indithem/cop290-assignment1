# Trading Simulator and Analyzer

## What this does?
This is a simple trading simulator and analyzer. It uses the jugaad-data API to get the stock data and then uses the data to simulate trading. 

There are many strategies that are implemented in the `strategies` directory. You can also implement your own strategy and use it with this simulator.
Some of the strategies implemented and ready to use are:
* Monotonic increasing strategy **Basic strategy**
* n-day moving average strategy **DMA**
* DMA with a stop loss **DMA2**
* Moving average convg/divergence **MACD**
* Relative Strength Index **RSI**
* Average Directional Movement Index **ADX**
* Linear Regression **Linear**
* Best of all the above strategies **Best**
* Mean-Reverting Pair Trading **Pair**
* Pair strategy with a stop loss **Pair2**

## How to use?
### To use
* Clone this repository.
* Run `make strategy=<strategy> symbol=<symbol> n=<n> x=<x> ...` with the attributes required for the strategy.

### To clean
* Run `make clean`

## For Contributors
There are three sections in this project:
* Strategies
* Util
* Handlers

### Adding more Strategies
First identify if your strategy has similarities with the base strategies. If it does, then you can derive your strategy from the base strategy and implement the required functions. If not, only then you can implement a new base strategy.
* Define your strategy in the *strategies* directory.
    - Think of how your strategy would make use of the data that is already available and implement a `init_fisrt_n_days` function.
    - Think of how your strategy would work on one instance of the data and implement a `run` function.
* Add the required functions in the *handlers* directory.

#### Defining a new base strategy
Whenever defining a new base strategy, make sure to also complete the following:
* A new enum of the class `Strategy_Types`.
* A new handler for those kind of base strategies.
* Linking your definition .cpp files in the *lib.cpp* file.


## See also
* [Strategies](/src/strategies/README.md)
* [Util](/src/util/README.md)
* [Handlers](/src/handlers/README.md)
