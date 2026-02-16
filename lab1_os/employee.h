//
//  employee.h
//  lab1_os
//

#ifndef employee_h
#define employee_h

#pragma pack(push, 1)
struct employee {
    int num;
    char name[10];
    double hours;
};
#pragma pack(pop)

#endif /* employee_h */
