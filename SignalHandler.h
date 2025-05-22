#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H
#include "Account.h"
#include <vector>

using namespace std;

class SignalHandler {
public:
    static void init(vector<Account>& accounts);
private:
    static void shutdown(int signum);
    static vector<Account>* accounts_ptr;
};

#endif
