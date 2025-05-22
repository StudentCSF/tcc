#ifndef TCC_USER_USER_H
#define TCC_USER_USER_H

#include <stdbool.h>
#include <stdlib.h>

typedef enum {
    TCC_kMales,
    TCC_kFemales,
    TCC_kOther
} TCC_user_Gender;

typedef struct {
    int TCC_kDay;
    int TCC_kMonth;
    int TCC_kYear;
} TCC_user_Date;

typedef struct {
    char* TCC_kStreet;
    char* TCC_kCity;
    char* TCC_kPostal_code;
} TCC_user_Address;

typedef struct {
    int id;
    char* TCC_kUsername;
    char* TCC_kEmail;
    int TCC_kAge;
    TCC_user_Date TCC_kRegistration_date;
    bool TCC_kIs_active;
    float TCC_kRating;
    double TCC_kSalary;
    TCC_user_Gender TCC_kGender;
    TCC_user_Address TCC_kAddress;
} TCC_user_User;

TCC_user_User TCC_user_CreateUser(void);
void TCC_user_FreeUser(TCC_user_User* user);

#endif
