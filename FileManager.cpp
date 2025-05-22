#include "FileManager.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

void FileManager::save_accounts(const vector<Account>& accounts) {
    ofstream file("accounts.dat");
    if (file.is_open()) {
        for (const auto& acc : accounts) {
            if (acc.is_active) {
                file << acc.account_id << " " << static_cast<int>(acc.account_type) << " \""
                     << acc.account_holder_name << "\" " << acc.age << " " << acc.balance << " "
                     << acc.pin << " " << acc.loan_balance << "\n";
            }
        }
        file.close();
        cout << "Accounts saved to accounts.dat\n";
    } else {
        cout << "Error: Could not open accounts.dat for writing\n";
    }
}

vector<Account> FileManager::load_accounts() {
    vector<Account> accounts;
    ifstream file("accounts.dat");
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int id, type, age;
            double balance, loan_balance;
            string name, pin;

            ss >> id >> type;
            ss.ignore(); // Skip space before name
            getline(ss, name, '"'); // Read up to first quote (might be empty)
            getline(ss, name, '"'); // Read the name between quotes
            ss >> age >> balance >> pin >> loan_balance;

            if (ss.good()) { // Ensure all fields were read successfully
                accounts.emplace_back(id, name, age, balance, pin, static_cast<AccountType>(type));
                accounts.back().loan_balance = loan_balance;
            }
        }
        file.close();
        cout << "Accounts loaded from accounts.dat\n";
    } else {
        cout << "No accounts.dat found or unable to open\n";
    }
    return accounts;
}

void FileManager::backup_accounts(const vector<Account>& accounts) {
    ofstream file("backup.dat");
    if (file.is_open()) {
        time_t now = time(nullptr);
        file << now << "\n";
        for (const auto& acc : accounts) {
            if (acc.is_active) {
                file << acc.account_id << " " << static_cast<int>(acc.account_type) << " \""
                     << acc.account_holder_name << "\" " << acc.age << " " << acc.balance << " "
                     << acc.pin << " " << acc.loan_balance << "\n";
            }
        }
        file.close();
        cout << "Backup created at " << ctime(&now);
    }
}

vector<Account> FileManager::restore_from_backup() {
    vector<Account> accounts;
    ifstream file("backup.dat");
    if (file.is_open()) {
        time_t backup_time;
        file >> backup_time;
        file.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip to next line
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            int id, type, age;
            double balance, loan_balance;
            string name, pin;

            ss >> id >> type;
            ss.ignore(); // Skip space before name
            getline(ss, name, '"'); // Read up to first quote (might be empty)
            getline(ss, name, '"'); // Read the name between quotes
            ss >> age >> balance >> pin >> loan_balance;

            if (ss.good()) {
                accounts.emplace_back(id, name, age, balance, pin, static_cast<AccountType>(type));
                accounts.back().loan_balance = loan_balance;
            }
        }
        file.close();
        cout << "Restored from backup at " << ctime(&backup_time);
    } else {
        cout << "No backup available\n";
    }
    return accounts;
}
