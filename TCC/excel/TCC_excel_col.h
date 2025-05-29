#ifndef TCC__EXCEL_COL_H
#define TCC__EXCEL_COL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/excel/TCC_excel_cell.h>
#include <stdint.h>

    typedef uint_least32_t TCC_RowCount;
    typedef uint_least32_t TCC_ColCount;

    typedef struct TCC_ExcelCol
    {
        TCC_ExcelCell header;
        TCC_ColCount col;
        TCC_RowCount rowCount;
        TCC_ExcelCell *cells;
    } TCC_ExcelCol;

    TCC_ExcelCell *TCC_excel_col_Cell(TCC_ExcelCol *pExcelCol, const TCC_RowCount row);

    void *TCC_excel_col_CellValue(TCC_ExcelCol *pExcelCol, const TCC_RowCount row);

#ifdef __cplusplus
}
#endif

#endif