#include "Transaction.h"
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>

using namespace std;

static mutex log_mutex;
static const int PIN_LENGTH = 4;

void Transaction::deposit(Account& acc, double amount, const string& pin) {
    if (!acc.authenticate(pin)) {
        cout << "Error: Authentication failed for Account " << acc.account_id << "\n";
        return;
    }
    lock_guard<mutex> lock(*acc.mtx);
    acc.balance += amount;
    cout << "Deposited " << amount << " to Account " << acc.account_id << ". New balance: " << acc.balance << "\n";
    log_transaction(acc.account_id, "deposit", amount);
}

void Transaction::withdraw(Account& acc, double amount, const string& pin) {
    if (!acc.authenticate(pin)) {
        cout << "Error: Authentication failed for Account " << acc.account_id << "\n";
        return;
    }
    lock_guard<mutex> lock(*acc.mtx);
    double available = acc.balance + acc.overdraft_limit;
    if (available >= amount) {
        acc.balance -= amount;
        cout << "Withdrew " << amount << " from Account " << acc.account_id << ". New balance: " << acc.balance << "\n";
        log_transaction(acc.account_id, "withdraw", amount);
    } else {
        cout << "Error: Insufficient funds for Account " << acc.account_id << "\n";
    }
}

void Transaction::transfer(Account& from, Account& to, double amount, const string& pin) {
    if (!from.authenticate(pin) || !to.is_active) {
        cout << "Error: Invalid account or authentication failed\n";
        return;
    }
    unique_lock<mutex> lock_from(*from.mtx, defer_lock);
    unique_lock<mutex> lock_to(*to.mtx, defer_lock);
    if (from.account_id < to.account_id) {
        lock_from.lock();
        lock_to.lock();
    } else {
        lock_to.lock();
        lock_from.lock();
    }
    if (from.balance + from.overdraft_limit >= amount) {
        from.balance -= amount;
        to.balance += amount;
        cout << "Transferred " << amount << " from Account " << from.account_id << " to " << to.account_id << "\n";
        log_transaction(from.account_id, "transfer_out", amount);
        log_transaction(to.account_id, "transfer_in", amount);
    } else {
        cout << "Error: Insufficient funds for transfer from Account " << from.account_id << "\n";
    }
}

void Transaction::request_loan(Account& acc, double amount, const string& pin) {
    if (!acc.authenticate(pin)) {
        cout << "Error: Authentication failed for Account " << acc.account_id << "\n";
        return;
    }
    lock_guard<mutex> lock(*acc.mtx);
    double max_loan = (acc.account_type == AccountType::BUSINESS) ? 5000.0 : 1000.0;
    if (amount > 0 && acc.loan_balance + amount <= max_loan) {
        acc.loan_balance += amount;
        acc.balance += amount;
        cout << "Loan of " << amount << " granted to Account " << acc.account_id
             << ". New balance: " << acc.balance << ", Loan balance: " << acc.loan_balance << "\n";
        log_transaction(acc.account_id, "loan", amount);
    } else {
        cout << "Error: Loan request denied for Account " << acc.account_id << "\n";
    }
}

void Transaction::pay_loan(Account& acc, double amount, const string& pin) {
    if (!acc.authenticate(pin)) {
        cout << "Error: Authentication failed for Account " << acc.account_id << "\n";
        return;
    }
    lock_guard<mutex> lock(*acc.mtx);
    if (acc.loan_balance >= amount && acc.balance >= amount) {
        acc.loan_balance -= amount;
        acc.balance -= amount;
        cout << "Paid " << amount << " towards loan for Account " << acc.account_id
             << ". Remaining loan: " << acc.loan_balance << "\n";
        log_transaction(acc.account_id, "loan_payment", amount);
    } else {
        cout << "Error: Insufficient funds or invalid loan payment for Account " << acc.account_id << "\n";
    }
}

void Transaction::apply_interest(Account& acc) {
    lock_guard<mutex> lock(*acc.mtx);
    if (acc.is_active && acc.balance > 0) {
        double interest = acc.balance * acc.interest_rate / 12;
        acc.balance += interest;
        cout << "Applied interest " << interest << " to Account " << acc.account_id
             << ". New balance: " << acc.balance << "\n";
        log_transaction(acc.account_id, "interest_earned", interest); // Renamed to clarify
    }
}

void Transaction::delete_account(Account& acc, const string& pin) {
    if (!acc.authenticate(pin)) {
        cout << "Error: Authentication failed for Account " << acc.account_id << "\n";
        return;
    }
    lock_guard<mutex> lock(*acc.mtx);
    if (acc.balance == 0.0 && acc.loan_balance == 0.0) {
        acc.is_active = false;
        cout << "Account " << acc.account_id << " deleted\n";
        log_transaction(acc.account_id, "delete", 0.0);
    } else {
        cout << "Error: Cannot delete Account " << acc.account_id << " with balance "
             << acc.balance << " or loan " << acc.loan_balance << "\n";
    }
}

void Transaction::generate_statement(const Account& acc, const string& pin, const vector<TransactionLog>& logs) {
    if (!acc.authenticate(pin)) {
        cout << "Error: Authentication failed for Account " << acc.account_id << "\n";
        return;
    }
    cout << "\nStatement for Account " << acc.account_id << " ("
         << (acc.account_type == AccountType::SAVINGS ? "Savings" :
             acc.account_type == AccountType::CHECKING ? "Checking" : "Business") << "):\n";
    cout << "Holder: " << acc.account_holder_name << " | Age: " << acc.age << "\n";
    cout << "Balance: " << acc.balance << " | Loan Balance: " << acc.loan_balance
         << " | Overdraft Limit: " << acc.overdraft_limit << "\n";
    cout << "Recent Transactions:\n";
    for (const auto& log : logs) {
        if (log.account_id == acc.account_id) {
            string time_str = ctime(&log.timestamp);
            time_str.pop_back();
            cout << log.type << ": " << log.amount << " at " << time_str << "\n";
        }
    }
}

vector<TransactionLog> Transaction::load_transaction_logs() {
    vector<TransactionLog> logs;
    ifstream log_file("transactions.log");
    if (log_file.is_open()) {
        string line;
        while (getline(log_file, line)) {
            TransactionLog log;
            stringstream ss(line);
            string timestamp_str;
            getline(ss, timestamp_str, '|');
            log.account_id = stoi(timestamp_str);
            getline(ss, log.type, '|');
            ss >> log.amount;
            getline(ss, timestamp_str, '|');
            getline(ss, timestamp_str);
            log.timestamp = time(nullptr); // Simplified for Windows
            logs.push_back(log);
        }
        log_file.close();
    }
    return logs;
}

void Transaction::log_transaction(int account_id, const string& type, double amount) {
    lock_guard<mutex> lock(log_mutex);
    ofstream log_file("transactions.log", ios::app);
    if (log_file.is_open()) {
        time_t now = time(nullptr);
        string time_str = ctime(&now);
        time_str.pop_back();
        log_file << account_id << " | " << type << " | " << amount << " | " << time_str << "\n";
        log_file.close();
    }
}
