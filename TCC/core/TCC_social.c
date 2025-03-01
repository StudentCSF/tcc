#include <TCC/TCC_SOCIAL.h>


TCC_Money TCC_SocialProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_SOCIAL_TAX_RATE * profits;

    return res;
}