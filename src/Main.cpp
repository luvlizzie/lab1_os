//
//  Main.cpp
//  lab1_os
//

#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
#include "employee.h"

using namespace std;

void displayBinaryFile(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    employee emp;
    cout << "\nBinary file contents:" << endl;
    cout << "ID\tName\tHours" << endl;
    cout << "----------------------" << endl;
    
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(employee))) {
        cout << emp.num << "\t" << emp.name << "\t" << emp.hours << endl;
    }
    
    file.close();
}

void displayReport(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error opening report file: " << filename << endl;
        return;
    }

    cout << "\nReport contents:" << endl;
    cout << "================" << endl;
    
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    
    file.close();
}

int main() {
    string binaryFileName;
    int recordCount;
    
    cout << "Enter binary file name: ";
    cin >> binaryFileName;
    cout << "Enter number of records: ";
    cin >> recordCount;
    
    string creatorCmd = "Creator.exe " + binaryFileName + " " + to_string(recordCount);
    
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    if (!CreateProcess(NULL, (LPSTR)creatorCmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cerr << "CreateProcess failed for Creator. Error: " << GetLastError() << endl;
        return 1;
    }
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    displayBinaryFile(binaryFileName);
    
    string reportFileName;
    double hourlyRate;
    
    cout << "\nEnter report file name: ";
    cin >> reportFileName;
    cout << "Enter hourly rate: ";
    cin >> hourlyRate;
    
    string reporterCmd = "Reporter.exe " + binaryFileName + " " + reportFileName + " " + to_string(hourlyRate);
    
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    
    if (!CreateProcess(NULL, (LPSTR)reporterCmd.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cerr << "CreateProcess failed for Reporter. Error: " << GetLastError() << endl;
        return 1;
    }
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    displayReport(reportFileName);
    
    cout << "\nProgram completed successfully!" << endl;
    
    return 0;
}
