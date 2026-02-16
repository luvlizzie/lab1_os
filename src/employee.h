#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <cstring>
#include <string>

static constexpr int MAX_NAME_LENGTH = 10;

struct Employee {
    int num;          
    char name[MAX_NAME_LENGTH];
    double hours;     
};

#endif
