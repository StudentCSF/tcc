#ifndef TCC__SOCIAL_H
#define TCC__SOCIAL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка
#define TCC_SOCIAL_TAX_RATE 0.3f


TCC_Money TCC_SocialProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

