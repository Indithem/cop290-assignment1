#pragma once
#include "base.h"
#include <vector>
#include <deque>

namespace Strategies{
    class BasicStrategy: public Strategy{
    private:
        double prev_price;
        bool first_day=true;
        int count=0;
    public:
        BasicStrategy( int x,  int n): Strategy(x,n){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class AdxStrategy: public Strategy{
    private:
        unsigned int x,n;
        double adx ;
    public:
        AdxStrategy(unsigned int x,unsigned int n , double adx): Strategy(x,n),n(n),adx(adx){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class DMAStrategy : public Strategy {
    private:
        int p;
        std::deque<double> n_days;
    public:
        DMAStrategy( int n,  int x,  int p): Strategy(n,x),p(p){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class DMA2Strategy : public Strategy {
    private:
        int p;
        int max_hold_days;
        double c1, c2;
        std::deque<double> n_days;
        double SF, AMA;
        int days_held;
        
    public:
        DMA2Strategy(int n, int x, int p, int max_hold_days, double c1, double c2) : Strategy(n, x), p(p){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class RsiStrategy : public Strategy {
    private:
        unsigned int n;
        double oversold , overbought;
        std::vector<double> prices;
        std::deque<double> n_days;
    public:
        RsiStrategy(unsigned int x,unsigned int n , double oversold,double overbought): Strategy(x,n),oversold(oversold),overbought(overbought){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class MacdStrategy : public Strategy {
    private:
        std::deque<double> prices;
        double short_EWM;
        double long_EWM;
        double MACD;
        double signal_line;
    public:
        MacdStrategy(int x) : Strategy(x,n), short_EWM(0), long_EWM(0), MACD(0), signal_line(0) {}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class RevertingPairStrategy: public PairsStrategy{
    private:
        double threshold;
        std::deque<double> n_days1;
        std::deque<double> n_days2;
    public:
        RevertingPairStrategy(int x, int n, double threshold): PairsStrategy(x,n),threshold(threshold){}
        void init_first_n_days(std::vector<double> prices_1 , std::vector<double> prices_2);
        std::pair <Action,Action> get(double p1 , double p2);
    };

    class StopLossPairStrategy: public PairsStrategy{
    private:
        double threshold, stop_loss_threshold;
        std::deque<double> n_days1;
        std::deque<double> n_days2;
    public:
        StopLossPairStrategy(int x, int n, double threshold, double stop_loss_threshold): PairsStrategy(x,n),threshold(threshold),stop_loss_threshold(stop_loss_threshold){}
        void init_first_n_days(std::vector<double> prices_1 , std::vector<double> prices_2);
        std::pair <Action,Action> get(double p1 , double p2);
    };

}
