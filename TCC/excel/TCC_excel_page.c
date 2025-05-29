#include <TCC/excel/TCC_excel_page.h>
#include <TCC/excel/TCC_excel_row.h>
#include <TCC/excel/TCC_excel_col.h>
#include <TCC/excel/TCC_excel_cell.h>

TCC_ColCount TCC_excel_page_FindIndex(const TCC_ExcelCell *pHeaders, TCC_ColCount colCount, TCC_ExcelCell header)
{
    TCC_ColCount index = 0;
    TCC_ExcelCell hdi = *(pHeaders + index);
    while (index < colCount && hdi.value != header.value)
        ++index;
    return (index == colCount ? -1 : index);
}

TCC_ExcelRow *TCC_excel_page_Row(TCC_ExcelPage *pExcelPage, const TCC_RowCount row)
{
    TCC_ExcelRow *pExcelRows = pExcelPage->rows;
    TCC_ExcelRow excelRow = *(pExcelRows + row);
    return &excelRow;
}

TCC_ExcelCol *TCC_excel_page_Col(TCC_ExcelPage *pExcelPage, const TCC_ColCount col)
{
    TCC_ExcelCol *pExcelCols = pExcelPage->cols;
    TCC_ExcelCol excelCol = *(pExcelCols + col);
    return &excelCol;
}

TCC_ExcelCol *TCC_excel_page_ColByHeader(TCC_ExcelPage *pExcelPage, const TCC_ExcelCell header)
{
    TCC_ExcelCell *pHeaders = pExcelPage->headers;
    TCC_ColCount colCount = pExcelPage->colCount;
    TCC_ColCount col = TCC_excel_page_FindIndex(pHeaders, colCount, header);
    TCC_ExcelCol *pExcelCol = TCC_excel_page_Col(pExcelPage, col);
    return pExcelCol;
}

TCC_ExcelCell *TCC_excel_page_Cell(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row)
{
    TCC_ExcelRow *pExcelRow = TCC_excel_page_Row(pExcelPage, row);
    TCC_ExcelCell *pExcelCell = TCC_excel_row_Cell(pExcelRow, col);
    return pExcelCell;
}

void *TCC_excel_page_CellValue(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row)
{
    TCC_ExcelCell *pExcelCell = TCC_excel_page_Cell(pExcelPage, col, row);
    void *pValue = TCC_excel_cell_Value(pExcelCell);
    return pValue;
}

int TCC_excel_page_CellValueAsInt(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row)
{
    void *pValue = TCC_excel_page_CellValue(pExcelPage, col, row);
    int intValue = *((int *)pValue);
    return intValue;
}

float TCC_excel_page_CellValueAsFloat(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row)
{
    void *pValue = TCC_excel_page_CellValue(pExcelPage, col, row);
    float floatValue = (float)*((int *)pValue);
    return floatValue;
}

char *TCC_excel_page_CellValueAsString(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row)
{
    void *pValue = TCC_excel_page_CellValue(pExcelPage, col, row);
    char *pStringValue = (char *)pValue;
    return pStringValue;
}

int TCC_excel_page_CellValueAsBool(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row)
{
    void *pValue = TCC_excel_page_CellValue(pExcelPage, col, row);
    int intValue = *((int *)pValue);
    int boolValue = (intValue > 0) ? 1 : 0;
    return boolValue;
}
