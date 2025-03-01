#ifndef TCC__EXCISE_H
#define TCC__EXCISE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Акциз на литр водки
#define TCC_EXCISE_VODKA_TAX_RATE 58


TCC_Money TCC_ExciseProfitsTax(const TCC_MoneyNotes* vodkaVolumesPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

