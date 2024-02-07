#include <vector>
#include "base.h"
#include <string>
using namespace std;

class regression_strategy: public strategy{
private:
    unsigned int x;
    double p ;
    string start,end;
    // x - The max position
    // • train_start_date, train_end_date
    // • p - The percent different required
public:
   regression_strategy(unsigned int x,double p , double regression): x(x),p(p),start(start),end(end){

   }
    ~regression_strategy(){}

    Action get(double price){
        
    }

    
};