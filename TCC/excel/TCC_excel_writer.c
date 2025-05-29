#include <TCC/excel/TCC_excel_writer.h>
#include <xlsxio_write.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>

TCC_Error TCC_excel_writer_Write(TCC_ExcelFile *pExcelFile, const TCC_Path path, const TCC_Format format)
{
    xlsxiowriter handle;
    for (TCC_PageCount i = 0; i < pExcelFile->pageCount; i++)
	{
        TCC_PageName name = pExcelFile->pages[i].name;
        char buff[100];
        snprintf(buff, 100, "%s_%s.%s", path, name, format);
		if ((handle = xlsxiowrite_open(buff, name)) == NULL)
		{
			return TCC_ERROR_EXCEL_WRITE_ERROR;
		}
		TCC_excel_writer_WritePage(&(pExcelFile->pages[i]), handle);
	}
	return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_writer_WritePage(TCC_ExcelPage *pExcelPage, xlsxiowriter handle)
{
    TCC_excel_writer_WriteHeaders(pExcelPage, handle);
	for (TCC_RowCount i = 0; i < pExcelPage->rowCount; i++)
	{
		TCC_excel_writer_WriteRow(&(pExcelPage->rows[i]), handle);
	}
	xlsxiowrite_close(handle);
	return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_writer_WriteHeaders(TCC_ExcelPage *pExcelPage, xlsxiowriter handle)
{
    xlsxiowrite_set_row_height(handle, 1);
	xlsxiowrite_set_detection_rows(handle, 10);
    for (TCC_ColCount i = 0; i < pExcelPage->colCount; i++)
	{
        xlsxiowrite_add_column(handle, pExcelPage->headers[i].value, 0);
    }
    xlsxiowrite_next_row(handle);
	return TCC_ERROR_MISSING;
}

TCC_Error TCC_excel_writer_WriteRow(TCC_ExcelRow *pExcelRow, xlsxiowriter handle)
{
	for (TCC_ColCount i = 0; i < pExcelRow->colCount; i++)
	{
		xlsxiowrite_add_cell_string(handle, pExcelRow->cells[i].value);
	}
    xlsxiowrite_next_row(handle);
    return TCC_ERROR_MISSING;
}
