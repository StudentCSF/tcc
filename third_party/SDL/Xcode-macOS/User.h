//
//  user.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#ifndef USER_H
#define USER_H

#include <stdbool.h>

typedef enum { MALE, FEMALE, OTHER } Gender;

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char* street;
    char* city;
    char* postal_code;
} Address;

typedef struct {
    int id;
    char* username;
    char* email;
    int age;
    Date registration_date;
    bool is_active;
    float rating;
    double salary;
    Gender gender;
    Address address;
} User;

User Create_user(void);
void Free_user(User* user);

#endif
