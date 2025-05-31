#ifndef TCC_PDF_DOCUMENT_H
#define TCC_PDF_DOCUMENT_H

#include <TCC_pdf_common.h>

void TCC_PDF_HandleError(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);
TCC_PDF_Document TCC_PDF_InitDocument(void (*error_handler)(HPDF_STATUS, HPDF_STATUS, void *));
void TCC_PDF_FinalizeDocument(TCC_PDF_Document *doc, const char *filename);
int TCC_PDF_CreateFromText(const char *text, const char *pdf_file);
int TCC_PDF_CreatePdfFromXLSX(const char *xlsx_file, const char *pdf_file);

#endif