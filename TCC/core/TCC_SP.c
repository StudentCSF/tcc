#include <TCC/TCC_SP.h>


TCC_Money TCC_SPprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = (profits - TCC_SP_DEDUCTION) * TCC_SP_TAX_RATE;

    return res;
}