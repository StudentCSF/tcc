#ifndef TCC__EXCEL_CELL_H
#define TCC__EXCEL_CELL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

    typedef uint_least32_t TCC_RowCount;
    typedef uint_least32_t TCC_ColCount;

    typedef struct TCC_ExcelCell
    {
        TCC_RowCount row;
        TCC_ColCount col;
        void *value;
    } TCC_ExcelCell;

    void *TCC_excel_cell_Value(TCC_ExcelCell *pExcelCell);

#ifdef __cplusplus
}
#endif

#endif