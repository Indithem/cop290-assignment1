#pragma once
#include <vector>

namespace Strategies{
#include "actions.h"

   class Strategy{

   public:
      int x,n;
      Strategy( int x, int n): x(x),n(n){}
      virtual Action get(double price)=0;
      virtual void init_first_n_days(std::vector<double>)=0;
      virtual ~Strategy(){}
   };

   class PairsStrategy{
   public:
      int x,n;
      PairsStrategy( int x, int n): x(x),n(n){}
      virtual std::pair<Action,Action> get(double price1,double price2)=0;
      virtual void init_first_n_days(std::vector<double>,std::vector<double>)=0;
   };

   class Adx_Strategy{
   public:
      int x,n;
      Adx_Strategy( int x, int n): x(x),n(n){}
      virtual Action get(double high,double low,double prev_close)=0;
      virtual void init_first_n_days(std::vector<double>,std::vector<double>)=0;
   };
   
   class LinearRegressionStrategy{
      double b0,b1,b2,b3,b4,b5,b6,b7;
      double prev_close=0, prev_open=0, prev_high=0, prev_low=0, prev_vwap=0, prev_no_of_trades=0;
      double p;
      bool first_day=true;
   public:
      int x,n;
      LinearRegressionStrategy( int x, double p): x(x), p(p/100){}
      Action get(double price, double close, double open, double high, double low, double vwap, double no_of_trades);
      // trains the linear regression model
      // needs a matrix of 
      void train_data(std::vector<std::vector<double>> x_matrix, std::vector<std::vector<double>> y_matrix);
   };

}