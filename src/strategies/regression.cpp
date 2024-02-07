#include <vector>
#include "base.h"
#include <string>
using namespace std;

class RegressionStrategy: public Strategy{
private:
    double p ;
    string start,end;
    // x - The max position
    // • train_start_date, train_end_date
    // • p - The percent different required
public:
   RegressionStrategy(unsigned int x,double p , double regression): Strategy(x),p(p),start(start),end(end){

   }
    ~RegressionStrategy(){}

    Action get(double price){
        
    }

    
};