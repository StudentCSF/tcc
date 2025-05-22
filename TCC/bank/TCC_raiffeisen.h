//
// Created by alett on 12.05.2025.
//

#ifndef TCC_RAIFFAIZEN_H
#define TCC_RAIFFAIZEN_H

#include <TCC/bank/TCC_banks.h>

typedef struct {
    char branch_code[10];
    time_t contract_date;
} TCC_raiffeisen_employee_info;

TCC_bank_api_status TCC_raiffeisen_GetEmployeeInfo(const char* employee_id, TCC_raiffeisen_employee_info* info);

#endif //TCC_RAIFFAIZEN_H
