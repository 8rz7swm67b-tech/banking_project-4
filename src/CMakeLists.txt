add_library(banking_lib
    MonetaryInstrument.cpp
    Cash.cpp
    Check.cpp
    Transaction.cpp
    Account.cpp
    CheckingAccount.cpp
    SavingsAccount.cpp
)
target_include_directories(banking_lib PUBLIC ${CMAKE_SOURCE_DIR}/include)
add_executable(banking_app main.cpp)
target_link_libraries(banking_app PRIVATE banking_lib)
