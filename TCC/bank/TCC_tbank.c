//
// Created by alett on 16.05.2025.
//

#include <TCC/bank/TCC_tbank.h>
#include <TCC/bank/TCC_banks.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    TCC_bank_Transaction* transactions;
    TCC_loan_interest_Rate* rates;
    TCC_card_Credit* credits;
    TCC_tbank_auth_Context auth_ctx;
} TCC_tbank_Cache;

static TCC_tbank_Cache* get_tbank_cache() {
    static TCC_tbank_Cache t_cache = {0};
    return &t_cache;
}

static void TCC_demo_data_Init(const TCC_tbank_auth_Context* auth) {
    TCC_tbank_Cache* cache = get_tbank_cache();
    memcpy(&cache->auth_ctx, auth, sizeof(TCC_tbank_auth_Context));

    static TCC_bank_Transaction tbank_transactions[] = {
        {.date = 1672531200, .amount = 75000.0, .currency = "RUB", .type = "salary", .status = "completed"},
        {.date = 1675209600, .amount = 150.5, .currency = "USD", .type = "cashback", .status = "completed"},
        {.date = 1677628800, .amount = 10000.0, .currency = "RUB", .type = "investment", .status = "pending"}
    };
    
    static TCC_loan_interest_Rate tbank_rates[] = {
        {.loan_type = "consumer", .interest_rate = 8.9, .last_update = 1672531200},
        {.loan_type = "business", .interest_rate = 6.5, .last_update = 1675209600}
    };
    
    static TCC_card_Credit tbank_credits[] = {
        {.date = 1672531200, .amount = 75000.0, .source = "employer", .description = "Monthly salary"},
        {.date = 1675209600, .amount = 150.5, .source = "cashback", .description = "Quarterly cashback"}
    };

    cache->transactions = malloc(sizeof(tbank_transactions));
    memcpy(cache->transactions, tbank_transactions, sizeof(tbank_transactions));

    cache->rates = malloc(sizeof(tbank_rates));
    memcpy(cache->rates, tbank_rates, sizeof(tbank_rates));

    cache->credits = malloc(sizeof(tbank_credits));
    memcpy(cache->credits, tbank_credits, sizeof(tbank_credits));
}

static TCC_bank_api_Status TCC_tbank_GetTransactions(const char* employee_id, TCC_bank_Transaction** transactions, size_t* count) {
    TCC_tbank_Cache* cache = get_tbank_cache();
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    *transactions = cache->transactions;
    *count = 3;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_tbank_GetInn(const char* employee_id, char* inn, size_t buffer_size) {
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    if(buffer_size < 13) {
        return TCC_bank_api_error_data;
    }
    
    strncpy(inn, "7710140679", buffer_size);
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_tbank_GetInterestRates(const char* employee_id, TCC_loan_interest_Rate** rates, size_t* count) {
    TCC_tbank_Cache* cache = get_tbank_cache();
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    *rates = cache->rates;
    *count = 2;
    return TCC_bank_api_ok;
}

static TCC_bank_api_Status TCC_tbank_GetCardCredits(const char* employee_id, TCC_card_Credit** credits, size_t* count) {
    TCC_tbank_Cache* cache = get_tbank_cache();
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    *credits = cache->credits;
    *count = 2;
    return TCC_bank_api_ok;
}

static const TCC_bank_Api tbank_api = {
    .get_transactions = TCC_tbank_GetTransactions,
    .get_inn = TCC_tbank_GetInn,
    .get_interest_rates = TCC_tbank_GetInterestRates,
    .get_card_credits = TCC_tbank_GetCardCredits
};

const TCC_bank_Api* TCC_tbank_InitApi(const TCC_tbank_auth_Context* auth) {
    if(strncmp(auth->digital_signature, "TBS-", 4) != 0) {
        return NULL;
    }
    
    TCC_demo_data_Init(auth);
    return &tbank_api;
}

void TCC_tbank_CleanupApi() {
    TCC_tbank_Cache* cache = get_tbank_cache();
    free(cache->transactions);
    free(cache->rates);
    free(cache->credits);
    memset(cache, 0, sizeof(*cache));
}
