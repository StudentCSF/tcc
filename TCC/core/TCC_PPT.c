#include <TCC/TCC_PPT.h>


TCC_Money TCC_PPT_BaseProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_PPT_BASE_TAX_RATE * profits;

    return res;
}
TCC_Money TCC_PPT_SpecialProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_PPT_SPECIAL_TAX_RATE * profits;

    return res;
}
TCC_Money TCC_PPT_OtherProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_PPT_OTHER_TAX_RATE * profits;

    return res;
}