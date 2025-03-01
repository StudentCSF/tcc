#ifndef TCC__PRIT_H
#define TCC__PRIT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// ������ ��� ��� ���. ���
#define TCC_PRIT_IE_TAX_RATE 0.04f
// ������ ��� ��� ��. ���
#define TCC_PRIT_LE_TAX_RATE 0.06f
// ������� ���������� ������ ��� ���. ���
#define TCC_PRIT_IE_TAX_DEDUCTION 0.01f
// ������� ���������� ������ ��� ��. ���
#define TCC_PRIT_LE_TAX_DEDUCTION 0.02f
// ������������ �������� ���������� ������
#define TCC_PRIT_MAX_TAX_DEDUCTION 10000


TCC_Money TCC_PrIT_IE_ProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_PrIT_LE_ProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

