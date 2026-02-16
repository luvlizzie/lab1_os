#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <string>
#include <vector>
#include <set>
#include <cstdio>
#include "../include/employee.h"

using ::testing::HasSubstr;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::set;
using std::ios;

static constexpr int TEST_EMPLOYEES_COUNT = 3;
static constexpr int MULTIPLE_EMPLOYEES_COUNT = 5;
static constexpr int SINGLE_EMPLOYEE = 1;

class CreatorTest : public ::testing::Test {
protected:
    void SetUp() override {
        system("mkdir -p test_data");
    }
    
    void TearDown() override {
        system("rm -rf test_data");
    }
    
    vector<Employee> readBinaryFile(const string& filename) {
        vector<Employee> employees;
        ifstream file(filename, ios::binary);
        if (!file.is_open()) return employees;
        
        Employee emp;
        while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
            employees.push_back(emp);
        }
        return employees;
    }
};

TEST_F(CreatorTest, CreatesBinaryFile) {
    string filename = "test_data/employees.bin";
    int count = TEST_EMPLOYEES_COUNT;
    
    string cmd = "./Creator.exe " + filename + " " + std::to_string(count);
    int result = system(cmd.c_str());
    
    EXPECT_EQ(result, 0);
    
    ifstream file(filename);
    EXPECT_TRUE(file.good());
}

TEST_F(CreatorTest, WritesCorrectEmployeeData) {
    string filename = "test_data/employees.bin";
    int count = SINGLE_EMPLOYEE;
    
    string cmd = "./Creator.exe " + filename + " " + std::to_string(count);
    system(cmd.c_str());
    
    auto employees = readBinaryFile(filename);
    
    ASSERT_EQ(employees.size(), count);
    
    if (!employees.empty()) {
        const auto& emp = employees[0];
        EXPECT_GT(emp.num, 0);
        EXPECT_STRNE(emp.name, "");
        EXPECT_GT(emp.hours, 0);
    }
}

TEST_F(CreatorTest, CreatesMultipleEmployees) {
    string filename = "test_data/employees.bin";
    int count = MULTIPLE_EMPLOYEES_COUNT;
    
    string cmd = "./Creator.exe " + filename + " " + std::to_string(count);
    system(cmd.c_str());
    
    auto employees = readBinaryFile(filename);
    
    ASSERT_EQ(employees.size(), count);
    
    set<int> ids;
    for (const auto& emp : employees) {
        ids.insert(emp.num);
    }
    EXPECT_EQ(ids.size(), employees.size());
}
