#ifndef INTEREST_H
#define INTEREST_H
#include "Account.h"
#include <vector>

using namespace std;

class Interest {
public:
    static void start_interest_calculation(vector<Account>& accounts);
private:
    static void calculate_interest(vector<Account>& accounts);
};

#endif
