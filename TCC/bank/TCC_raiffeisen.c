//
// Created by alett on 16.05.2025.
//

#include <TCC/bank/TCC_banks.h>
#include <TCC/bank/TCC_raiffeisen.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    TCC_bank_Transaction* transactions;
    TCC_loan_interest_Rate* rates;
    TCC_card_Credit* credits;
     TCC_raiffeisen_employee_Info employee_info;
    char auth_token[64];
} TCC_raiffeisen_Cache;

static TCC_raiffeisen_Cache r_cache = {0};

static void TCC_demo_data_Init(const char* auth_token) {
    strncpy(r_cache.auth_token, auth_token, sizeof(r_cache.auth_token)-1);
    
    static TCC_bank_Transaction raif_transactions[] = {
        {.date = 1672531200, .amount = 120000.0, .currency = "RUB", .type = "salary", .status = "completed"},
        {.date = 1675209600, .amount = 2500.0, .currency = "EUR", .type = "bonus", .status = "completed"},
        {.date = 1677628800, .amount = 45000.0, .currency = "RUB", .type = "transfer", .status = "pending"}
    };
    
    static TCC_loan_interest_Rate raif_rates[] = {
        {.loan_type = "mortgage", .interest_rate = 6.5, .last_update = 1672531200},
        {.loan_type = "business_loan", .interest_rate = 8.2, .last_update = 1675209600}
    };
    
    static TCC_card_Credit raif_credits[] = {
        {.date = 1672531200, .amount = 120000.0, .source = "employer", .description = "Monthly salary"},
        {.date = 1675209600, .amount = 2500.0, .source = "bonus", .description = "Annual performance bonus"}
    };

    r_cache.employee_info = ( TCC_raiffeisen_employee_Info){
        .branch_code = "RAIF-MOS-001",
        .contract_date = 1640995200
    };

    r_cache.transactions = malloc(sizeof(raif_transactions));
    memcpy(r_cache.transactions, raif_transactions, sizeof(raif_transactions));

    r_cache.rates = malloc(sizeof(raif_rates));
    memcpy(r_cache.rates, raif_rates, sizeof(raif_rates));

    r_cache.credits = malloc(sizeof(raif_credits));
    memcpy(r_cache.credits, raif_credits, sizeof(raif_credits));
}

static TCC_bank_api_Status TCC_raif_GetTransactions(const char* employee_id, TCC_bank_Transaction** transactions, size_t* count) {
    if(strncmp(employee_id, "raif_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *transactions = r_cache.transactions;
    *count = 3;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_raif_GetInn(const char* employee_id, char* inn, size_t buffer_size) {
    if(strncmp(employee_id, "raif_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    if(buffer_size < 13) {
        return TCC_bank_api_error_data;
    }
    
    strncpy(inn, "7736207543", buffer_size);
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_raif_GetInterestRates(const char* employee_id, TCC_loan_interest_Rate** rates, size_t* count) {
    if(strncmp(employee_id, "raif_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *rates = r_cache.rates;
    *count = 2;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_raif_GetCardCredits(const char* employee_id, TCC_card_Credit** credits, size_t* count) {
    if(strncmp(employee_id, "raif_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *credits = r_cache.credits;
    *count = 2;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_raif_GetEmployeeInfo(const char* employee_id,  TCC_raiffeisen_employee_Info* info) {
    if(strncmp(employee_id, "raif_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *info = r_cache.employee_info;
    return TCC_bank_api_ok;
}

static const TCC_bank_Api raiffeisen_api = {
    .get_transactions = TCC_raif_GetTransactions,
    .get_inn = TCC_raif_GetInn,
    .get_interest_rates = TCC_raif_GetInterestRates,
    .get_card_credits = TCC_raif_GetCardCredits
};

const TCC_bank_Api* TCC_raiffeisen_InitApi(const char* auth_token) {
    if(auth_token == NULL || strncmp(auth_token, "RAIF-", 5) != 0) {
        return NULL;
    }
    
    TCC_demo_data_Init(auth_token);
    return &raiffeisen_api;
}

void TCC_raiffeisen_CleanupApi() {
    free(r_cache.transactions);
    free(r_cache.rates);
    free(r_cache.credits);
    memset(&r_cache, 0, sizeof(r_cache));
}

TCC_bank_api_Status TCC_raiffeisen_GetEmployeeInfo(const char* employee_id,  TCC_raiffeisen_employee_Info* info) {
    return TCC_raif_GetEmployeeInfo(employee_id, info);
}
