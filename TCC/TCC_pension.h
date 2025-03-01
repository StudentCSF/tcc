#ifndef TCC__PENSION_H
#define TCC__PENSION_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// ������ ����
#define TCC_PENSION_TAX_RATE 0.22f
// ������������ ��������� ����� �� ���, ��� �������� ��������� ������� ������ ����
#define TCC_PENSION_MAX_YEAR_SUM_FOR_TAX_RATE 1917000
// �������� ���������� ������ ����
#define TCC_PENSION_LIGHT_TAX_RATE 0.151f


TCC_Money TCC_PensionProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

