#ifndef TCC__EXCEL_WRITER_H
#define TCC__EXCEL_WRITER_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <TCC/utils/TCC_error.h>
#include <TCC/excel/TCC_excel_file.h>
#include <stdint.h>

	typedef int_least8_t *TCC_Path;
    typedef int_least8_t *TCC_Format;

	TCC_Error TCC_excel_writer_Write(TCC_ExcelFile *pExcelFile, const TCC_Path path, const TCC_Format format);

#ifdef __cplusplus
}
#endif

#endif