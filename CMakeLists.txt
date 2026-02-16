cmake_minimum_required(VERSION 3.14)
project(EmployeeRecordsSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Creator.exe src/Creator.cpp)
add_executable(Reporter.exe src/Reporter.cpp)
add_executable(Main.exe src/Main.cpp)

target_include_directories(Creator.exe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Reporter.exe PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(Main.exe PRIVATE ${CMAKE_SOURCE_DIR}/include)

if (MINGW)
    set_target_properties(Creator.exe PROPERTIES SUFFIX "")
    set_target_properties(Reporter.exe PROPERTIES SUFFIX "")
    set_target_properties(Main.exe PROPERTIES SUFFIX "")
endif()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(test_runner 
    tests/test_runner.cpp
    tests/test_creator.cpp 
    tests/test_reporter.cpp
)

target_link_libraries(test_runner 
    gtest_main
    gmock
    pthread
)

target_include_directories(test_runner PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
)

add_custom_command(TARGET test_runner POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_BINARY_DIR}/Creator.exe
        ${CMAKE_BINARY_DIR}/Reporter.exe
        ${CMAKE_BINARY_DIR}
)

enable_testing()
add_test(NAME all_tests COMMAND test_runner)
