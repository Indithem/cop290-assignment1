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

    class AdxStrategy: public Adx_Strategy{
    private:
        double adx ;
        double ATR;
        double DIplus;
        double DIminus;
        double adx_threshold;
        int count;
        double prev_high;
        double prev_low;
        std::deque<double> n_high;
        std::deque<double> n_low;
    public:
        AdxStrategy( int x, int n , double adx_threshold): Adx_Strategy(x,n),adx_threshold(adx_threshold),count(0),prev_high(0.0),prev_low(0.0){}
        void init_first_n_days(std::vector<double> high_price, std::vector<double> low_price);
        Action get(double high, double low, double prev_close);
    };

    class DMAStrategy : public Strategy {
    private:
        int p;
        std::deque<double> n_days;
    public:
        DMAStrategy(int x, int n, int p): Strategy(x,n),p(p){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class DMA2Strategy : public Strategy {
    private:
        int p;
        int max_hold_days;
        double c1, c2;
        int count;
        std::deque<double> n_days;
        double SF, AMA;
        int days_held;
        
    public:
        DMA2Strategy(int n, int x, int p, int max_hold_days, double c1, double c2) : Strategy(n, x), p(p),max_hold_days(max_hold_days),c1(c1),c2(c2),count(0){}
        void init_first_n_days(std::vector<double> days);
        Action get(double price);
    };

    class RsiStrategy : public Strategy {
    private:
        double oversold , overbought;
        std::vector<double> prices;
        std::deque<double> n_days;
    public:
        RsiStrategy( int x,int n , double oversold,double overbought): Strategy(x,n),oversold(oversold),overbought(overbought){}
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
        int count;
    public:
        MacdStrategy(int x) : Strategy(x,0), short_EWM(0.0), long_EWM(0.0), MACD(0.0), signal_line(0.0),count(0) {}
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
