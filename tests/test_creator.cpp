#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include "../src/employee.h"

using namespace std;
using namespace testing;

class CreatorTest : public Test {
protected:
    void SetUp() override {
        testFilename = "test_creator.bin";
    }

    void TearDown() override {
        remove(testFilename.c_str());
    }

    string testFilename;
};

TEST_F(CreatorTest, CreatesBinaryFile) {
    ofstream file(testFilename, ios::binary);
    ASSERT_TRUE(file.is_open());
    file.close();
    
    ifstream checkFile(testFilename);
    ASSERT_TRUE(checkFile.good());
}

TEST_F(CreatorTest, WritesCorrectEmployeeData) {
    employee emp{1, "Test", 40.0};
    
    ofstream outFile(testFilename, ios::binary);
    outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    outFile.close();
    
    ifstream inFile(testFilename, ios::binary);
    employee readEmp;
    inFile.read(reinterpret_cast<char*>(&readEmp), sizeof(employee));
    
    EXPECT_EQ(readEmp.num, 1);
    EXPECT_STREQ(readEmp.name, "Test");
    EXPECT_DOUBLE_EQ(readEmp.hours, 40.0);
}

TEST_F(CreatorTest, CreatesMultipleEmployees) {
    employee emp1{1, "Alice", 40.0};
    employee emp2{2, "Bob", 35.5};
    
    ofstream outFile(testFilename, ios::binary);
    outFile.write(reinterpret_cast<char*>(&emp1), sizeof(employee));
    outFile.write(reinterpret_cast<char*>(&emp2), sizeof(employee));
    outFile.close();
    
    ifstream inFile(testFilename, ios::binary);
    employee readEmp1, readEmp2;
    inFile.read(reinterpret_cast<char*>(&readEmp1), sizeof(employee));
    inFile.read(reinterpret_cast<char*>(&readEmp2), sizeof(employee));
    
    EXPECT_EQ(readEmp1.num, 1);
    EXPECT_STREQ(readEmp1.name, "Alice");
    EXPECT_DOUBLE_EQ(readEmp1.hours, 40.0);
    
    EXPECT_EQ(readEmp2.num, 2);
    EXPECT_STREQ(readEmp2.name, "Bob");
    EXPECT_DOUBLE_EQ(readEmp2.hours, 35.5);
}
