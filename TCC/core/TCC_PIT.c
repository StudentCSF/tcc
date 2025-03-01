#include <TCC/TCC_PIT.h>


TCC_Money TCC_PITprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money taxRate = profits > TCC_PIT_MAX_YEAR_SUM_FOR_TAX_RATE ? TCC_PIT_STRONG_TAX_RATE : TCC_PIT_TAX_RATE;

    const TCC_Money res = taxRate * profits;

    return res;
}