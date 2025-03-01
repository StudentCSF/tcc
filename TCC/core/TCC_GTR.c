#include <TCC/TCC_GTR.h>


TCC_Money TCC_GTR_BaseProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_MoneyNotes* lossesHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money losses = TCC_moneyYearSum(lossesHistoryPtr, year);

    const TCC_Money res = TCC_GTR_BASE_TAX_RATE * (profits - losses);

    return res;
}
TCC_Money TCC_GTR_OezProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_MoneyNotes* lossesHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money losses = TCC_moneyYearSum(lossesHistoryPtr, year);

    const TCC_Money res = TCC_GTR_OEZ_TAX_RATE * (profits - losses);

    return res;
}
TCC_Money TCC_GTR_ToserProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_MoneyNotes* lossesHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money losses = TCC_moneyYearSum(lossesHistoryPtr, year);

    const TCC_Money res = TCC_GTR_TOSER_TAX_RATE * (profits - losses);

    return res;
}