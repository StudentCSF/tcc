#ifndef TCC__EXCEL_ROW_H
#define TCC__EXCEL_ROW_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/excel/TCC_excel_cell.h>
#include <stdint.h>

    typedef uint_least32_t TCC_RowCount;
    typedef uint_least32_t TCC_ColCount;

    typedef struct TCC_ExcelRow
    {
        TCC_ColCount colCount;
        TCC_RowCount row;
        TCC_ExcelCell *cells;
    } TCC_ExcelRow;

    TCC_ExcelCell *TCC_excel_row_Cell(TCC_ExcelRow *pExcelRow, const TCC_ColCount col);

    void *TCC_excel_row_CellValue(TCC_ExcelRow *pExcelRow, const TCC_ColCount col);

#ifdef __cplusplus
}
#endif

#endif 