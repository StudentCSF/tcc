//
// Created by alett on 12.05.2025.
//

#include <TCC/bank/TCC_sber.h>
#include <TCC/bank/TCC_banks.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    TCC_bank_Transaction* transactions;
    TCC_loan_interest_Rate* rates;
    TCC_card_Credit* credits;
} TCC_sber_Cache;

static TCC_sber_Cache* get_sber_cache() {
    static TCC_sber_Cache s_cache = {0};
    return &s_cache;
}

static void TCC_demo_data_Init() {
    TCC_sber_Cache* cache = get_sber_cache();
    
    static TCC_bank_Transaction sber_transactions[] = {
        {.date = 1672531200, .amount = 150000.0, .currency = "RUB", .type = "salary", .status = "completed"},
        {.date = 1675209600, .amount = 5000.0, .currency = "USD", .type = "bonus", .status = "completed"},
        {.date = 1677628800, .amount = 30000.0, .currency = "RUB", .type = "transfer", .status = "pending"}
    };

    static TCC_loan_interest_Rate sber_rates[] = {
        {.loan_type = "mortgage", .interest_rate = 7.8, .last_update = 1672531200},
        {.loan_type = "car", .interest_rate = 12.5, .last_update = 1675209600}
    };

    static TCC_card_Credit sber_credits[] = {
        {.date = 1672531200, .amount = 150000.0, .source = "employer", .description = "Salary January"},
        {.date = 1675209600, .amount = 5000.0, .source = "bonus", .description = "Q4 Performance Bonus"}
    };

    cache->transactions = malloc(sizeof(sber_transactions));
    memcpy(cache->transactions, sber_transactions, sizeof(sber_transactions));

    cache->rates = malloc(sizeof(sber_rates));
    memcpy(cache->rates, sber_rates, sizeof(sber_rates));

    cache->credits = malloc(sizeof(sber_credits));
    memcpy(cache->credits, sber_credits, sizeof(sber_credits));
}

static TCC_bank_api_Status TCC_sber_GetTransactions(const char* employee_id, TCC_bank_Transaction** transactions, size_t* count) {
    TCC_sber_Cache* cache = get_sber_cache();
    if(strncmp(employee_id, "sber_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *transactions = cache->transactions;
    *count = 3;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_sber_GetInn(const char* employee_id, char* inn, size_t buffer_size) {
    if(strncmp(employee_id, "sber_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    if(buffer_size < 13) {
        return TCC_bank_api_error_data;
    }

    strncpy(inn, "7707083893", buffer_size);
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_sber_GetInterestRates(const char* employee_id, TCC_loan_interest_Rate** rates, size_t* count) {
    TCC_sber_Cache* cache = get_sber_cache();
    if(strncmp(employee_id, "sber_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *rates = cache->rates;
    *count = 2;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_sber_GetCardCredits(const char* employee_id, TCC_card_Credit** credits, size_t* count) {
    TCC_sber_Cache* cache = get_sber_cache();
    if(strncmp(employee_id, "sber_", 5) != 0) {
        return TCC_bank_api_error_auth;
    }

    *credits = cache->credits;
    *count = 2;
    return TCC_bank_api_ok;
}

static const TCC_bank_Api sber_api = {
    .get_transactions = TCC_sber_GetTransactions,
    .get_inn = TCC_sber_GetInn,
    .get_interest_rates = TCC_sber_GetInterestRates,
    .get_card_credits = TCC_sber_GetCardCredits
};

const TCC_bank_Api* TCC_sber_InitApi() {
    TCC_demo_data_Init();
    return &sber_api;
}

void TCC_sber_CleanupApi() {
    TCC_sber_Cache* cache = get_sber_cache();
    free(cache->transactions);
    free(cache->rates);
    free(cache->credits);
    memset(cache, 0, sizeof(*cache));
}
