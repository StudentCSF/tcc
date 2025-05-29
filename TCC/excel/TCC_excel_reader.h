#ifndef TCC__EXCEL_READER_H
#define TCC__EXCEL_READER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/utils/TCC_error.h>
#include <TCC/excel/TCC_excel_file.h>
#include <stdint.h>

	typedef int_least8_t *TCC_Path;

	TCC_Error TCC_excel_reader_Read(TCC_ExcelFile *pExcelFile, const TCC_Path path);

	TCC_Error TCC_excel_reader_ReadFromMemory(TCC_ExcelFile *pExcelFile, const TCC_Path path);

	TCC_Error TCC_excel_reader_ReadFromFileHandle(TCC_ExcelFile *pExcelFile, const TCC_Path path);

#ifdef __cplusplus
}
#endif

#endif