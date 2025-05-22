//
// Created by alett on 11.05.2025.
//

#ifndef BANK_API_H
#define BANK_API_H

#include <time.h>
#include <stddef.h>

typedef enum {
    TCC_bank_api_ok,
    TCC_bank_api_error_connection,
    TCC_bank_api_error_auth,
    TCC_bank_api_error_data,
    TCC_bank_api_error_no_bank,
    TCC_bank_api_error_not_implemented,
    TCC_bank_api_error_not_found,
    TCC_bank_api_error_exists,
    TCC_bank_api_error_memory
} TCC_bank_api_Status;

typedef struct {
    time_t date;
    double amount;
    char currency[4];  // ISO 4217
    char type[20];     // "salary", "transfer", "payment"
    char status[15];   // "completed", "pending", "failed"
} TCC_bank_Transaction;

typedef struct {
    char loan_type[30];   // "mortgage", "car", "personal"
    double interest_rate;
    time_t last_update;
} TCC_loan_interest_Rate;

typedef struct {
    time_t date;
    double amount;
    char source[50];      // "employer", "transfer", "gov_payment"
    char description[100];
} TCC_card_Credit;

typedef struct TCC_bank_Api {
    TCC_bank_api_Status (*get_transactions)(const char* employee_id, TCC_bank_Transaction** transactions, size_t* count);
    TCC_bank_api_Status (*get_inn)(const char* employee_id, char* inn, size_t buffer_size);
    TCC_bank_api_Status (*get_interest_rates)(const char* employee_id, TCC_loan_interest_Rate** rates, size_t* count);
    TCC_bank_api_Status (*get_card_credits)(const char* employee_id, TCC_card_Credit** credits, size_t* count);
} TCC_bank_Api;

#endif
