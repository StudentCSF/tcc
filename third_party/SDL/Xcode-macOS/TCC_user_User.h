#ifndef TCC_USER_USER_H
#define TCC_USER_USER_H

#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    males,
    females,
    other
} TCC_user_Gender;

typedef struct {
    int TCC_kDay;
    int month;
    int year;
} TCC_user_Date;

typedef struct {
    char* street;
    char* city;
    char* postal_code;
} TCC_user_Address;

typedef struct {
    int id;
    char* username;
    char* email;
    int age;
    TCC_user_Date registration_date;
    bool is_active;
    float rating;
    double salary;
    TCC_user_Gender gender;
    TCC_user_Address address;
} TCC_user_User;

TCC_user_User TCC_user_CreateUser(void);
void TCC_user_FreeUser(TCC_user_User* user);

#endif
