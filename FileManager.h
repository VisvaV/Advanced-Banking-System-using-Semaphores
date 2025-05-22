#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include "Account.h"
#include <vector>

using namespace std;

class FileManager {
public:
    static void save_accounts(const vector<Account>& accounts);
    static vector<Account> load_accounts();
    static void backup_accounts(const vector<Account>& accounts);
    static vector<Account> restore_from_backup();
};

#endif
