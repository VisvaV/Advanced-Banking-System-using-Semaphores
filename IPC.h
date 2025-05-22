#ifndef IPC_H
#define IPC_H
#include "Account.h"
#include <vector>
#include <windows.h>

using namespace std;

class IPC {
public:
    static bool init_shared_memory(vector<Account>& accounts);
    static vector<Account> get_shared_accounts();
    static void cleanup_shared_memory();
private:
    static HANDLE hMapFile;
    static Account* shared_accounts;
};

#endif
