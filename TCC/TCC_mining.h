#ifndef TCC__MINING_H
#define TCC__MINING_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/TCC_money.h>

// Ставка на добычу леч. грязи и мин. вод
#define TCC_MINING_DIRT_TAX_RATE 0.075f
// Ставка на добычу алмазов
#define TCC_MINING_DIAMOND_TAX_RATE 0.084f
// Ставка на добычу редких металлов
#define TCC_MINING_RARE_METAL_TAX_RATE 0.048f
// Ставка на добычу торфа и горючих сланцев
#define TCC_MINING_PEAT_TAX_RATE 0.04f
// Ставка на добычу радиоактивных металлов
#define TCC_MINING_RAD_TAX_RATE 0.055f


TCC_Money TCC_MiningDirtProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_MiningDiamondProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_MiningRareMetalProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_MiningPeatProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);
TCC_Money TCC_MiningRadProfitsTax(const TCC_MoneyNotes* profitsHistoryPtr, const TCC_DateYear year);


#ifdef __cplusplus
}
#endif


#endif

