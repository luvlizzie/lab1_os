#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
#include "../src/employee.h"

using namespace testing;
using namespace std;

class ReporterTest : public ::testing::Test {
protected:
    void SetUp() override {
        system("mkdir -p test_data");
        createTestBinaryFile("test_data/test.bin", 3);
    }
    
    void TearDown() override {
        system("rm -rf test_data");
    }
    
    void createTestBinaryFile(const string& filename, int count) {
        ofstream file(filename, ios::binary);
        for (int i = 1; i <= count; ++i) {
            employee emp;
            emp.num = i;
            string name = "Emp" + to_string(i);
            strncpy(emp.name, name.c_str(), sizeof(emp.name) - 1);
            emp.name[sizeof(emp.name) - 1] = '\0';
            emp.hours = i * 10;
            file.write(reinterpret_cast<char*>(&emp), sizeof(employee));
        }
    }
    
    string readReportFile(const string& filename) {
        ifstream file(filename);
        stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
};

TEST_F(ReporterTest, CreatesReportFile) {
    string binFile = "test_data/test.bin";
    string reportFile = "test_data/report.txt";
    double hourlyRate = 100.0;
    
    string cmd = "./Reporter.exe " + binFile + " " + 
                 reportFile + " " + to_string(hourlyRate);
    int result = system(cmd.c_str());
    
    EXPECT_EQ(result, 0);
    
    ifstream file(reportFile);
    EXPECT_TRUE(file.good());
}

TEST_F(ReporterTest, ReportHasCorrectContent) {
    string binFile = "test_data/test.bin";
    string reportFile = "test_data/report.txt";
    double hourlyRate = 100.0;
    
    string cmd = "./Reporter.exe " + binFile + " " + 
                 reportFile + " " + to_string(hourlyRate);
    system(cmd.c_str());
    
    string content = readReportFile(reportFile);
    
    EXPECT_THAT(content, HasSubstr("Report for file:"));
    EXPECT_THAT(content, HasSubstr("ID, Name, Hours, Salary"));
    EXPECT_THAT(content, HasSubstr("1, Emp1, 10, 1000"));
    EXPECT_THAT(content, HasSubstr("2, Emp2, 20, 2000"));
    EXPECT_THAT(content, HasSubstr("3, Emp3, 30, 3000"));
}

TEST_F(ReporterTest, HandlesEmptyFile) {
    string emptyFile = "test_data/empty.bin";
    string reportFile = "test_data/report.txt";
    double hourlyRate = 100.0;
    
    ofstream file(emptyFile, ios::binary);
    file.close();
    
    string cmd = "./Reporter.exe " + emptyFile + " " + 
                 reportFile + " " + to_string(hourlyRate);
    int result = system(cmd.c_str());
    
    EXPECT_EQ(result, 0);
    
    ifstream report(reportFile);
    EXPECT_TRUE(report.good());
    
    string content = readReportFile(reportFile);
    EXPECT_THAT(content, HasSubstr("Report for file:"));
    EXPECT_THAT(content, HasSubstr("ID, Name, Hours, Salary"));
    
    stringstream ss(content);
    string line;
    int lineCount = 0;
    while (getline(ss, line)) {
        lineCount++;
    }
    EXPECT_LE(lineCount, 2);
}

TEST_F(ReporterTest, CalculatesCorrectSalaries) {
    string binFile = "test_data/test.bin";
    string reportFile = "test_data/report.txt";
    double hourlyRate = 100.0;
    
    string cmd = "./Reporter.exe " + binFile + " " + 
                 reportFile + " " + to_string(hourlyRate);
    system(cmd.c_str());
    
    string content = readReportFile(reportFile);
    
    stringstream ss(content);
    string line;
    int lineNum = 0;
    while (getline(ss, line)) {
        lineNum++;
        if (lineNum <= 2) continue;
        
        size_t pos1 = line.find(',');
        size_t pos2 = line.find(',', pos1 + 1);
        size_t pos3 = line.find(',', pos2 + 1);
        
        if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
            int id = stoi(line.substr(0, pos1));
            string name = line.substr(pos1 + 2, pos2 - pos1 - 2);
            int hours = stoi(line.substr(pos2 + 2, pos3 - pos2 - 2));
            int salary = stoi(line.substr(pos3 + 2));
            
            EXPECT_EQ(salary, hours * 100);
        }
    }
}
