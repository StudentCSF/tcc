#include <TCC/TCC_eco.h>


TCC_Money TCC_EcoProfitsTax(const TCC_MoneyNotes* tonsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(tonsHistoryPtr, year);

    const TCC_Money res = TCC_ECO_TAX_RATE * profits;

    return res;
}