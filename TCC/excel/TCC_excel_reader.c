#include <TCC/excel/TCC_excel_reader.h>
#include <xlsxio_read.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

TCC_Error TCC_excel_reader_Read(TCC_ExcelFile *pExcelFile, int_least8_t *const path)
{
    xlsxioreader xlsxioRead;
    if ((xlsxioRead = xlsxioread_open(path)) == NULL)
    {
        return TCC_ERROR_EXCEL_OPEN_ERROR;
    }
    return TCC_excel_reader_ProcessFile(pExcelFile, xlsxioRead);
}

TCC_Error TCC_excel_reader_ReadFromMemory(TCC_ExcelFile *pExcelFile, const TCC_Path path)
{
    pExcelFile->path = path;
    xlsxioreader xlsxioRead;
    int fileHandle;
    char *pBuf = NULL;
    size_t bufLen = 0;
    if ((fileHandle = open(path, O_RDONLY | O_BINARY)) != -1)
    {
        struct stat fileInfo;
        if (fstat(fileHandle, &fileInfo) == 0)
        {
            if ((pBuf = malloc(fileInfo.st_size)) != NULL)
            {
                if (fileInfo.st_size > 0 && read(fileHandle, pBuf, fileInfo.st_size) == fileInfo.st_size)
                {
                    bufLen = fileInfo.st_size;
                }
            }
        }
        close(fileHandle);
    }
    if (!pBuf || bufLen == 0)
    {
        return TCC_ERROR_EXCEL_LOAD_ERROR;
    }
    if ((xlsxioRead = xlsxioread_open_memory(pBuf, bufLen, 1)) == NULL)
    {
        return TCC_ERROR_EXCEL_PROCESSING_ERROR;
    }
    return TCC_excel_reader_ProcessFile(pExcelFile, xlsxioRead);
}

TCC_Error TCC_excel_reader_ReadFromFileHandle(TCC_ExcelFile *pExcelFile, const TCC_Path path)
{
    xlsxioreader xlsxioRead;
    int fileHandle;
    if ((fileHandle = open(path, O_RDONLY | O_BINARY, 0)) == -1)
    {
        return TCC_ERROR_EXCEL_OPEN_ERROR;
    }
    if ((xlsxioRead = xlsxioread_open_filehandle(fileHandle)) == NULL)
    {
        return TCC_ERROR_EXCEL_PROCESSING_ERROR;
    }
    return TCC_excel_reader_ProcessFile(pExcelFile, xlsxioRead);
}

TCC_Error TCC_excel_reader_ProcessFile(TCC_ExcelFile *pExcelFile, xlsxioreader xlsxioRead)
{
    xlsxioreadersheetlist sheetlist;
    TCC_PageCount pageCount = 0;
    TCC_PageName *sheetNames = malloc(sizeof(char *) * 10);
    if ((sheetlist = xlsxioread_sheetlist_open(xlsxioRead)) == NULL)
    {
        printf("ERROR %s\n", "Can not read sheet list");
        return TCC_ERROR_EXCEL_PROCESSING_ERROR;
    } else {
        TCC_PageName sheetName;
        while ((sheetName = xlsxioread_sheetlist_next(sheetlist)) != NULL) {
            sheetNames[pageCount] = malloc(sizeof(char));
            strcpy(sheetNames[pageCount], sheetName);
            pageCount++;
        }
        xlsxioread_sheetlist_close(sheetlist);
        sheetlist = xlsxioread_sheetlist_open(xlsxioRead);
    }
    pExcelFile->pages = (TCC_ExcelPage*)malloc(sizeof(TCC_ExcelPage) * pageCount);
    pExcelFile->pageCount = 0;
    for (TCC_PageCount i = 0; i < pageCount; ++i)
    {
        pExcelFile->pages[i].name = sheetNames[i];
        xlsxioreadersheet sheet = xlsxioread_sheet_open(xlsxioRead, pExcelFile->pages[i].name, XLSXIOREAD_SKIP_ALL_EMPTY);
        (pExcelFile->pageCount)++;
        TCC_excel_reader_CreateRows(&(pExcelFile->pages[i]), &sheet);
        xlsxioread_sheet_close(sheet);
    }
    xlsxioread_sheetlist_close(sheetlist);
    xlsxioread_close(xlsxioRead);
    return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_reader_CreateHeaders(TCC_ExcelPage *pPage, xlsxioreadersheet *pSheet, TCC_ColCount colCount) {
    xlsxioread_sheet_next_row(*pSheet);
    pPage->headers = (TCC_ExcelCell*)malloc(sizeof(TCC_ExcelCell) * colCount);
    char *pValue;
    for (TCC_ColCount i = 0; i < colCount; ++i)
    {
        if ((pValue = xlsxioread_sheet_next_cell(*pSheet)) == NULL) {
            printf("ERROR %s\n", "No cells in header");
            return TCC_ERROR_EXCEL_PROCESSING_ERROR;
        }
        pPage->headers[i].value = pValue;
//        xlsxioread_free(value);
    }
    return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_reader_CreateRows(TCC_ExcelPage *pPage, xlsxioreadersheet *pSheet)
{
    // TODO считать строки и столбцы динамически
    TCC_RowCount rowCount = 3;
    TCC_ColCount colCount = 3;
    pPage->rows = (TCC_ExcelRow*)malloc(sizeof(TCC_ExcelRow) * rowCount);
    pPage->rowCount = 0;
    pPage->colCount = colCount;
    TCC_excel_reader_CreateHeaders(pPage, pSheet, colCount);
    for (TCC_RowCount i = 0; i < rowCount; ++i)
    {
        xlsxioread_sheet_next_row(*pSheet);
        pPage->rows[i].colCount = 0;
        pPage->rows[i].cells = (TCC_ExcelCell*)malloc(sizeof(TCC_ExcelCell) * colCount);
        pPage->rowCount++;
        TCC_excel_reader_CreateCellsToRow(&(pPage->rows[i]), pSheet, colCount);
    }
    return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_reader_CreateCellsToRow(TCC_ExcelRow *pRow, xlsxioreadersheet *pSheet, int colCount)
{
    char *value;
    for (TCC_ColCount i = 0; i < colCount; ++i)
    {
        if ((value = xlsxioread_sheet_next_cell(*pSheet)) == NULL) {
            printf("ERROR %s\n", "No cells in row");
            return TCC_ERROR_EXCEL_PROCESSING_ERROR;
        }
        pRow->cells[i].value = value;
        pRow->colCount++;
//        xlsxioread_free(value);
    }
    return TCC_ERROR_MISSING;
}

//TODO можно сделать запись по столбцам (исправить аналогично как по строкам)

TCC_Error TCC_excel_reader_CreateCol(TCC_ExcelPage *pPage, xlsxioreadersheet sheet)
{
    while (xlsxioread_sheet_next_row(sheet))
    {
        TCC_ExcelCol *pCol = (TCC_ExcelCol*)malloc(sizeof(TCC_ExcelCol));
        *(pPage->cols + pPage->colCount) = *pCol;
        pPage->colCount++;
        TCC_excel_reader_CreateCellToCol(pCol, sheet);
    }
    return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_reader_CreateCellToCol(TCC_ExcelCol *pCol, xlsxioreadersheet sheet)
{
    XLSXIOCHAR *value;
    while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL)
    {
        TCC_ExcelCell *pCell = (TCC_ExcelCell*)malloc(sizeof(TCC_ExcelCell));
        pCell->value = value;
        *(pCol->cells + pCol->rowCount) = *pCell;
        (pCol->rowCount)++;
        xlsxioread_free(value);
    }
    return TCC_ERROR_MISSING;
}