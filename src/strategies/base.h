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
   

}