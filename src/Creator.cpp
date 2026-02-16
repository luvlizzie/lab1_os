#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <stdexcept>
#include "../include/employee.h"

using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using std::string;
using std::ofstream;
using std::make_unique;

static constexpr int MIN_HOURS = 1;
static constexpr int MAX_HOURS = 168; 
static constexpr int BUFFER_SIZE = 256;

void clearInputStream() {
    cin.clear();
    cin.ignore(BUFFER_SIZE, '\n');
}

int getValidatedInt(const string& prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            clearInputStream();
            return value;
        }
        cerr << "Invalid input. Please enter a number between " 
             << min << " and " << max << endl;
        clearInputStream();
    }
}

double getValidatedDouble(const string& prompt, double min, double max) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= min && value <= max) {
            clearInputStream();
            return value;
        }
        cerr << "Invalid input. Please enter a number between " 
             << min << " and " << max << endl;
        clearInputStream();
    }
}

string getValidatedName(const string& prompt) {
    string name;
    while (true) {
        cout << prompt;
        cin >> name;
        clearInputStream();
        
        if (name.length() < MAX_NAME_LENGTH) {
            return name;
        }
        cerr << "Name too long. Maximum " << (MAX_NAME_LENGTH - 1) 
             << " characters." << endl;
    }
}

Employee inputEmployee(int id) {
    Employee emp{};
    emp.num = id;
    
    cout << "\nEmployee #" << id << endl;
    
    string name = getValidatedName("Name (max 9 chars): ");
    strncpy(emp.name, name.c_str(), MAX_NAME_LENGTH - 1);
    emp.name[MAX_NAME_LENGTH - 1] = '\0';
    
    emp.hours = getValidatedDouble("Hours worked: ", MIN_HOURS, MAX_HOURS);
    
    return emp;
}

bool writeEmployeeToFile(ofstream& file, const Employee& emp) {
    try {
        file.write(reinterpret_cast<const char*>(&emp), sizeof(Employee));
        return file.good();
    } catch (const std::exception& e) {
        cerr << "Error writing to file: " << e.what() << endl;
        return false;
    }
}

int main(int argc, char* argv[]) {
    try {
        string filename;
        int recordCount;

        if (argc == 3) {
            filename = argv[1];
            recordCount = std::stoi(argv[2]);
        } else {
            cout << "=== Employee Data Creator ===" << endl;
            cout << "Enter filename: ";
            cin >> filename;
            recordCount = getValidatedInt("Enter number of employees: ", 1, 1000);
        }

        ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            cerr << "Error: Cannot create file " << filename << endl;
            return 1;
        }

        cout << "\nEnter " << recordCount << " employees:" << endl;
        
        for (int i = 1; i <= recordCount; ++i) {
            auto emp = inputEmployee(i);
            
            if (!writeEmployeeToFile(file, emp)) {
                cerr << "Failed to write employee #" << i << endl;
                return 1;
            }
        }

        file.close();
        cout << "\nBinary file created successfully: " << filename << endl;
        
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
