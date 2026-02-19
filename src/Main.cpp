#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <stdexcept>
#include "employee.h"

#ifdef _WIN32
#include <windows.h>
#endif

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::ifstream;
using std::to_string;

static constexpr int MIN_RECORDS = 1;
static constexpr int MAX_RECORDS = 1000;
static constexpr double MIN_RATE = 0.0;
static constexpr double MAX_RATE = 1000.0;

void clearScreen() {
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
}

int getValidatedInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(256, '\n');
            return value;
        }
        cerr << "Invalid input. Please enter a number between " 
             << min << " and " << max << endl;
        cin.clear();
        cin.ignore(256, '\n');
    }
}

double getValidatedDouble(const string& prompt, double min, double max) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            cin.ignore(256, '\n');
            return value;
        }
        cerr << "Invalid input. Please enter a number between " 
             << min << " and " << max << endl;
        cin.clear();
        cin.ignore(256, '\n');
    }
}

void printBinaryFile(const string& filename) {
    ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        cerr << "Error: Cannot open binary file: " << filename << endl;
        return;
    }
    
    cout << "\n" << string(50, '=') << endl;
    cout << "=== Binary File Contents: " << filename << " ===" << endl;
    cout << string(50, '=') << endl;
    
    Employee emp;
    int count = 0;
    while (file.read(reinterpret_cast<char*>(&emp), sizeof(Employee))) {
        cout << "Record #" << ++count << ":" << endl;
        cout << "  ID:    " << emp.num << endl;
        cout << "  Name:  " << emp.name << endl;
        cout << "  Hours: " << emp.hours << endl;
        cout << string(30, '-') << endl;
    }
    
    if (count == 0) {
        cout << "File is empty." << endl;
    } else {
        cout << "Total records: " << count << endl;
    }
    cout << string(50, '=') << "\n" << endl;
}

void printReportFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open report file: " << filename << endl;
        return;
    }
    
    cout << "\n" << string(50, '=') << endl;
    cout << "=== Report Contents: " << filename << " ===" << endl;
    cout << string(50, '=') << endl;
    
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    cout << string(50, '=') << "\n" << endl;
}

#ifdef _WIN32
int runCreator(const string& filename, int recordCount) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    string cmdLine = "Creator.exe " + filename + " " + to_string(recordCount);
    
    cout << "\nStarting Creator process..." << endl;
    cout << "Command: " << cmdLine << endl;
    
    if (!CreateProcess(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cerr << "Failed to create Creator process. Error: " << GetLastError() << endl;
        return 1;
    }
    
    cout << "Creator process started. PID: " << pi.dwProcessId << endl;
    cout << "Waiting for Creator to complete..." << endl;
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    cout << "Creator finished with exit code: " << exitCode << endl;
    return exitCode;
}

int runReporter(const string& filename, const string& reportFile, double hourlyRate) {
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    string cmdLine = "Reporter.exe " + filename + " " + reportFile + " " + to_string(hourlyRate);
    
    cout << "\nStarting Reporter process..." << endl;
    cout << "Command: " << cmdLine << endl;
    
    if (!CreateProcess(NULL, (LPSTR)cmdLine.c_str(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        cerr << "Failed to create Reporter process. Error: " << GetLastError() << endl;
        return 1;
    }
    
    cout << "Reporter process started. PID: " << pi.dwProcessId << endl;
    cout << "Waiting for Reporter to complete..." << endl;
    
    WaitForSingleObject(pi.hProcess, INFINITE);
    
    DWORD exitCode;
    GetExitCodeProcess(pi.hProcess, &exitCode);
    
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    
    cout << "Reporter finished with exit code: " << exitCode << endl;
    return exitCode;
}
#else
int runCreator(const string& filename, int recordCount) {
    string createCmd = "./Creator.exe " + filename + " " + to_string(recordCount);
    cout << "\nStarting Creator..." << endl;
    return system(createCmd.c_str());
}

int runReporter(const string& filename, const string& reportFile, double hourlyRate) {
    string reportCmd = "./Reporter.exe " + filename + " " + 
                       reportFile + " " + to_string(hourlyRate);
    cout << "\nStarting Reporter..." << endl;
    return system(reportCmd.c_str());
}
#endif

int main() {
    try {
        clearScreen();
        
        cout << "=== Employee Records System ===" << endl;
        cout << "Version 1.0" << endl;
        cout << string(30, '=') << endl;
        
        string filename;
        cout << "\nEnter filename for binary data: ";
        cin >> filename;
        cin.ignore(256, '\n');
        
        int recordCount = getValidatedInt("Enter number of employees: ", 
                                         MIN_RECORDS, MAX_RECORDS);
        
        int creatorResult = runCreator(filename, recordCount);
        
        if (creatorResult == 0) {
            cout << "\n✓ Creator finished successfully!" << endl;
            
            printBinaryFile(filename);
            
            string reportFile;
            cout << "Enter report file name: ";
            cin >> reportFile;
            cin.ignore(256, '\n');
            
            double hourlyRate = getValidatedDouble("Enter hourly rate: ", 
                                                  MIN_RATE, MAX_RATE);
            
            int reporterResult = runReporter(filename, reportFile, hourlyRate);
            
            if (reporterResult == 0) {
                cout << "\n✓ Reporter finished successfully!" << endl;
                
                printReportFile(reportFile);
                
                cout << "\n✓ Program completed successfully!" << endl;
            } else {
                cerr << "\n✗ Reporter failed with code: " << reporterResult << endl;
                return reporterResult;
            }
        } else {
            cerr << "\n✗ Creator failed with code: " << creatorResult << endl;
            return creatorResult;
        }
        
    } catch (const std::exception& e) {
        cerr << "Unexpected error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
