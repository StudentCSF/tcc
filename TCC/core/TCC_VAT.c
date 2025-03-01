#include <TCC/TCC_VAT.h>


TCC_Money TCC_VATprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);
    
    const TCC_Money taxRate = profits > TCC_VAT_MAX_YEAR_SUM_FOR_TAX_RATE ? TCC_VAT_LIGHT_TAX_RATE : TCC_VAT_TAX_RATE;

    const TCC_Money res = taxRate * profits / (1 + taxRate);

    return res;
}