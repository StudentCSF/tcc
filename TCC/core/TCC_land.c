#include <TCC/TCC_land.h>


TCC_Money TCC_LandMainProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_LAND_MAIN_TAX_RATE * profits;

    return res;
}
TCC_Money TCC_LandSpecialProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_LAND_SPECIAL_TAX_RATE * profits;

    return res;
}