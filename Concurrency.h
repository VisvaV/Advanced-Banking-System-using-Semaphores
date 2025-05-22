#ifndef CONCURRENCY_H
#define CONCURRENCY_H
#include "Account.h"
#include <vector>

using namespace std;

class Concurrency {
public:
    static void start_transactions(vector<Account>& accounts);
private:
    static void banking_operation(Account& acc1, Account& acc2, int thread_id);
};

#endif
