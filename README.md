# Banking Project

## Design decisions

The project follows the supplied UML design. `Account` is an abstract base class for `CheckingAccount` and `SavingsAccount`. `MonetaryInstrument` is an abstract base class for `Cash` and `Check`. Each class has a header and an implementation file, and its data members are private.

Polymorphism lets each account apply its own rules through `CanDeposit` and `CanWithdraw`. Checking accounts accept cash and check deposits, but only cash withdrawals. Savings accounts accept only cash deposits and withdrawals. The driver uses a `std::vector<Account*>` to work with both account types. Virtual `IsCash` and `IsCheck` functions identify instruments without `dynamic_cast`.

Composition is used because each account contains a vector of transactions. Each transaction stores an amount, date, type, and a shared pointer to its instrument. The shared pointer keeps the instrument available for the transaction history. Cash calculates its value from bill counts, while a check stores its amount, number, and bank. Accounts store only an initial balance; `GetBalance` calculates the current balance from the transaction history.

One development challenge was handling transaction amounts that differ from instrument values, as shown in the assignment examples and supplied tests. The balance therefore uses the transaction amount without requiring the two values to match. Another challenge was keeping rejected transactions out of the history. The program checks the transaction type, positive amount, account policy, and sufficient funds before recording a transaction. Negative starting balances, negative instrument values, and null instruments are also rejected.
