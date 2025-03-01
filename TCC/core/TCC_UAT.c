#include <TCC/TCC_VAT.h>


TCC_Money TCC_UATprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money_f res = TCC_UAT_TAX_RATE * profits;

    return res;
}