#ifndef TCC__EXCEL_PAGE_H
#define TCC__EXCEL_PAGE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/excel/TCC_excel_row.h>
#include <TCC/excel/TCC_excel_col.h>
#include <TCC/excel/TCC_excel_cell.h>
#include <stdint.h>

    typedef int_least8_t *TCC_PageName;
    typedef uint_least32_t TCC_RowCount;
    typedef uint_least32_t TCC_ColCount;

    typedef struct TCC_ExcelPage
    {
        TCC_PageName name;
        TCC_ColCount colCount;
        TCC_RowCount rowCount;
        TCC_ExcelCell *headers;
        TCC_ExcelRow *rows;
        TCC_ExcelCol *cols;
    } TCC_ExcelPage;

    TCC_ExcelRow *TCC_excel_page_Row(TCC_ExcelPage *pExcelPage, const TCC_RowCount row);

    TCC_ExcelCol *TCC_excel_page_Col(TCC_ExcelPage *pExcelPage, const TCC_ColCount col);

    TCC_ExcelCol *TCC_excel_page_ColByHeader(TCC_ExcelPage *pExcelPage, const TCC_ExcelCell header);

    TCC_ExcelCell *TCC_excel_page_Cell(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row);

    void *TCC_excel_page_CellValue(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row);

    int TCC_excel_page_CellValueAsInt(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row);

    float TCC_excel_page_CellValueAsFloat(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row);

    char *TCC_excel_page_CellValueAsString(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row);

    int TCC_excel_page_CellValueAsBool(TCC_ExcelPage *pExcelPage, const TCC_ColCount col, const TCC_RowCount row);

#ifdef __cplusplus
}
#endif

#endif