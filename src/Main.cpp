#include <iostream>
#include <string>
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

int runCreator(const string& filename, int recordCount) {
    #ifdef _WIN32
    string createCmd = "Creator.exe " + filename + " " + to_string(recordCount);
    #else
    string createCmd = "./Creator.exe " + filename + " " + to_string(recordCount);
    #endif
    
    cout << "\nStarting Creator..." << endl;
    return system(createCmd.c_str());
}

int runReporter(const string& filename, const string& reportFile, double hourlyRate) {
    #ifdef _WIN32
    string reportCmd = "Reporter.exe " + filename + " " + 
                       reportFile + " " + to_string(hourlyRate);
    #else
    string reportCmd = "./Reporter.exe " + filename + " " + 
                       reportFile + " " + to_string(hourlyRate);
    #endif
    
    cout << "\nStarting Reporter..." << endl;
    return system(reportCmd.c_str());
}

int main() {
    try {
        clearScreen();
        
        cout << "=== Employee Records System ===" << endl;
        cout << "Version 1.0" << endl;
        cout << string(30, '=') << endl;
        
        string filename;
        cout << "Enter filename for binary data: ";
        cin >> filename;
        cin.ignore(256, '\n');
        
        int recordCount = getValidatedInt("Enter number of employees: ", 
                                         MIN_RECORDS, MAX_RECORDS);
        
        double hourlyRate = getValidatedDouble("Enter hourly rate: ", 
                                              MIN_RATE, MAX_RATE);
        
        string reportFile = "report_" + filename + ".txt";
        
        int creatorResult = runCreator(filename, recordCount);
        
        if (creatorResult == 0) {
            cout << "\nCreator finished successfully!" << endl;
            
            int reporterResult = runReporter(filename, reportFile, hourlyRate);
            
            if (reporterResult == 0) {
                cout << "\nReporter finished successfully!" << endl;
                cout << "Report saved to: " << reportFile << endl;
            } else {
                cerr << "\nReporter failed with code: " << reporterResult << endl;
                return reporterResult;
            }
        } else {
            cerr << "\nCreator failed with code: " << creatorResult << endl;
            return creatorResult;
        }
        
    } catch (const std::exception& e) {
        cerr << "Unexpected error: " << e.what() << endl;
        return 1;
    }
    
    return 0;
}
