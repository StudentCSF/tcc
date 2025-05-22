//
// Created by alett on 11.05.2025.
//

#include <TCC/bank/TCC_banks.h>
#include <stdlib.h>
#include <string.h>

typedef struct TCC_bank_Entry {
    const char* bank_name;
    const TCC_bank_Api* api;
    void* context;
    struct TCC_bank_Entry* next;
} TCC_bank_Entry;

typedef struct {
    TCC_bank_Entry* banks;
    const TCC_bank_Api* active_bank;
    void* active_context;
} TCC_BankSystem;

static TCC_BankSystem* get_bank_system() {
    static TCC_BankSystem bank_system = {0};
    return &bank_system;
}

static TCC_bank_Entry* TCC_FindBankEntry(const char* bank_name) {
    TCC_BankSystem* system = get_bank_system();
    TCC_bank_Entry* entry = system->banks;
    while (entry != NULL) {
        if (strcmp(entry->bank_name, bank_name) == 0) {
            return entry;
        }
        entry = entry->next;
    }
    return NULL;
}

TCC_bank_api_Status TCC_RegisterBank(const char* bank_name, const TCC_bank_Api* api, void* init_context) {
    if (bank_name == NULL || api == NULL) {
        return TCC_bank_api_error_data;
    }

    if (TCC_FindBankEntry(bank_name) != NULL) {
        return TCC_bank_api_error_exists;
    }

    TCC_bank_Entry* new_entry = malloc(sizeof(TCC_bank_Entry));
    if (!new_entry) {
        return TCC_bank_api_error_memory;
    }

    TCC_BankSystem* system = get_bank_system();
    new_entry->bank_name = strdup(bank_name);
    new_entry->api = api;
    new_entry->context = init_context;
    new_entry->next = system->banks;
    system->banks = new_entry;

    return TCC_bank_api_ok;
}

TCC_bank_api_Status TCC_UnregisterBank(const char* bank_name) {
    TCC_BankSystem* system = get_bank_system();
    TCC_bank_Entry** ptr = &system->banks;
    while (*ptr != NULL) {
        TCC_bank_Entry* entry = *ptr;
        if (strcmp(entry->bank_name, bank_name) == 0) {
            *ptr = entry->next;
            free((void*)entry->bank_name);
            free(entry);
            return TCC_bank_api_ok;
        }
        ptr = &entry->next;
    }
    return TCC_bank_api_error_not_found;
}

TCC_bank_api_Status TCC_SetActiveBank(const char* bank_name) {
    TCC_bank_Entry* entry = TCC_FindBankEntry(bank_name);
    if (entry == NULL) {
        return TCC_bank_api_error_not_found;
    }

    TCC_BankSystem* system = get_bank_system();
    system->active_bank = entry->api;
    system->active_context = entry->context;
    return TCC_bank_api_ok;
}

const char* TCC_GetActiveBankName() {
    TCC_BankSystem* system = get_bank_system();
    TCC_bank_Entry* entry = system->banks;
    while (entry != NULL) {
        if (entry->api == system->active_bank) {
            return entry->bank_name;
        }
        entry = entry->next;
    }
    return NULL;
}

TCC_bank_api_Status TCC_GetTransactions(const char* employee_id, TCC_bank_Transaction** transactions, size_t* count) {
    TCC_BankSystem* system = get_bank_system();
    if (system->active_bank == NULL) {
        return TCC_bank_api_error_no_bank;
    }
    
    if (system->active_bank->get_transactions == NULL) {
        return TCC_bank_api_error_not_implemented;
    }

    return system->active_bank->get_transactions(
        employee_id, 
        transactions, 
        count
    );
}

TCC_bank_api_Status TCC_GetInn(const char* employee_id, char* inn_buffer, size_t buffer_size) {
    TCC_BankSystem* system = get_bank_system();
    if (system->active_bank == NULL) {
        return TCC_bank_api_error_no_bank;
    }

    if (system->active_bank->get_inn == NULL) {
        return TCC_bank_api_error_not_implemented;
    }

    return system->active_bank->get_inn(
        employee_id, 
        inn_buffer, 
        buffer_size
    );
}

TCC_bank_api_Status TCC_GetInterestRates(const char* employee_id, TCC_loan_interest_Rate** rates, size_t* count) {
    TCC_BankSystem* system = get_bank_system();
    if (system->active_bank == NULL) {
        return TCC_bank_api_error_no_bank;
    }

    if (system->active_bank->get_interest_rates == NULL) {
        return TCC_bank_api_error_not_implemented;
    }

    return system->active_bank->get_interest_rates(
        employee_id, 
        rates, 
        count
    );
}

TCC_bank_api_Status TCC_GetCardCredits(const char* employee_id, TCC_card_Credit** credits, size_t* count) {
    TCC_BankSystem* system = get_bank_system();
    if (system->active_bank == NULL) {
        return TCC_bank_api_error_no_bank;
    }

    if (system->active_bank->get_card_credits == NULL) {
        return TCC_bank_api_error_not_implemented;
    }

    return system->active_bank->get_card_credits(
        employee_id, 
        credits, 
        count
    );
}

void TCC_CleanupAll() {
    TCC_BankSystem* system = get_bank_system();
    TCC_bank_Entry* entry = system->banks;
    while (entry != NULL) {
        TCC_bank_Entry* next = entry->next;
        free((void*)entry->bank_name);
        free(entry);
        entry = next;
    }
    
    system->banks = NULL;
    system->active_bank = NULL;
    system->active_context = NULL;
}