## main.cpp
* ~~Find a better way than using pointer to initialize different classes.~~ C++ simply works this way.
* ~~Add x into picture.~~
* Seperate logging and running of strategy. (maybe use vector\<string>?)

## Handlers
* Implement **Stop Loss**

## RevertPair Strategy
* Doesnt buy for some test cases.
make strategy=PAIRS symbol1=SBIN symbol2=ADANIENT x=5 n=20 threshold=2 start_date=01/01/2023 end_date=01/01/2024

## Makefile
* ~~update/depend on entire folder~~

## Utils
* make a contigous deque

## base.h
* ~~The only virtual function is run, so maybe we can use a function pointer instead of a class.~~ Class inheritance is the way in C++.