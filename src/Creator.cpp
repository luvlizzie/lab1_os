//
//  Creator.cpp
//  lab1_os
//

#include <iostream>
#include <fstream>
#include <string>
#include "employee.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: Creator <binary_file_name> <records_count>" << endl;
        return 1;
    }

    string filename = argv[1];
    int recordCount = stoi(argv[2]);

    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Error creating file: " << filename << endl;
        return 1;
    }

    employee emp;
    cout << "Enter " << recordCount << " employees:" << endl;
    
    for (int i = 0; i < recordCount; i++) {
        cout << "\nEmployee #" << i + 1 << endl;
        cout << "ID: ";
        cin >> emp.num;
        
        cout << "Name (max 9 characters): ";
        cin >> emp.name;
        
        cout << "Hours worked: ";
        cin >> emp.hours;
        
        outFile.write(reinterpret_cast<char*>(&emp), sizeof(employee));
    }

    outFile.close();
    cout << "\nBinary file created successfully!" << endl;
    
    return 0;
}
