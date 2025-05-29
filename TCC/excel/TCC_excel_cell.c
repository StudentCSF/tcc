#include <TCC/excel/TCC_excel_cell.h>

void *TCC_excel_cell_Value(TCC_ExcelCell *pExcelCell) {
    void *pValue = pExcelCell->value;
    return pValue;
}
