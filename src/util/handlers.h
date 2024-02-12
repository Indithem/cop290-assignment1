#pragma once
#include <string>
#include <vector>
#include "../strategies/lib.h"
#include "csv_parser.h"

void Simple_Strategy_handler(int argc, char* argv[]);
void Pairs_Strategy_handler(int argc, char* argv[]);
void Best_of_all_Strategy_handler(int argc, char* argv[]);

void write_to_csv_files_simple(std::vector<Strategies::Action> actions, int x);

std::pair<double, std::vector<Strategies::Action>> run_simple_strategy(Strategies::Strategy* strat);
