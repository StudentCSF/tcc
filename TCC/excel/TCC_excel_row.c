#include <TCC/excel/TCC_excel_row.h>
#include <TCC/excel/TCC_excel_cell.h>

TCC_ExcelCell *TCC_excel_row_Cell(TCC_ExcelRow *pExcelRow, const TCC_ColCount col)
{
    TCC_ExcelCell *pCells = pExcelRow->cells;
    TCC_ExcelCell cell = *(pCells + col);
    return &cell;
    
}
void *TCC_excel_row_CellValue(TCC_ExcelRow *pExcelRow, const TCC_ColCount col)
{
    TCC_ExcelCell *pCell = TCC_excel_row_Cell(pExcelRow, col);
    void *pValue = TCC_excel_cell_Value(pCell);
    return pValue;
}
