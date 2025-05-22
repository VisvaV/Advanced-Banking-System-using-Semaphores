# Advanced Banking System

A robust, multithreaded banking simulation application built using C++ that replicates real-world banking operations. This system supports concurrent transaction processing, inter-process communication (IPC), signal handling, file-based persistence, and a secure admin interface. It’s an advanced Operating Systems project designed to demonstrate practical OS concepts like threading, synchronization, shared memory, and process communication in a banking context.

---

## Features

- **Account Management:** Create, delete, and manage bank accounts with different types like Savings, Current, and Business.
- **Transaction Support:** Deposit, withdraw, transfer money, request and pay loans — all with logging and error handling.
- **Multithreading:** Simulates real-world concurrency by processing transactions across multiple threads.
- **Synchronization:** Uses mutexes and semaphores to ensure safe access to shared data.
- **Data Persistence:** Account and transaction information is stored in files (`accounts.dat`, `transactions.log`).
- **Shared Memory IPC:** Windows shared memory is used to enable communication and state sharing between processes.
- **Signal Handling:** Handles system signals like SIGINT to perform safe shutdown and data saving.
- **Interest Module:** Applies periodic interest to accounts.
- **Backup and Restore:** Supports creating and restoring backups of account data.
- **Admin Interface:** Password-protected interface to view and manage accounts, transactions, and backups.
- **Platform Support:** Designed for Windows, particularly for IPC features.

---

## Modules Overview

### 1. Account Module
- Encapsulates account details such as ID, name, age, balance, loan details, account type.
- Provides thread-safe operations using `std::mutex`.
- Includes helper methods for account creation, display, authentication, and deletion.

### 2. Transaction Module
- Handles core banking functions: deposit, withdraw, transfer, loan handling.
- Logs all transactions with timestamps in `transactions.log`.
- Ensures atomicity using mutex locking.

### 3. Concurrency Handler
- Simulates real-time transaction processing using threads.
- Creates multiple threads that operate on random accounts performing varied actions.
- Uses semaphores and mutexes to avoid race conditions.

### 4. File I/O and Persistence
- Loads account data from `accounts.dat` at start and saves changes during shutdown.
- Supports creating human-readable logs of transactions and backups.

### 5. Shared Memory / IPC (Windows)
- Implements inter-process communication using `CreateFileMapping`, `MapViewOfFile`, etc.
- Demonstrates process-safe memory sharing of account state.

### 6. Signal Handling
- Captures `SIGINT` (Ctrl+C) using `signal()` and performs clean shutdown and data saving.

### 7. Interest Calculation
- Periodically (every 10 seconds) applies interest to savings and business accounts.

### 8. Admin Interface
- Secured via PIN/password (default: `admin123`).
- Allows admin to view all accounts, transactions, and backup data.

---

##  Compilation Instructions

### Requirements
- C++ Compiler with C++11+ support (e.g., g++ or MSVC)
- Windows (for IPC features using `Windows.h`)

### Compilation (G++)

```bash
g++ -o banking_system main.cpp -std=c++11
```

---

## Example Workflow

1. User launches the app, and sample accounts are loaded or created.
2. Multiple threads simulate user transactions (deposit, withdraw, transfer, loan).
3. Accounts and transactions are safely persisted.
4. Admin can log in to review summaries and create backups.
5. On SIGINT or manual exit, system saves state and logs.

---

##  Limitations

- IPC works only on Windows due to dependency on `Windows.h`.
- Shared memory block is limited (fixed to 50 accounts).
- Simplified PIN system (stored as plain integers).
- No encryption/security beyond basic access control.
- Console-only interface (no GUI).
- Interest is simulated every 10 seconds for demo purposes.
- Logging is done in plaintext files.

---

## Project Structure

```
project/
│── main.cpp                # Main driver and UI
│── account.h / account.cpp # Account class
│── transaction.h/.cpp     # Transaction handling logic
│── ipc.h/.cpp             # IPC via shared memory (Windows)
│── interest.h/.cpp        # Periodic interest calculation
│── fileio.h/.cpp          # File load/save/backup logic
│── admin.h/.cpp           # Admin login and operations
│── utils.h/.cpp           # Helpers and utilities
│── transactions.log       # Logs of all transactions
│── accounts.dat           # Persistent data for accounts
│── backup.dat             # Backup file
```

---

## License

This project is open-sourced under the MIT License.
