#include <TCC/TCC_CD.h>


TCC_Money TCC_CDprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_CD_TAX_RATE * profits;

    return res;
}