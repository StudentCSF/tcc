#include <TCC/TCC_EXCISE.h>


TCC_Money TCC_ExciseProfitsTax(const TCC_MoneyNotes* vodkaVolumesPtr, const TCC_DateYear year)
{
    const TCC_Money totalVolume = TCC_moneyYearSum(vodkaVolumesPtr, year);

    const TCC_Money res = TCC_EXCISE_VODKA_TAX_RATE * totalVolume;

    return res;
}