#include "Account.h"
#include "Transaction.h"
#include "Concurrency.h"
#include "FileManager.h"
#include "Interest.h"
#include "IPC.h"
#include "SignalHandler.h"

#include "Account.cpp"
#include "Transaction.cpp"
#include "Concurrency.cpp"
#include "FileManager.cpp"
#include "Interest.cpp"
#include "IPC.cpp"
#include "SignalHandler.cpp"

#include <iostream>
#include <vector>
#include <random>
#include <fstream>

using namespace std;

void initialize_sample_data(vector<Account>& accounts) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> bal_dist(100.0, 5000.0);
    uniform_int_distribution<> type_dist(0, 2);
    for (int i = 0; i < 50; ++i) {
        string name = "Holder" + to_string(i);
        int age = 19 + (i % 50);
        string pin = to_string(1000 + i % 9000);
        double initial_balance = bal_dist(gen);
        AccountType type = static_cast<AccountType>(type_dist(gen));
        accounts.emplace_back(i, name, age, initial_balance, pin, type);
        cout << "Created Account " << i << " ("
             << (type == AccountType::SAVINGS ? "Savings" : type == AccountType::CHECKING ? "Checking" : "Business")
             << ") with balance " << initial_balance << " and PIN " << pin << "\n";
    }
}

void admin_menu(const vector<Account>& accounts, const vector<TransactionLog>& logs) {
    string admin_password;
    cout << "Enter admin password: ";
    cin >> admin_password;
    if (admin_password != "admin123") {
        cout << "Incorrect password. Access denied.\n";
        return;
    }
    cout << "\nAdmin Menu:\n";
    cout << "1. View all accounts (accounts.dat)\n";
    cout << "2. View all transactions (transactions.log)\n";
    cout << "3. View backup data (backup.dat)\n";
    cout << "Enter choice: ";
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: {
            cout << "\nAll Accounts (accounts.dat):\n";
            for (const auto& acc : accounts) {
                if (acc.is_active) {
                    cout << "ID: " << acc.account_id << " | Name: " << acc.account_holder_name
                         << " | Age: " << acc.age << " | Type: "
                         << (acc.account_type == AccountType::SAVINGS ? "Savings" :
                             acc.account_type == AccountType::CHECKING ? "Checking" : "Business")
                         << " | Balance: " << acc.balance << " | PIN: " << acc.pin
                         << " | Loan: " << acc.loan_balance << "\n";
                }
            }
            break;
        }
        case 2: {
            cout << "\nAll Transactions (transactions.log):\n";
            for (const auto& log : logs) {
                string time_str = ctime(&log.timestamp);
                time_str.pop_back();
                cout << "Account " << log.account_id << " | " << log.type << " | "
                     << log.amount << " | " << time_str << "\n";
            }
            break;
        }
        case 3: {
            cout << "\nBackup Data (backup.dat):\n";
            ifstream file("backup.dat");
            if (file.is_open()) {
                time_t backup_time;
                file >> backup_time;
                string time_str = ctime(&backup_time);
                time_str.pop_back();
                cout << "Backup Time: " << time_str << "\n";
                int id, type, age;
                double balance, loan_balance;
                string name, pin;
                while (file >> id >> type) {
                    file.ignore();
                    getline(file, name, '"');
                    getline(file, name, '"');
                    file >> age >> balance >> pin >> loan_balance;
                    cout << "ID: " << id << " | Name: " << name << " | Age: " << age << " | Type: "
                         << (type == 0 ? "Savings" : type == 1 ? "Checking" : "Business")
                         << " | Balance: " << balance << " | PIN: " << pin << " | Loan: " << loan_balance << "\n";
                }
                file.close();
            } else {
                cout << "No backup available\n";
            }
            break;
        }
        default:
            cout << "Invalid choice\n";
    }
}

void main_menu(vector<Account>& accounts, vector<TransactionLog>& logs) {
    size_t choice, account_id, to_account;
    int account_type, age;
    double amount;
    string pin, name;
    do {
        cout << "\nBanking System Menu:\n";
        cout << "1. Create Account\n2. Deposit\n3. Withdraw\n4. Transfer\n5. Check Balance\n";
        cout << "6. Request Loan\n7. Pay Loan\n8. Generate Statement\n9. Delete Account\n";
        cout << "10. Backup System\n11. Restore from Backup\n12. Admin Menu\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Enter account holder name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter age: "; cin >> age;
                if (age < 18) {
                    cout << "Error: Account holder must be at least 18 years old.\n";
                    break;
                }
                cout << "Enter initial balance: "; cin >> amount;
                cout << "Enter PIN (4 digits): "; cin >> pin;
                cout << "Enter account type (0=Savings, 1=Checking, 2=Business): "; cin >> account_type;
                if (account_type >= 0 && account_type <= 2 && pin.length() == 4) {
                    int new_id = accounts.size();
                    accounts.emplace_back(new_id, name, age, amount, pin, static_cast<AccountType>(account_type));
                    cout << "Account created successfully. Your Account ID is: " << new_id << "\n";
                } else {
                    cout << "Invalid input\n";
                }
                break;
            case 2:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter deposit amount: "; cin >> amount;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size())
                    Transaction::deposit(accounts[account_id], amount, pin);
                break;
            case 3:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter withdrawal amount: "; cin >> amount;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size())
                    Transaction::withdraw(accounts[account_id], amount, pin);
                break;
            case 4:
                cout << "Enter your account ID: "; cin >> account_id;
                cout << "Enter recipient's account ID: "; cin >> to_account;
                cout << "Enter transfer amount: "; cin >> amount;
                cout << "Enter your PIN: "; cin >> pin;
                if (account_id < accounts.size() && to_account < accounts.size())
                    Transaction::transfer(accounts[account_id], accounts[to_account], amount, pin);
                break;
            case 5:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size()) {
                    double bal = accounts[account_id].get_balance();
                    cout << "Account balance: " << bal << "\n";
                }
                break;
            case 6:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter loan amount: "; cin >> amount;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size())
                    Transaction::request_loan(accounts[account_id], amount, pin);
                break;
            case 7:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter payment amount: "; cin >> amount;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size())
                    Transaction::pay_loan(accounts[account_id], amount, pin);
                break;
            case 8:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size())
                    Transaction::generate_statement(accounts[account_id], pin, logs);
                break;
            case 9:
                cout << "Enter account ID: "; cin >> account_id;
                cout << "Enter PIN: "; cin >> pin;
                if (account_id < accounts.size())
                    Transaction::delete_account(accounts[account_id], pin);
                break;
            case 10:
                FileManager::backup_accounts(accounts);
                break;
            case 11:
                accounts = FileManager::restore_from_backup();
                break;
            case 12:
                admin_menu(accounts, logs);
                break;
            case 0:
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 0);
}

int main() {
    vector<Account> accounts;
    vector<TransactionLog> logs;

    accounts = FileManager::load_accounts();
    cout << "Loaded " << accounts.size() << " accounts from accounts.dat\n";
    if (accounts.empty()) {
        initialize_sample_data(accounts);
        FileManager::save_accounts(accounts);
        cout << "Initialized and saved " << accounts.size() << " accounts\n";
    }
    logs = Transaction::load_transaction_logs();

    SignalHandler::init(accounts);
    if (!IPC::init_shared_memory(accounts)) {
        cout << "Continuing without shared memory...\n";
    }
    main_menu(accounts, logs);

    FileManager::save_accounts(accounts);
    cout << "Saved " << accounts.size() << " accounts to accounts.dat\n";
    IPC::cleanup_shared_memory();
    cout << "Banking System Shutdown\n";
    return 0;
}
