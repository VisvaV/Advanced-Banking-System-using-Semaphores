#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Account.h"
#include "Transaction.h"
#include "FileManager.h"
#include "IPC.h"
#include "SignalHandler.h"
#include <QMainWindow>
#include <vector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_createAccountButton_clicked();
    void on_depositButton_clicked();
    void on_withdrawButton_clicked();
    void on_transferButton_clicked();
    void on_checkBalanceButton_clicked();
    void on_requestLoanButton_clicked();
    void on_payLoanButton_clicked();
    void on_generateStatementButton_clicked();
    void on_deleteAccountButton_clicked();
    void on_backupButton_clicked();
    void on_restoreButton_clicked();
    void on_adminButton_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<Account> accounts;
    std::vector<TransactionLog> logs;
    void loadInitialData();
    void updateStatus(const QString& message);
};

#endif // MAINWINDOW_H
