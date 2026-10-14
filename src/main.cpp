/**
 * @file main.cpp
 * @brief Demonstrates both account types and every instrument policy.
 */
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "Cash.h"
#include "Check.h"
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>

/**
 * @brief Prints an account balance and its transaction history.
 * @param account Account to display through the base class interface.
 */
void PrintAccount(const Account& account) {
    std::cout << "\n" << account.GetAccountType() << " balance: $"
              << account.GetBalance() << "\n";
    std::cout << "Transaction history:\n";
    for (const Transaction& transaction : account.GetTransactions()) {
        std::cout << transaction.GetDate() << " | ";
        if (transaction.GetType() == TransactionType::Deposit) {
            std::cout << "Deposit";
        } else {
            std::cout << "Withdrawal";
        }
        std::cout << " | $" << transaction.GetAmount() << " | ";
        if (transaction.GetInstrument().IsCash()) {
            std::cout << "Cash\n";
        } else {
            std::cout << "Check\n";
        }
    }
}

/** @brief Runs the banking demonstration. @return Zero on completion. */
int main() {
    CheckingAccount checking(1000.0);
    SavingsAccount savings(500.0);
    std::shared_ptr<MonetaryInstrument> cash =
        std::make_shared<Cash>(2, 1, 0, 3);
    std::shared_ptr<MonetaryInstrument> check =
        std::make_shared<Check>(100.0, "123456", "My Bank");
    std::shared_ptr<MonetaryInstrument> withdrawalCash =
        std::make_shared<Cash>(1, 0, 1, 0);

    Transaction cashDeposit(53.0, "2026-10-01", TransactionType::Deposit, cash);
    Transaction checkDeposit(100.0, "2026-10-01", TransactionType::Deposit, check);
    Transaction cashWithdrawal(25.0, "2026-10-02", TransactionType::Withdrawal,
                               withdrawalCash);
    Transaction checkWithdrawal(100.0, "2026-10-02", TransactionType::Withdrawal,
                                check);

    std::vector<Account*> accounts;
    accounts.push_back(&checking);
    accounts.push_back(&savings);

    std::cout << std::fixed << std::setprecision(2) << std::boolalpha;
    std::cout << "Cash: 2 twenties, 1 ten, 0 fives, 3 ones = $"
              << cash->GetAmount() << "\n";
    std::cout << "Check: #123456, My Bank, $" << check->GetAmount() << "\n";
    for (Account* account : accounts) {
        std::cout << "\n" << account->GetAccountType() << " account\n";
        std::cout << "Cash deposit accepted: " << account->Deposit(cashDeposit) << "\n";
        std::cout << "Check deposit accepted: " << account->Deposit(checkDeposit) << "\n";
        std::cout << "Cash withdrawal accepted: " << account->Withdraw(cashWithdrawal) << "\n";
        std::cout << "Check withdrawal accepted: " << account->Withdraw(checkWithdrawal) << "\n";
        PrintAccount(*account);
    }
    return 0;
}
