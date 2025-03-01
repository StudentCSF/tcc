#ifndef TCC__VAT_H
#define TCC__VAT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// ������ ���
#define TCC_VAT_TAX_RATE 0.2f
// ������������ ��������� ����� �� ���, ��� �������� ��������� ������� ������ ���
#define TCC_VAT_MAX_YEAR_SUM_FOR_TAX_RATE 600000000
// �������� ���������� ������ ���
#define TCC_VAT_LIGHT_TAX_RATE 0.05f


TCC_Money TCC_VATprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

