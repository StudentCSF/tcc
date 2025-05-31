//
//  user.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
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
    int id;                 // Уникальный идентификатор
    char* TCC_kUsername;         // Имя пользователя
    char* TCC_kEmail;            // Электронная почта
    int TCC_kAge;                // Возраст
    TCC_user_Date TCC_kRegistration_date; // Дата регистрации
    bool TCC_kIs_active;         // Активен ли аккаунт
    float TCC_kRating;           // Рейтинг пользователя
    double TCC_kSalary;          // Зарплата
    TCC_user_Gender TCC_kGender;          // Пол
    TCC_user_Address TCC_kAddress;        // Адрес проживания
} TCC_user_User;

TCC_user_User TCC_user_CreateUser(void) {
    return (TCC_user_User){
        .id = 1001,
        .TCC_kUsername = strdup("Daniil Shmoylov"),
        .TCC_kEmail = strdup("daniil.shmoylov@example.com"),
        .TCC_kAge = 30,
        .TCC_kRegistration_date = {15, 5, 2020},
        .TCC_kIs_active = true,
        .TCC_kRating = 4.7f,
        .TCC_kSalary = 45000.50,
        .TCC_kGender =     TCC_kMales,
        .TCC_kAddress = {
            .TCC_kStreet = strdup("123 Main St"),
            .TCC_kCity = strdup("New York"),
            .TCC_kPostal_code = strdup("10001")
        }
    };
}

void TCC_user_FreeUser(TCC_user_User* user) {
    //TODO: - -> в этом разобраться
    free(user->TCC_kUsername);
    free(user->TCC_kEmail);
    free(user->TCC_kAddress.TCC_kStreet);
    free(user->TCC_kAddress.TCC_kCity);
    free(user->TCC_kAddress.TCC_kPostal_code);
}
