#include "SignalHandler.h"
#include "FileManager.h"
#include "IPC.h"
#include <signal.h>
#include <iostream>

using namespace std;

vector<Account>* SignalHandler::accounts_ptr = nullptr;

void SignalHandler::shutdown(int signum) {
    cout << "Shutting down gracefully...\n";
    if (accounts_ptr) {
        FileManager::save_accounts(*accounts_ptr);
    }
    IPC::cleanup_shared_memory();
    exit(signum);
}

void SignalHandler::init(vector<Account>& accounts) {
    accounts_ptr = &accounts;
    signal(SIGINT, shutdown);
}
