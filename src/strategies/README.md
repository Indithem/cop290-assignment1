# Strategies
This is the module/library regarding the strategies.
This folder contains all files related to the strategies that are to be implemented in the assignment. This entire module/library is defined under the namespace `Strategies`.

## Base Classes
There are two Base classes that can be inherited by the strategies. 
* `Strategy` abstract class is the base class for those strategies which depend on a single stock
* `PairsStrategy` abstract class is the base class for those strategies which depend on a pair of stocks.
* `Adx_Strategy` class is a specific class made for the strategies that require high and low values of the stock in addition.


### Strategy
Following from the assignment, x, and n are taken as int.
This contains the following two pure virtual functions.
#### void init_first_n_days(vector<double>)
Before the simulation starts, we need to process the previous n days of data. This function is called to do that.
#### Action get(double)
This function is called to get the action to be taken on the current day.

### PairsStrategy
Follows the same structure as the `Strategy` class. The only difference is that the `init_first_n_days` function takes in a two vector of doubles and `get` function takes in two doubles as input and outputs a pair . This is because the pairs strategy depends on two stocks.

### Action
This is an enum to signal whether to buy, sell, or hold the stock for a given day.


# Folder Structure
The structure of the folder is as follows:

## base.h
This file should contain all the base classes that are to be inherited by the strategies.


## lib.h
This is the header of all the classes that are present in this folder. Think of this as rust's `mod.rs` file.

## lib.cpp
This bundles all the cpp files. This file exists because g++ cannot create object files from multiple source files.

## cpp files
These files provide an implementation for a particular strategy.