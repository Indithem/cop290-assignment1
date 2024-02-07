#include <vector>
#include <actions.h>


class Strategy{
private:
    unsigned int x,n;

public:
    Strategy(unsigned int x, unsigned int n);
    ~Strategy();
    Action get();
};