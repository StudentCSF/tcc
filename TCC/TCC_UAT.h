#ifndef TCC__UAT_H
#define TCC__UAT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// —Ú‡‚Í‡ EC’Õ
#define TCC_UAT_TAX_RATE 0.06f


TCC_Money TCC_UATprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

