//
// Created by alett on 16.05.2025.
//

#include <TCC/bank/TCC_tbank.h>
#include <TCC/bank/TCC_banks.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    TCC_bank_transaction* transactions;
    TCC_loan_interest_rate* rates;
    TCC_card_credit* credits;
    TCC_tbank_auth_context auth_ctx;
} TCC_tbank_cache;

static TCC_tbank_cache t_cache = {0};

static void TCC_demo_data_Init(const TCC_tbank_auth_context* auth) {
    memcpy(&t_cache.auth_ctx, auth, sizeof(TCC_tbank_auth_context));

    static TCC_bank_transaction tbank_transactions[] = {
        {.date = 1672531200, .amount = 75000.0, .currency = "RUB", .type = "salary", .status = "completed"},
        {.date = 1675209600, .amount = 150.5, .currency = "USD", .type = "cashback", .status = "completed"},
        {.date = 1677628800, .amount = 10000.0, .currency = "RUB", .type = "investment", .status = "pending"}
    };
    
    static TCC_loan_interest_rate tbank_rates[] = {
        {.loan_type = "consumer", .interest_rate = 8.9, .last_update = 1672531200},
        {.loan_type = "business", .interest_rate = 6.5, .last_update = 1675209600}
    };
    
    static TCC_card_credit tbank_credits[] = {
        {.date = 1672531200, .amount = 75000.0, .source = "employer", .description = "Monthly salary"},
        {.date = 1675209600, .amount = 150.5, .source = "cashback", .description = "Quarterly cashback"}
    };

    t_cache.transactions = malloc(sizeof(tbank_transactions));
    memcpy(t_cache.transactions, tbank_transactions, sizeof(tbank_transactions));

    t_cache.rates = malloc(sizeof(tbank_rates));
    memcpy(t_cache.rates, tbank_rates, sizeof(tbank_rates));

    t_cache.credits = malloc(sizeof(tbank_credits));
    memcpy(t_cache.credits, tbank_credits, sizeof(tbank_credits));
}

static TCC_bank_api_status TCC_tbank_GetTransactions(const char* employee_id, TCC_bank_transaction** transactions, size_t* count) {
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    *transactions = t_cache.transactions;
    *count = 3;
    return TCC_bank_api_ok;
}

static TCC_bank_api_status TCC_tbank_GetInn(const char* employee_id, char* inn, size_t buffer_size) {
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    if(buffer_size < 13) {
        return TCC_bank_api_error_data;
    }
    
    strncpy(inn, "7710140679", buffer_size);
    return TCC_bank_api_ok;
}

static TCC_bank_api_status TCC_tbank_GetInterestRates(const char* employee_id, TCC_loan_interest_rate** rates, size_t* count) {
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    *rates = t_cache.rates;
    *count = 2;
    return TCC_bank_api_ok;
}

static TCC_bank_api_status TCC_tbank_GetCardCredits(const char* employee_id, TCC_card_credit** credits, size_t* count) {
    if(strncmp(employee_id, "tbank_", 6) != 0) {
        return TCC_bank_api_error_auth;
    }

    *credits = t_cache.credits;
    *count = 2;
    return TCC_bank_api_ok;
}

static const TCC_bank_api tbank_api = {
    .get_transactions = TCC_tbank_GetTransactions,
    .get_inn = TCC_tbank_GetInn,
    .get_interest_rates = TCC_tbank_GetInterestRates,
    .get_card_credits = TCC_tbank_GetCardCredits
};

const TCC_bank_api* TCC_tbank_InitApi(const TCC_tbank_auth_context* auth) {
    if(strncmp(auth->digital_signature, "TBS-", 4) != 0) {
        return NULL;
    }
    
    TCC_demo_data_Init(auth);
    return &tbank_api;
}

void TCC_tbank_CleanupApi() {
    free(t_cache.transactions);
    free(t_cache.rates);
    free(t_cache.credits);
    memset(&t_cache, 0, sizeof(t_cache));
}
