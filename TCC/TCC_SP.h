#ifndef TCC__SP_H
#define TCC__SP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка налога на продажу имущества
#define TCC_SP_TAX_RATE 0.13f
#define TCC_SP_DEDUCTION 1000000


TCC_Money TCC_SPprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

