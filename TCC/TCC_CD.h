#ifndef TCC__CD_H
#define TCC__CD_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка таможенной пошлины
#define TCC_CD_TAX_RATE 0.1f


TCC_Money TCC_CDprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

