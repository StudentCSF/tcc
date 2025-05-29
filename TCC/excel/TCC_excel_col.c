#include <TCC/excel/TCC_excel_col.h>
#include <TCC/excel/TCC_excel_cell.h>

TCC_ExcelCell *TCC_excel_col_Cell(TCC_ExcelCol *pExcelCol, const TCC_RowCount row)
{
    TCC_ExcelCell *pCells = pExcelCol->cells;
    TCC_ExcelCell cell = *(pCells + row);
    return &cell;
}

void *TCC_excel_col_CellValue(TCC_ExcelCol *pExcelCol, const TCC_RowCount row)
{
    TCC_ExcelCell *pCell = TCC_excel_col_Cell(pExcelCol, row);
    void *pValue = TCC_excel_cell_Value(pCell);
    return pValue;
}
