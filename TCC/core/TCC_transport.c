#include <TCC/TCC_transport.h>


TCC_Money TCC_TransportTaxForMoreThan100(const TCC_MoneyNotes* autoPowerPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(autoPowerPtr, year);
    const TCC_Money res = TCC_TRANSPORT_STRONG_TAX_RATE * profits;

    return res;
}

TCC_Money TCC_TransportTaxForLessThan100(const TCC_MoneyNotes* autoPowerPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(autoPowerPtr, year);
    const TCC_Money res = TCC_TRANSPORT_TAX_RATE * profits;

    return res;
}