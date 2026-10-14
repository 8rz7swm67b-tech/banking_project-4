#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "Account.h"
#include "Cash.h"
#include "Check.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "Transaction.h"

//
// Cash Tests
//

TEST(CashTests, ComputesAmountCorrectly) {
  Cash cash(2, 1, 0, 3);

  EXPECT_DOUBLE_EQ(cash.GetAmount(), 53.0);
}

TEST(CashTests, ZeroDenominationsProducesZero) {
  Cash cash(0, 0, 0, 0);

  EXPECT_DOUBLE_EQ(cash.GetAmount(), 0.0);
}

TEST(CashTests, ReportsCashType) {
  Cash cash(1, 0, 0, 0);

  EXPECT_TRUE(cash.IsCash());
  EXPECT_FALSE(cash.IsCheck());
}

//
// Check Tests
//

TEST(CheckTests, StoresAmount) {
  Check check(250.0, "1001", "ABC Bank");

  EXPECT_DOUBLE_EQ(check.GetAmount(), 250.0);
}

TEST(CheckTests, ReportsCheckType) {
  Check check(100.0, "1001", "ABC Bank");

  EXPECT_TRUE(check.IsCheck());
  EXPECT_FALSE(check.IsCash());
}

//
// Transaction Tests
//

TEST(TransactionTests, StoresAmount) {
  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(20.0, "2026-10-01", TransactionType::Deposit, cash);

  EXPECT_DOUBLE_EQ(tx.GetAmount(), 20.0);
}

TEST(TransactionTests, StoresDate) {
  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(20.0, "2026-10-01", TransactionType::Deposit, cash);

  EXPECT_EQ(tx.GetDate(), "2026-10-01");
}

TEST(TransactionTests, StoresTransactionType) {
  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(20.0, "2026-10-01", TransactionType::Deposit, cash);

  EXPECT_EQ(tx.GetType(), TransactionType::Deposit);
}

//
// Checking Account Tests
//

TEST(CheckingAccountTests, AcceptsCashDeposit) {
  CheckingAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(20.0, "2026-10-01", TransactionType::Deposit, cash);

  EXPECT_TRUE(account.Deposit(tx));
}

TEST(CheckingAccountTests, AcceptsCheckDeposit) {
  CheckingAccount account(100.0);

  auto check = std::make_shared<Check>(50.0, "1001", "ABC Bank");

  Transaction tx(50.0, "2026-10-01", TransactionType::Deposit, check);

  EXPECT_TRUE(account.Deposit(tx));
}

TEST(CheckingAccountTests, AcceptsCashWithdrawal) {
  CheckingAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(20.0, "2026-10-01", TransactionType::Withdrawal, cash);

  EXPECT_TRUE(account.Withdraw(tx));
}

TEST(CheckingAccountTests, RejectsCheckWithdrawal) {
  CheckingAccount account(100.0);

  auto check = std::make_shared<Check>(50.0, "1001", "ABC Bank");

  Transaction tx(50.0, "2026-10-01", TransactionType::Withdrawal, check);

  EXPECT_FALSE(account.Withdraw(tx));
}

//
// Savings Account Tests
//

TEST(SavingsAccountTests, AcceptsCashDeposit) {
  SavingsAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(20.0, "2026-10-01", TransactionType::Deposit, cash);

  EXPECT_TRUE(account.Deposit(tx));
}

TEST(SavingsAccountTests, RejectsCheckDeposit) {
  SavingsAccount account(100.0);

  auto check = std::make_shared<Check>(50.0, "1001", "ABC Bank");

  Transaction tx(50.0, "2026-10-01", TransactionType::Deposit, check);

  EXPECT_FALSE(account.Deposit(tx));
}

TEST(SavingsAccountTests, AcceptsCashWithdrawal) {
  SavingsAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  Transaction tx(10.0, "2026-10-01", TransactionType::Withdrawal, cash);

  EXPECT_TRUE(account.Withdraw(tx));
}

TEST(SavingsAccountTests, RejectsCheckWithdrawal) {
  SavingsAccount account(100.0);

  auto check = std::make_shared<Check>(50.0, "1001", "ABC Bank");

  Transaction tx(50.0, "2026-10-01", TransactionType::Withdrawal, check);

  EXPECT_FALSE(account.Withdraw(tx));
}

//
// Account Tests
//

TEST(AccountTests, EmptyHistoryUsesInitialBalance) {
  CheckingAccount account(125.0);

  EXPECT_DOUBLE_EQ(account.GetBalance(), 125.0);
}

TEST(AccountTests, ComputesBalanceFromHistory) {
  CheckingAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  account.Deposit(
      Transaction(20.0, "2026-10-01", TransactionType::Deposit, cash));

  account.Withdraw(
      Transaction(10.0, "2026-10-02", TransactionType::Withdrawal, cash));

  EXPECT_DOUBLE_EQ(account.GetBalance(), 110.0);
}

TEST(AccountTests, RejectsOverdraft) {
  CheckingAccount account(50.0);

  auto cash = std::make_shared<Cash>(5, 0, 0, 0);

  Transaction tx(100.0, "2026-10-01", TransactionType::Withdrawal, cash);

  EXPECT_FALSE(account.Withdraw(tx));
}

TEST(AccountTests, MultipleTransactionsComputeCorrectBalance) {
  CheckingAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  account.Deposit(
      Transaction(50.0, "2026-10-01", TransactionType::Deposit, cash));

  account.Deposit(
      Transaction(25.0, "2026-10-02", TransactionType::Deposit, cash));

  account.Withdraw(
      Transaction(30.0, "2026-10-03", TransactionType::Withdrawal, cash));

  EXPECT_DOUBLE_EQ(account.GetBalance(), 145.0);
}

//
// Transaction History Tests
//

TEST(AccountTests, TransactionHistoryContainsAcceptedTransactions) {
  CheckingAccount account(100.0);

  auto cash = std::make_shared<Cash>(1, 0, 0, 0);

  account.Deposit(
      Transaction(20.0, "2026-10-01", TransactionType::Deposit, cash));

  const auto &history = account.GetTransactions();

  ASSERT_EQ(history.size(), 1);

  EXPECT_EQ(history[0].GetType(), TransactionType::Deposit);
}

//
// Polymorphism Tests
//

TEST(PolymorphismTests, AccountsCanBeStoredPolymorphically) {
  std::vector<std::unique_ptr<Account>> accounts;

  accounts.push_back(std::make_unique<CheckingAccount>(100.0));

  accounts.push_back(std::make_unique<SavingsAccount>(100.0));

  EXPECT_EQ(accounts[0]->GetAccountType(), "Checking");

  EXPECT_EQ(accounts[1]->GetAccountType(), "Savings");
}

TEST(PolymorphismTests, BalanceCanBeAccessedViaBasePointer) {
  std::vector<std::unique_ptr<Account>> accounts;

  accounts.push_back(std::make_unique<CheckingAccount>(200.0));

  accounts.push_back(std::make_unique<SavingsAccount>(300.0));

  EXPECT_DOUBLE_EQ(accounts[0]->GetBalance(), 200.0);

  EXPECT_DOUBLE_EQ(accounts[1]->GetBalance(), 300.0);
}
