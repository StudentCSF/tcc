#ifndef TCC__STS_H
#define TCC__STS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

//УСН Доходы
#define TCC_STS_PROFITS_TAX_RATE 0.06f
//УСН Доходы минус расходы
#define TCC_STS_PROFITS_MINUS_LOSSES_TAX_RATE 0.15f
#define TCC_STS_PROFITS_MIN_TAX_RATE 0.01f


TCC_Money TCC_STSprofitsTax(const TCC_MoneyNotes * profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_STSprofitsMinusLossesTax(const TCC_MoneyNotes* profitsHistoryPtr,
    const TCC_MoneyNotes* losesHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

