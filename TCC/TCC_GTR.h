#ifndef TCC__GTR_H
#define TCC__GTR_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Базовая ставка налога общего режима налогообложения
#define TCC_GTR_BASE_TAX_RATE 0.25f
// Ставка налога общего режима налогообложения (ОЭЗ
#define TCC_GTR_OEZ_TAX_RATE 0.15f
// Ставка налога общего режима налогообложения (ТОСЭР)
#define TCC_GTR_TOSER_TAX_RATE 0.05f


TCC_Money TCC_GTR_BaseProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_MoneyNotes* lossesHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_GTR_OezProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_MoneyNotes* lossesHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_GTR_ToserProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_MoneyNotes* lossesHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

