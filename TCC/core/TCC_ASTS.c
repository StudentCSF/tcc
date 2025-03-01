#include <TCC/TCC_ASTS.h>

TCC_Money TCC_ASTS_IP_profitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money potentialRes = TCC_ASTS_PROFITS_TAX_RATE * profits;
    const TCC_Money minTax = TCC_ASTS_PROFITS_MIN_TAX_RATE * profits;

    return potentialRes > minTax ? potentialRes : minTax;
}

TCC_Money TCC_ASTS_Org_profitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    if (profits > TCC_ASTS_ORG_PROFITS_MAX_IN_YEAR) {
        return 0;
    }

    const TCC_Money potentialRes = TCC_ASTS_PROFITS_TAX_RATE * profits;
    const TCC_Money minTax = TCC_ASTS_PROFITS_MIN_TAX_RATE * profits;

    return potentialRes > minTax ? potentialRes : minTax;
}

//УСН Доходы минус расходы
#define TCC_ASTS_PROFITS_MINUS_LOSSES_TAX_RATE 0.15f
//TODO Проверить минималку и её условия
#define TCC_ASTS_PROFITS_MINUS_LOSSES_MIN_TAX_RATE 0.01f
TCC_Money TCC_ASTSprofitsMinusLosesTax(const TCC_MoneyNotes* profitsHistoryPtr,
    const TCC_MoneyNotes* losesHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    if (profits > TCC_ASTS_ORG_PROFITS_MAX_IN_YEAR) {
        return 0;
    }

    const TCC_Money loses = TCC_moneyYearSum(profitsHistoryPtr, year);
    const TCC_Money potentialRes = TCC_ASTS_PROFITS_TAX_RATE * (profits - loses);
    const TCC_Money minTax = TCC_ASTS_PROFITS_MIN_TAX_RATE * profits;

    return potentialRes > minTax ? potentialRes : minTax;
}