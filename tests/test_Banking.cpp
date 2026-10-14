/**
 * @file test_Banking.cpp
 * @brief Tests instruments, account policies, balances, and histories.
 */
#include "Cash.h"
#include "Check.h"
#include "CheckingAccount.h"
#include "SavingsAccount.h"
#include "Transaction.h"
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>
#include <vector>

/** @brief Checks each denomination and the assignment's example. */
TEST(CashTest, ComputesDenominationTotal) {
    EXPECT_DOUBLE_EQ(Cash(2, 1, 0, 3).GetAmount(), 53.0);
    EXPECT_DOUBLE_EQ(Cash(1, 0, 0, 0).GetAmount(), 20.0);
    EXPECT_DOUBLE_EQ(Cash(0, 1, 0, 0).GetAmount(), 10.0);
    EXPECT_DOUBLE_EQ(Cash(0, 0, 1, 0).GetAmount(), 5.0);
    EXPECT_DOUBLE_EQ(Cash(0, 0, 0, 1).GetAmount(), 1.0);
    EXPECT_DOUBLE_EQ(Cash(0, 0, 0, 0).GetAmount(), 0.0);
}

/** @brief Checks virtual queries through base class references. */
TEST(InstrumentTest, UsesPolymorphicQueries) {
    Cash cash(2, 1, 0, 3);
    Check check(75.25, "101", "Sample Bank");
    MonetaryInstrument& cashBase = cash;
    MonetaryInstrument& checkBase = check;
    EXPECT_TRUE(cashBase.IsCash());
    EXPECT_FALSE(cashBase.IsCheck());
    EXPECT_DOUBLE_EQ(cashBase.GetAmount(), 53.0);
    EXPECT_FALSE(checkBase.IsCash());
    EXPECT_TRUE(checkBase.IsCheck());
    EXPECT_DOUBLE_EQ(checkBase.GetAmount(), 75.25);
}

/** @brief Checks constructor validation for instrument values. */
TEST(InstrumentTest, RejectsNegativeValues) {
    EXPECT_THROW(Cash(-1, 0, 0, 0), std::invalid_argument);
    EXPECT_THROW(Cash(0, -1, 0, 0), std::invalid_argument);
    EXPECT_THROW(Cash(0, 0, -1, 0), std::invalid_argument);
    EXPECT_THROW(Cash(0, 0, 0, -1), std::invalid_argument);
    EXPECT_THROW(Check(-1.0, "101", "Sample Bank"), std::invalid_argument);
}

/** @brief Checks that a transaction retains all supplied fields. */
TEST(TransactionTest, StoresRecordAndInstrument) {
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
    Transaction transaction(20.0, "2026-10-01", TransactionType::Deposit, cash);
    EXPECT_DOUBLE_EQ(transaction.GetAmount(), 20.0);
    EXPECT_EQ(transaction.GetDate(), "2026-10-01");
    EXPECT_EQ(transaction.GetType(), TransactionType::Deposit);
    EXPECT_EQ(&transaction.GetInstrument(), cash.get());
}

/** @brief A transaction must contain an instrument. */
TEST(TransactionTest, RejectsNullInstrument) {
    EXPECT_THROW(Transaction(20.0, "2026-10-01", TransactionType::Deposit, nullptr),
                 std::invalid_argument);
}

/** @brief Checks initial balances and empty histories. */
TEST(AccountTest, StartsWithInitialBalanceAndEmptyHistory) {
    CheckingAccount checking(100.0);
    SavingsAccount savings(200.0);
    EXPECT_DOUBLE_EQ(checking.GetBalance(), 100.0);
    EXPECT_DOUBLE_EQ(savings.GetBalance(), 200.0);
    EXPECT_TRUE(checking.GetTransactions().empty());
    EXPECT_TRUE(savings.GetTransactions().empty());
    EXPECT_THROW(CheckingAccount(-1.0), std::invalid_argument);
    EXPECT_THROW(SavingsAccount(-1.0), std::invalid_argument);
}

/** @brief Tests all four entries in the checking account policy table. */
TEST(CheckingTest, EnforcesInstrumentPolicies) {
    CheckingAccount account(100.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
    std::shared_ptr<MonetaryInstrument> check =
        std::make_shared<Check>(30.0, "101", "Sample Bank");
    EXPECT_TRUE(account.Deposit(Transaction(20.0, "2026-10-01", TransactionType::Deposit, cash)));
    EXPECT_TRUE(account.Deposit(Transaction(30.0, "2026-10-02", TransactionType::Deposit, check)));
    EXPECT_TRUE(account.Withdraw(Transaction(20.0, "2026-10-03", TransactionType::Withdrawal, cash)));
    EXPECT_FALSE(account.Withdraw(Transaction(30.0, "2026-10-04", TransactionType::Withdrawal, check)));
    EXPECT_DOUBLE_EQ(account.GetBalance(), 130.0);
    EXPECT_EQ(account.GetTransactions().size(), 3u);
}

/** @brief Tests all four entries in the savings account policy table. */
TEST(SavingsTest, EnforcesInstrumentPolicies) {
    SavingsAccount account(100.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
    std::shared_ptr<MonetaryInstrument> check =
        std::make_shared<Check>(30.0, "101", "Sample Bank");
    EXPECT_TRUE(account.Deposit(Transaction(20.0, "2026-10-01", TransactionType::Deposit, cash)));
    EXPECT_FALSE(account.Deposit(Transaction(30.0, "2026-10-02", TransactionType::Deposit, check)));
    EXPECT_TRUE(account.Withdraw(Transaction(20.0, "2026-10-03", TransactionType::Withdrawal, cash)));
    EXPECT_FALSE(account.Withdraw(Transaction(30.0, "2026-10-04", TransactionType::Withdrawal, check)));
    EXPECT_DOUBLE_EQ(account.GetBalance(), 100.0);
    EXPECT_EQ(account.GetTransactions().size(), 2u);
}

/** @brief Checks that the requested operation matches the transaction type. */
TEST(AccountTest, RejectsWrongTransactionTypes) {
    CheckingAccount checking(100.0);
    SavingsAccount savings(100.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
    Transaction deposit(20.0, "2026-10-01", TransactionType::Deposit, cash);
    Transaction withdrawal(20.0, "2026-10-01", TransactionType::Withdrawal, cash);
    std::vector<Account*> accounts = {&checking, &savings};
    for (Account* account : accounts) {
        EXPECT_FALSE(account->Deposit(withdrawal));
        EXPECT_FALSE(account->Withdraw(deposit));
        EXPECT_TRUE(account->GetTransactions().empty());
        EXPECT_DOUBLE_EQ(account->GetBalance(), 100.0);
    }
}

/** @brief Rejected amounts must not change either account's history. */
TEST(AccountTest, RejectsNonpositiveAmounts) {
    CheckingAccount checking(100.0);
    SavingsAccount savings(100.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
    std::vector<Account*> accounts = {&checking, &savings};
    for (Account* account : accounts) {
        EXPECT_FALSE(account->Deposit(Transaction(0.0, "2026-10-01", TransactionType::Deposit, cash)));
        EXPECT_FALSE(account->Deposit(Transaction(-10.0, "2026-10-01", TransactionType::Deposit, cash)));
        EXPECT_FALSE(account->Withdraw(Transaction(0.0, "2026-10-01", TransactionType::Withdrawal, cash)));
        EXPECT_FALSE(account->Withdraw(Transaction(-10.0, "2026-10-01", TransactionType::Withdrawal, cash)));
        EXPECT_TRUE(account->GetTransactions().empty());
        EXPECT_DOUBLE_EQ(account->GetBalance(), 100.0);
    }
}

/** @brief Tests the full-balance boundary and an overdraft attempt. */
TEST(AccountTest, AllowsFullWithdrawalButRejectsOverdraft) {
    CheckingAccount checking(100.0);
    SavingsAccount savings(100.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(5, 0, 0, 0);
    std::vector<Account*> accounts = {&checking, &savings};
    for (Account* account : accounts) {
        EXPECT_FALSE(account->Withdraw(Transaction(101.0, "2026-10-01", TransactionType::Withdrawal, cash)));
        EXPECT_TRUE(account->Withdraw(Transaction(100.0, "2026-10-02", TransactionType::Withdrawal, cash)));
        EXPECT_DOUBLE_EQ(account->GetBalance(), 0.0);
        EXPECT_EQ(account->GetTransactions().size(), 1u);
    }
}

/** @brief Checks history order, amounts, dates, types, and instruments. */
TEST(AccountTest, PreservesHistoryAndRecomputesBalance) {
    CheckingAccount account(100.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
    std::shared_ptr<MonetaryInstrument> check =
        std::make_shared<Check>(12.5, "102", "Sample Bank");
    ASSERT_TRUE(account.Deposit(Transaction(20.0, "2026-10-01", TransactionType::Deposit, cash)));
    EXPECT_DOUBLE_EQ(account.GetBalance(), 120.0);
    ASSERT_TRUE(account.Withdraw(Transaction(5.0, "2026-10-02", TransactionType::Withdrawal, cash)));
    EXPECT_DOUBLE_EQ(account.GetBalance(), 115.0);
    ASSERT_TRUE(account.Deposit(Transaction(12.5, "2026-10-03", TransactionType::Deposit, check)));
    EXPECT_DOUBLE_EQ(account.GetBalance(), 127.5);
    EXPECT_DOUBLE_EQ(account.GetBalance(), 127.5);
    const std::vector<Transaction>& history = account.GetTransactions();
    ASSERT_EQ(history.size(), 3u);
    EXPECT_EQ(history[0].GetDate(), "2026-10-01");
    EXPECT_DOUBLE_EQ(history[0].GetAmount(), 20.0);
    EXPECT_TRUE(history[0].GetInstrument().IsCash());
    EXPECT_EQ(history[1].GetDate(), "2026-10-02");
    EXPECT_EQ(history[1].GetType(), TransactionType::Withdrawal);
    EXPECT_DOUBLE_EQ(history[1].GetAmount(), 5.0);
    EXPECT_EQ(history[2].GetDate(), "2026-10-03");
    EXPECT_EQ(history[2].GetType(), TransactionType::Deposit);
    EXPECT_DOUBLE_EQ(history[2].GetAmount(), 12.5);
    EXPECT_TRUE(history[2].GetInstrument().IsCheck());
}

/** @brief Uses the separate transaction amount, as in the sample driver. */
TEST(AccountTest, UsesTransactionAmountForBalance) {
    CheckingAccount account(0.0);
    std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(2, 3, 4, 5);
    EXPECT_DOUBLE_EQ(cash->GetAmount(), 95.0);
    EXPECT_TRUE(account.Deposit(Transaction(100.0, "2026-10-01", TransactionType::Deposit, cash)));
    EXPECT_DOUBLE_EQ(account.GetBalance(), 100.0);
}

/** @brief Different accounts apply their own policies through base pointers. */
TEST(AccountTest, SupportsRuntimePolymorphism) {
    CheckingAccount checking(0.0);
    SavingsAccount savings(0.0);
    Account* first = &checking;
    Account* second = &savings;
    std::shared_ptr<MonetaryInstrument> check =
        std::make_shared<Check>(20.0, "103", "Sample Bank");
    Transaction deposit(20.0, "2026-10-01", TransactionType::Deposit, check);
    EXPECT_EQ(first->GetAccountType(), "Checking");
    EXPECT_EQ(second->GetAccountType(), "Savings");
    EXPECT_TRUE(first->Deposit(deposit));
    EXPECT_FALSE(second->Deposit(deposit));
}

/** @brief A copied transaction keeps its instrument alive. */
TEST(AccountTest, KeepsInstrumentAfterOriginalTransactionEnds) {
    CheckingAccount account(0.0);
    {
        std::shared_ptr<MonetaryInstrument> cash = std::make_shared<Cash>(1, 0, 0, 0);
        Transaction deposit(20.0, "2026-10-01", TransactionType::Deposit, cash);
        ASSERT_TRUE(account.Deposit(deposit));
    }
    ASSERT_EQ(account.GetTransactions().size(), 1u);
    EXPECT_TRUE(account.GetTransactions()[0].GetInstrument().IsCash());
    EXPECT_DOUBLE_EQ(account.GetTransactions()[0].GetInstrument().GetAmount(), 20.0);
    EXPECT_DOUBLE_EQ(account.GetBalance(), 20.0);
}
