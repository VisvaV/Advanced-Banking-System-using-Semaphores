#include "Account.h"

using namespace std;

Account::Account(int id, const string& name, int age, double initial_balance, const string& pin, AccountType type)
    : account_id(id), account_holder_name(name), age(age), balance(initial_balance), is_active(true), pin(pin),
      loan_balance(0.0), account_type(type), mtx(make_unique<mutex>()) {
    switch (type) {
        case AccountType::SAVINGS:
            interest_rate = 0.05;
            overdraft_limit = 0.0;
            break;
        case AccountType::CHECKING:
            interest_rate = 0.01;
            overdraft_limit = 200.0;
            break;
        case AccountType::BUSINESS:
            interest_rate = 0.03;
            overdraft_limit = 1000.0;
            break;
    }
}

Account::Account(const Account& other)
    : account_id(other.account_id), account_holder_name(other.account_holder_name), age(other.age),
      balance(other.balance), is_active(other.is_active), pin(other.pin), interest_rate(other.interest_rate),
      overdraft_limit(other.overdraft_limit), loan_balance(other.loan_balance), account_type(other.account_type),
      mtx(make_unique<mutex>()) {
}

Account::Account(Account&& other) noexcept
    : account_id(other.account_id), account_holder_name(move(other.account_holder_name)), age(other.age),
      balance(other.balance), is_active(other.is_active), pin(move(other.pin)), interest_rate(other.interest_rate),
      overdraft_limit(other.overdraft_limit), loan_balance(other.loan_balance), account_type(other.account_type),
      mtx(move(other.mtx)) {
    other.account_id = -1;
    other.balance = 0.0;
    other.is_active = false;
    other.loan_balance = 0.0;
}

Account& Account::operator=(const Account& other) {
    if (this != &other) {
        account_id = other.account_id;
        account_holder_name = other.account_holder_name;
        age = other.age;
        balance = other.balance;
        is_active = other.is_active;
        pin = other.pin;
        interest_rate = other.interest_rate;
        overdraft_limit = other.overdraft_limit;
        loan_balance = other.loan_balance;
        account_type = other.account_type;
        mtx = make_unique<mutex>();
    }
    return *this;
}

Account& Account::operator=(Account&& other) noexcept {
    if (this != &other) {
        account_id = other.account_id;
        account_holder_name = move(other.account_holder_name);
        age = other.age;
        balance = other.balance;
        is_active = other.is_active;
        pin = move(other.pin);
        interest_rate = other.interest_rate;
        overdraft_limit = other.overdraft_limit;
        loan_balance = other.loan_balance;
        account_type = other.account_type;
        mtx = move(other.mtx);

        other.account_id = -1;
        other.balance = 0.0;
        other.is_active = false;
        other.loan_balance = 0.0;
    }
    return *this;
}

bool Account::authenticate(const string& pin) const {
    return this->pin == pin && is_active;
}

double Account::get_balance() {
    lock_guard<mutex> lock(*mtx);
    return balance;
}
