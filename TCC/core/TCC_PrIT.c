#include <TCC/TCC_PrIT.h>


TCC_Money TCC_PrIT_IE_ProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money potentionTaxDeduction = TCC_PRIT_IE_TAX_DEDUCTION * profits;

    const TCC_Money taxDedution = TCC_PRIT_MAX_TAX_DEDUCTION > potentionTaxDeduction ? potentionTaxDeduction : TCC_PRIT_MAX_TAX_DEDUCTION;

    const TCC_Money deductProfits = profits - taxDedution;

    const TCC_Money primaryTax = TCC_PRIT_IE_TAX_RATE * deductProfits;

    const TCC_Money deductTax = primaryTax - TCC_PRIT_MAX_TAX_DEDUCTION;

    const TCC_Money res = deductTax > 0 ? deductTax : primaryTax;

    return res;
}

TCC_Money TCC_PrIT_LE_ProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year)
{
    const TCC_Money profits = TCC_moneyYearSum(profitsHistoryPtr, year);

    const TCC_Money potentionTaxDeduction = TCC_PRIT_LE_TAX_DEDUCTION * profits;

    const TCC_Money taxDedution = TCC_PRIT_MAX_TAX_DEDUCTION > potentionTaxDeduction ? potentionTaxDeduction : TCC_PRIT_MAX_TAX_DEDUCTION;

    const TCC_Money deductProfits = profits - taxDedution;

    const TCC_Money primaryTax = TCC_PRIT_LE_TAX_RATE * deductProfits;

    const TCC_Money deductTax = primaryTax - TCC_PRIT_MAX_TAX_DEDUCTION;

    const TCC_Money res = deductTax > 0 ? deductTax : primaryTax;

    return res;
}