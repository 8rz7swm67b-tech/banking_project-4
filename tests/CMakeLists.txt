include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(test_Banking test_Banking.cpp test_project2basic.cpp)
target_link_libraries(test_Banking PRIVATE banking_lib GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(test_Banking)
