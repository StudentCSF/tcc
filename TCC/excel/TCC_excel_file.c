#include <TCC/excel/TCC_excel_file.h>
#include <TCC/excel/TCC_excel_page.h>

TCC_ColCount TCC_excel_file_FindIndex(const TCC_ExcelPage *pPages, TCC_PageCount pageCount, TCC_PageName pageName)
{
    TCC_PageCount index = 0;
    TCC_ExcelPage pgi = *(pPages + index);
    while (index < pageCount && pgi.name != pageName)
        ++index;
    return (index == pageCount ? -1 : index);
}

TCC_ExcelPage *TCC_excel_file_Page(TCC_ExcelFile *pExcelFile, const TCC_PageCount pageNum)
{
    TCC_ExcelPage *pPages = pExcelFile->pages;
    TCC_ExcelPage page = *(pPages + pageNum);
    return &page;
}

TCC_ExcelPage *TCC_excel_file_PageByName(TCC_ExcelFile *pExcelFile, const TCC_PageName pageName)
{
    TCC_ExcelPage *pPages = pExcelFile->pages;
    TCC_PageCount pageCount = pExcelFile->pageCount;
    TCC_PageCount pageNum = TCC_excel_file_FindIndex(pPages, pageCount, pageName);
    TCC_ExcelPage *pPage = TCC_excel_file_Page(pExcelFile, pageNum);
    return pPage;
}
