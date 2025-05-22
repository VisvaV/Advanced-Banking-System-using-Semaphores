#ifndef TRANSACTION_H
#define TRANSACTION_H
#include "Account.h"
#include <string>
#include <vector>

using namespace std;

struct TransactionLog {
    int account_id;
    string type;
    double amount;
    time_t timestamp;
};

class Transaction {
public:
    static void deposit(Account& acc, double amount, const string& pin);
    static void withdraw(Account& acc, double amount, const string& pin);
    static void transfer(Account& from, Account& to, double amount, const string& pin);
    static void request_loan(Account& acc, double amount, const string& pin);
    static void pay_loan(Account& acc, double amount, const string& pin);
    static void apply_interest(Account& acc);
    static void delete_account(Account& acc, const string& pin);
    static void generate_statement(const Account& acc, const string& pin, const vector<TransactionLog>& logs);
    static vector<TransactionLog> load_transaction_logs();
    static void log_transaction(int account_id, const string& type, double amount);
};

#endif
