#ifndef TCC__EXCEL_FILE_H
#define TCC__EXCEL_FILE_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/excel/TCC_excel_page.h>
#include <stdint.h>

    typedef int_least8_t *TCC_Path;
    typedef int_least8_t *TCC_PageName;
    typedef uint_least16_t TCC_PageCount;

    typedef struct TCC_ExcelFile
    {
        TCC_Path path;
        TCC_PageCount pageCount;
        TCC_ExcelPage *pages;
    } TCC_ExcelFile;

    TCC_ExcelPage *TCC_excel_file_Page(TCC_ExcelFile *pExcelFile, const TCC_PageCount pageNum);

    TCC_ExcelPage *TCC_excel_file_PageByName(TCC_ExcelFile *pExcelFile, const TCC_PageName pageName);

#ifdef __cplusplus
}
#endif

#endif