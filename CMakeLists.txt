cmake_minimum_required(VERSION 3.10)
project(lab1_os)

set(CMAKE_CXX_STANDARD 11)

# Находим Google Test
find_package(GTest REQUIRED)
find_package(GMock REQUIRED)

# Исходные файлы
add_executable(creator src/Creator.cpp)
add_executable(reporter src/Reporter.cpp)
add_executable(main src/Main.cpp)

# Тесты
add_executable(test_runner 
    tests/test_runner.cpp
    tests/test_creator.cpp
    tests/test_reporter.cpp
)

target_link_libraries(test_runner 
    GTest::GTest
    GTest::Main
    GMock::GMock
)

enable_testing()
add_test(NAME lab1_tests COMMAND test_runner)
