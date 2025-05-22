#include "IPC.h"
#include <iostream>

using namespace std;

HANDLE IPC::hMapFile = nullptr;
Account* IPC::shared_accounts = nullptr;

bool IPC::init_shared_memory(vector<Account>& accounts) {
    const int MAX_ACCOUNTS = 50;

    hMapFile = OpenFileMappingA(FILE_MAP_ALL_ACCESS, FALSE, "BankSharedMemory");
    if (hMapFile == nullptr) {
        hMapFile = CreateFileMappingA(INVALID_HANDLE_VALUE, nullptr, PAGE_READWRITE, 0,
                                      sizeof(Account) * MAX_ACCOUNTS, "BankSharedMemory");
        if (hMapFile == nullptr) {
            DWORD error = GetLastError();
            cerr << "Could not create file mapping object (" << error << ")\n";
            return false;
        }
    }

    shared_accounts = (Account*)MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0,
                                              sizeof(Account) * MAX_ACCOUNTS);
    if (shared_accounts == nullptr) {
        DWORD error = GetLastError();
        cerr << "Could not map view of file (" << error << ")\n";
        CloseHandle(hMapFile);
        hMapFile = nullptr;
        return false;
    }

    if (GetLastError() != ERROR_ALREADY_EXISTS) {
        for (size_t i = 0; i < accounts.size() && i < MAX_ACCOUNTS; ++i) {
            shared_accounts[i] = accounts[i];
        }
    }

    cout << "Shared memory initialized (Windows)\n";
    return true;
}

vector<Account> IPC::get_shared_accounts() {
    vector<Account> accounts;
    if (shared_accounts == nullptr) {
        cout << "Shared memory not initialized\n";
        return accounts;
    }
    for (int i = 0; i < 50; ++i) {
        if (shared_accounts[i].is_active) {
            accounts.push_back(shared_accounts[i]);
        }
    }
    return accounts;
}

void IPC::cleanup_shared_memory() {
    if (shared_accounts != nullptr) {
        UnmapViewOfFile(shared_accounts);
        shared_accounts = nullptr;
    }
    if (hMapFile != nullptr) {
        CloseHandle(hMapFile);
        hMapFile = nullptr;
        cout << "Shared memory cleaned up (Windows)\n";
    }
}
