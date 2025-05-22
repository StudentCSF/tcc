//
// Created by alett on 12.05.2025.
//

#ifndef TCC_TBANK_H
#define TCC_TBANK_H

#include <TCC/bank/TCC_banks.h>

typedef struct {
    char digital_signature[50];
    char api_version[10];
} TCC_tbank_auth_context;

const TCC_bank_api* TCC_tbank_InitApi(const TCC_tbank_auth_context* auth);

#endif //TCC_TBANK_H
