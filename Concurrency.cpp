#include "Concurrency.h"
#include "Transaction.h"
#include <thread>
#include <chrono>

using namespace std;

void Concurrency::banking_operation(Account& acc1, Account& acc2, int thread_id) {
    string pin = acc1.pin; // Use the account's PIN instead of hardcoded "1234"
    Transaction::deposit(acc1, 100.0 * (thread_id + 1), pin);
    this_thread::sleep_for(chrono::seconds(1));
    Transaction::withdraw(acc1, 50.0 * (thread_id + 1), pin);
    this_thread::sleep_for(chrono::seconds(1));
    Transaction::transfer(acc1, acc2, 25.0 * (thread_id + 1), pin);
    this_thread::sleep_for(chrono::seconds(1));
    Transaction::request_loan(acc1, 200.0, pin);
    this_thread::sleep_for(chrono::seconds(1));
    Transaction::pay_loan(acc1, 50.0, pin);
    this_thread::sleep_for(chrono::seconds(1));
    Transaction::apply_interest(acc1);
}

void Concurrency::start_transactions(vector<Account>& accounts) {
    vector<thread> threads;
    for (int i = 0; i < 5; ++i) {
        int acc1_idx = i % accounts.size();
        int acc2_idx = (i + 1) % accounts.size();
        threads.emplace_back(banking_operation, ref(accounts[acc1_idx]), ref(accounts[acc2_idx]), i);
    }
    for (auto& t : threads) t.join();
}
