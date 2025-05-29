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
    int day;
    int month;
    int year;
} TCC_user_Date;

typedef struct {
    char* street;
    char* city;
    char* postal_code;
} TCC_user_Address;

typedef struct {
    int id;                 // Уникальный идентификатор
    char* username;         // Имя пользователя
    char* email;            // Электронная почта
    int age;                // Возраст
    TCC_user_Date registration_date; // Дата регистрации
    bool is_active;         // Активен ли аккаунт
    float rating;           // Рейтинг пользователя
    double salary;          // Зарплата
    TCC_user_Gender gender;          // Пол
    TCC_user_Address address;        // Адрес проживания
} TCC_user_User;

TCC_user_User TCC_user_CreateUser(void) {
    return (TCC_user_User){
        .id = 1001,
        .username = strdup("Daniil Shmoylov"),
        .email = strdup("daniil.shmoylov@example.com"),
        .age = 30,
        .registration_date = {15, 5, 2020},
        .is_active = true,
        .rating = 4.7f,
        .salary = 45000.50,
        .gender = TCC_kMales,
        .address = {
            .street = strdup("123 Main St"),
            .city = strdup("New York"),
            .postal_code = strdup("10001")
        }
    };
}

void TCC_user_FreeUser(TCC_user_User* user) {
    free(user->username);
    free(user->email);
    free(user->address.street);
    free(user->address.city);
    free(user->address.postal_code);
}
