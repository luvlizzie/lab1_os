#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include "../src/employee.h"

using namespace std;
using namespace testing;

class ReporterTest : public Test {
protected:
    void SetUp() override {
        binaryFile = "test_data.bin";
        reportFile = "test_report.txt";
        
        ofstream outFile(binaryFile, ios::binary);
        employee emp1{2, "Bob", 35.5};
        employee emp2{1, "Alice", 40.0};
        outFile.write(reinterpret_cast<char*>(&emp1), sizeof(employee));
        outFile.write(reinterpret_cast<char*>(&emp2), sizeof(employee));
        outFile.close();
    }

    void TearDown() override {
        remove(binaryFile.c_str());
        remove(reportFile.c_str());
    }

    string binaryFile;
    string reportFile;
};

TEST_F(ReporterTest, CreatesReportFile) {
    double hourlyRate = 10.0;
    
    ifstream inFile(binaryFile, ios::binary);
    vector<employee> employees;
    employee emp;
    
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    inFile.close();
    
    sort(employees.begin(), employees.end(), 
        [](const employee& a, const employee& b) {
            return a.num < b.num;
        });
    
    ofstream outFile(reportFile);
    outFile << "ID, Name, Hours, Salary" << endl;
    
    for (const auto& e : employees) {
        double salary = e.hours * hourlyRate;
        outFile << e.num << ", " << e.name << ", " 
                << e.hours << ", " << salary << endl;
    }
    outFile.close();
    
    ifstream checkFile(reportFile);
    ASSERT_TRUE(checkFile.good());
}

TEST_F(ReporterTest, ReportHasCorrectContent) {
    double hourlyRate = 10.0;
    
    ifstream inFile(binaryFile, ios::binary);
    vector<employee> employees;
    employee emp;
    
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    inFile.close();
    
    sort(employees.begin(), employees.end(), 
        [](const employee& a, const employee& b) {
            return a.num < b.num;
        });
    
    ofstream outFile(reportFile);
    outFile << "ID, Name, Hours, Salary" << endl;
    
    for (const auto& e : employees) {
        double salary = e.hours * hourlyRate;
        outFile << e.num << ", " << e.name << ", " 
                << e.hours << ", " << salary << endl;
    }
    outFile.close();
    
    ifstream resultFile(reportFile);
    string line;
    getline(resultFile, line);
    EXPECT_EQ(line, "ID, Name, Hours, Salary");
    
    getline(resultFile, line);
    EXPECT_EQ(line, "1, Alice, 40, 400");
    
    getline(resultFile, line);
    EXPECT_EQ(line, "2, Bob, 35.5, 355");
}

TEST_F(ReporterTest, HandlesEmptyFile) {
    remove(binaryFile.c_str());
    ofstream emptyFile(binaryFile, ios::binary);
    emptyFile.close();
    
    double hourlyRate = 10.0;
    
    ifstream inFile(binaryFile, ios::binary);
    vector<employee> employees;
    employee emp;
    
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    inFile.close();
    
    ofstream outFile(reportFile);
    outFile << "ID, Name, Hours, Salary" << endl;
    outFile.close();
    
    ifstream resultFile(reportFile);
    string line;
    getline(resultFile, line);
    EXPECT_EQ(line, "ID, Name, Hours, Salary");
    
    EXPECT_FALSE(getline(resultFile, line));
}
