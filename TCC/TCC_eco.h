#ifndef TCC__ECO_H
#define TCC__ECO_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка экологического налога в рублях за тонну выбросов
#define TCC_ECO_TAX_RATE 100


TCC_Money TCC_EcoProfitsTax(const TCC_MoneyNotes* tonsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

