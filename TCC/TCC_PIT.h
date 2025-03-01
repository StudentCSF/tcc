#ifndef TCC__PIT_H
#define TCC__PIT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// ������ ����
#define TCC_PIT_TAX_RATE 0.13f
// ������������ ��������� ����� �� ���, ��� �������� ��������� ������� ������ ����
#define TCC_PIT_MAX_YEAR_SUM_FOR_TAX_RATE 2400000
// �������� ���������� ������ ����
#define TCC_PIT_STRONG_TAX_RATE 0.22f


TCC_Money TCC_PITprofitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

