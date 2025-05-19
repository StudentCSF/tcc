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
    MALE,
    FEMALE,
    OTHER
} Gender;

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
    int id;                 // Уникальный идентификатор
    char* username;         // Имя пользователя
    char* email;            // Электронная почта
    int age;                // Возраст
    Date registration_date; // Дата регистрации
    bool is_active;         // Активен ли аккаунт
    float rating;           // Рейтинг пользователя
    double salary;          // Зарплата
    Gender gender;          // Пол
    Address address;        // Адрес проживания
} User;

// mock
User Create_user(void) {
    return (User){
        .id = 1001,
        .username = strdup("Daniil Shmoylov"),
        .email = strdup("daniil.shmoylov@example.com"),
        .age = 30,
        .registration_date = {15, 5, 2020},
        .is_active = true,
        .rating = 4.7f,
        .salary = 45000.50,
        .gender = MALE,
        .address = {
            .street = strdup("123 Main St"),
            .city = strdup("New York"),
            .postal_code = strdup("10001")
        }
    };
}

void Free_user(User* user) {
    free(user->username);
    free(user->email);
    free(user->address.street);
    free(user->address.city);
    free(user->address.postal_code);
}
