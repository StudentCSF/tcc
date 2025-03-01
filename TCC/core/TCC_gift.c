#include <TCC/TCC_GIFT.h>


TCC_Money TCC_GiftProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_GIFT_TAX_RATE * profits;

    return res;
}