#include "Interest.h"
#include "Transaction.h"
#include <thread>
#include <chrono>

using namespace std;

void Interest::calculate_interest(vector<Account>& accounts) {
    while (true) {
        this_thread::sleep_for(chrono::seconds(10));
        for (auto& acc : accounts) {
            Transaction::apply_interest(acc);
        }
    }
}

void Interest::start_interest_calculation(vector<Account>& accounts) {
    thread(calculate_interest, ref(accounts)).detach();
}
