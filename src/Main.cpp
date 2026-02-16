#include <iostream>
#include <string>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

int main() {
    string filename;
    int recordCount;
    double hourlyRate;

    cout << "=== Employee Records Program ===" << endl;
    
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    
    cout << "Enter filename: ";
    cin >> filename;
    
    cout << "Enter number of records: ";
    cin >> recordCount;
    
    cout << "Enter hourly rate: ";
    cin >> hourlyRate;
    
    #ifdef _WIN32
    string createCmd = "Creator.exe " + filename + " " + to_string(recordCount);
    string reportCmd = "Reporter.exe " + filename + " report.txt " + to_string(hourlyRate);
    #else
    string createCmd = "./Creator.exe " + filename + " " + to_string(recordCount);
    string reportCmd = "./Reporter.exe " + filename + " report.txt " + to_string(hourlyRate);
    #endif
    
    cout << "\nStarting Creator..." << endl;
    int result = system(createCmd.c_str());
    
    if (result == 0) {
        cout << "File successfully created!" << endl;
        cout << "\nStarting Reporter..." << endl;
        system(reportCmd.c_str());
        cout << "Report created in report.txt" << endl;
    } else {
        cerr << "Error creating file!" << endl;
    }
    
    return 0;
}
