//
//  Reporter.cpp
//  lab1_os
//

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "employee.h"

using namespace std;

bool compareEmployees(const employee& a, const employee& b) {
    return a.num < b.num;
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cerr << "Usage: Reporter <binary_file> <report_file> <hourly_rate>" << endl;
        return 1;
    }

    string binaryFile = argv[1];
    string reportFile = argv[2];
    double hourlyRate = stod(argv[3]);

    ifstream inFile(binaryFile, ios::binary);
    if (!inFile) {
        cerr << "Error opening binary file: " << binaryFile << endl;
        return 1;
    }

    vector<employee> employees;
    employee emp;
    
    while (inFile.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        employees.push_back(emp);
    }
    
    inFile.close();

    sort(employees.begin(), employees.end(), compareEmployees);

    ofstream outFile(reportFile);
    if (!outFile) {
        cerr << "Error creating report file: " << reportFile << endl;
        return 1;
    }

    // ВСЁ НА АНГЛИЙСКОМ!
    outFile << "Report for file: " << binaryFile << endl;
    outFile << "ID, Name, Hours, Salary" << endl;

    for (const auto& e : employees) {
        double salary = e.hours * hourlyRate;
        outFile << e.num << ", " << e.name << ", " << e.hours << ", " << salary << endl;
    }

    outFile.close();
    cout << "Report created successfully: " << reportFile << endl;

    return 0;
}
