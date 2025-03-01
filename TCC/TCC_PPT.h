#ifndef TCC__PPT_H
#define TCC__PPT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ѕазова€ ставка налога на имущество физ. лиц
#define TCC_PPT_BASE_TAX_RATE 0.001f
// —тавка налога на имущество физ. лиц из особого перечн€
#define TCC_PPT_SPECIAL_TAX_RATE 0.02f
// —тавка налога на имущество физ. лиц не из базового перечн€
#define TCC_PPT_OTHER_TAX_RATE 0.002f


TCC_Money TCC_PPT_BaseProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_PPT_SpecialProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_PPT_OtherProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

