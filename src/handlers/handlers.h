#pragma once
#include <string>
#include <vector>
#include "../strategies/lib.h"
#include "../util/csv_parser.h"

void Simple_Strategy_handler(int argc, char* argv[]);
void Pairs_Strategy_handler(int argc, char* argv[]);
void Best_of_all_Strategy_handler(int argc, char* argv[]);
void Adx_Strategy_handler(int argc, char* argv[]);
void Linear_Regression_Strategy_handler(int argc, char* argv[]);

void play_on_actions(Strategies::Action a, double& cash, int& position, double price, int x);
void write_to_csv_files_simple(std::vector<Strategies::Action> actions, int x, int n);
std::string get_argument(std::vector<std::string>, std::string arg_name);

std::pair<double, std::vector<Strategies::Action>> run_simple_strategy(Strategies::Strategy* strat);
Strategies::Strategy* construct_simple_strategy(std::vector<std::string>, std::string strategy_str);

std::pair<double, std::vector<Strategies::Action>> run_adx_strategy(Strategies::AdxStrategy* strat);
Strategies::AdxStrategy* construct_adx_strategy(std::vector<std::string>);

std::pair<double, std::vector<Strategies::Action>> run_linear_regression_strategy(Strategies::LinearRegressionStrategy* strat, util::CSV_reader* historical_data);
Strategies::LinearRegressionStrategy* construct_linear_regression_strategy(std::vector<std::string>);
void train_linear_reg_model(Strategies::LinearRegressionStrategy* strat);

