# Handlers
This directory contains the handlers for the managing the I/O of the project. The `main` function hands the control over to these handlers depending on the strategy that is called.
There is a handle for every base Strategy class.
An extra handler for Best strategy is also implemented.

There also some utility functions that are used to perform the I/O file operations.

## Simple Strategy Handler
This is the handler for all `Strategy` base class derived strategies. 
* `construct_simple_strategy` : This function is used to construct the strategy object and return it. This determines which strategy to use and constructs the object taking the required parameters from argv.
* `run_simple_strategy` : This function is used to run the strategy and write the output to the file. It takes the strategy object and produces a pair of final pnl and the vector of Actions that are taken by the strategy.
* `write_to_csv_files_simple` : This handles writing to the csv files. It takes the vector of Actions and writes it to the csv file. It simulates trading.

## Pairs Strategy Handler
This is the handler for the `PairsStrategy` base derived classes. The structure of this handler is similar to the *Simple Strategy Handler* except that it does it for a pair of stocks.

## Adx Strategy Handler
This is the handler for the `Adx_Strategy` base derived classes. The structure of this handler is similar to the *Simple Strategy Handler*. It inculcates high, low and previous close prices for the stock.

## Linear Regression Strategy Handler
This is the handler for the `LinearRegressionStrategy` base derived classes. 
* `train_linear_regression` : This function is used to train the linear regression model.

## Best Strategy Handler
This is the handler for the *Best Strategy*. It uses the Simple and Linear Strategy Handlers to run.
There is a helper `best_of_all_helpers` namespace defined. You can change the *constants* to change the weights of the strategies.
* `run_simple` : Runs the Basic derived Strategies.
* various `run_<type>` methods : All the helper functions for running the strategies.
We make use of the `OpenMP` library to parallelize the running of the strategies.