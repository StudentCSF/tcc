#ifndef TCC__LAND_H
#define TCC__LAND_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// ќсновна€ ставка земельного налога
#define TCC_LAND_MAIN_TAX_RATE 0.03f
// —тавка земельного налога дл€ прочих строений
#define TCC_LAND_SPECIAL_TAX_RATE 0.015f


TCC_Money TCC_LandMainProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_LandSpecialProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

