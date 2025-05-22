#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>
#include <mutex>
#include <memory>

using namespace std;

enum class AccountType { SAVINGS, CHECKING, BUSINESS };

class Account {
public:
    int account_id;
    string account_holder_name; // Added account holder name
    int age;                    // Added age
    double balance;
    bool is_active;
    string pin;
    double interest_rate;
    double overdraft_limit;
    double loan_balance;
    AccountType account_type;
    unique_ptr<mutex> mtx;

    Account(int id, const string& name, int age, double initial_balance, const string& pin, AccountType type);
    Account(const Account& other);
    Account(Account&&) noexcept;
    Account& operator=(const Account& other);
    Account& operator=(Account&&) noexcept;
    ~Account() = default;

    bool authenticate(const string& pin) const;
    double get_balance();
};

#endif
