#ifndef TCC__ASTS_H
#define TCC__ASTS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

//АУСН Доходы
#define TCC_ASTS_PROFITS_TAX_RATE 0.08f
//TODO проверить иинималку
#define TCC_ASTS_PROFITS_MIN_TAX_RATE 0.01f
//для применения АУСН годовой доход организации не должен превышать (для ИП нет ограничения)
#define TCC_ASTS_ORG_PROFITS_MAX_IN_YEAR 60000000

TCC_Money TCC_ASTS_IP_profitsTax(const TCC_MoneyNotes* profitsHistoryPtr,
    const TCC_DateYear year);
TCC_Money TCC_ASTS_Org_profitsTax(const TCC_MoneyNotes * profitsHistoryPtr, 
    const TCC_DateYear year);
TCC_Money TCC_ASTSprofitsMinusLosesTax(const TCC_MoneyNotes * profitsHistoryPtr, 
    const TCC_MoneyNotes* losesHistoryPtr, const TCC_DateYear year);



#ifdef __cplusplus
}
#endif


#endif