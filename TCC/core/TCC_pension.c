#include <TCC/TCC_PENSION.h>


TCC_Money TCC_PensionProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money taxRate = profits > TCC_PENSION_MAX_YEAR_SUM_FOR_TAX_RATE ? TCC_PENSION_LIGHT_TAX_RATE : TCC_PENSION_TAX_RATE;

    const TCC_Money res = taxRate * profits;

    return res;
}