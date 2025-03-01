#include <TCC/TCC_mining.h>


TCC_Money TCC_MiningDirtProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_MINING_DIRT_TAX_RATE * profits;

    return res;
}

TCC_Money TCC_MiningDiamondProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_MINING_DIAMOND_TAX_RATE * profits;

    return res;
}

TCC_Money TCC_MiningRareMetalProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_MINING_RARE_METAL_TAX_RATE * profits;

    return res;
}

TCC_Money TCC_MiningPeatProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_MINING_PEAT_TAX_RATE * profits;

    return res;
}

TCC_Money TCC_MiningRadProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money res = TCC_MINING_RAD_TAX_RATE * profits;

    return res;
}