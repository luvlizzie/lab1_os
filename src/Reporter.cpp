#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <iomanip>
#include <stdexcept>
#include "../include/employee.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::setw;
using std::left;
using std::fixed;
using std::setprecision;

static constexpr int TABLE_WIDTH = 50;
static constexpr int ID_WIDTH = 8;
static constexpr int NAME_WIDTH = 12;
static constexpr int HOURS_WIDTH = 10;
static constexpr int SALARY_WIDTH = 12;

vector<Employee> readEmployeesFromFile(const string& filename) {
    vector<Employee> employees;
    ifstream file(filename, std::ios::binary);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    Employee emp;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        employees.push_back(emp);
    }
    
    if (file.bad()) {
        throw std::runtime_error("Error reading file: " + filename);
    }
    
    return employees;
}

void printReportHeader(const string& filename) {
    cout << "\n" << string(TABLE_WIDTH, '=') << endl;
    cout << "Report for file: " << filename << endl;
    cout << string(TABLE_WIDTH, '=') << endl;
    
    cout << left 
         << setw(ID_WIDTH) << "ID"
         << setw(NAME_WIDTH) << "Name"
         << setw(HOURS_WIDTH) << "Hours"
         << setw(SALARY_WIDTH) << "Salary" << endl;
    cout << string(TABLE_WIDTH, '-') << endl;
}

void printEmployeeReport(const Employee& emp, double hourlyRate) {
    double salary = emp.hours * hourlyRate;
    
    cout << left
         << setw(ID_WIDTH) << emp.num
         << setw(NAME_WIDTH) << emp.name
         << setw(HOURS_WIDTH) << fixed << setprecision(1) << emp.hours
         << "$" << fixed << setprecision(2) << salary << endl;
}

void writeReportToFile(const string& filename, const vector<Employee>& employees, 
                      double hourlyRate) {
    ofstream file(filename);
    
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create report file: " + filename);
    }
    
    file << "Report for file: test_data/test.bin" << endl;
    file << "ID, Name, Hours, Salary" << endl;
    
    for (const auto& emp : employees) {
        double salary = emp.hours * hourlyRate;
        file << emp.num << ", " << emp.name << ", " 
             << emp.hours << ", " << static_cast<int>(salary) << endl;
    }
    
    file.close();
}

int main(int argc, char* argv[]) {
    try {
        string binaryFile;
        string reportFile;
        double hourlyRate;

        if (argc == 4) {
            binaryFile = argv[1];
            reportFile = argv[2];
            hourlyRate = std::stod(argv[3]);
        } else {
            cout << "=== Report Generator ===" << endl;
            cout << "Enter binary file name: ";
            cin >> binaryFile;
            cout << "Enter report file name: ";
            cin >> reportFile;
            cout << "Enter hourly rate: ";
            cin >> hourlyRate;
        }

        auto employees = readEmployeesFromFile(binaryFile);
        
        if (employees.empty()) {
            cout << "Warning: No employee records found in file." << endl;
        }

        printReportHeader(binaryFile);
        
        for (const auto& emp : employees) {
            printEmployeeReport(emp, hourlyRate);
        }
        
        writeReportToFile(reportFile, employees, hourlyRate);
        
        cout << string(TABLE_WIDTH, '-') << endl;
        cout << "Report created successfully: " << reportFile << endl;
        
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
