#ifndef TCC__TRANSPORT_H
#define TCC__TRANSPORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка (рублей на 1 л.с.) для авто с <= 100 л.с.
#define TCC_TRANSPORT_TAX_RATE 3.5f
// Ставка (рублей на 1 л.с.) для авто с > 100 л.с.
#define TCC_TRANSPORT_STRONG_TAX_RATE 5.0f


TCC_Money TCC_TransportTaxForMoreThan100(const TCC_MoneyNotes* autoPowerPtr, const TCC_DateYear year);
TCC_Money TCC_TransportTaxForLessThan100(const TCC_MoneyNotes* autoPowerPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

