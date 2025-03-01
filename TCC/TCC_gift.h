#ifndef TCC__GIFT_H
#define TCC__GIFT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка налога на дарение
#define TCC_GIFT_TAX_RATE 0.13f


TCC_Money TCC_GiftProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

