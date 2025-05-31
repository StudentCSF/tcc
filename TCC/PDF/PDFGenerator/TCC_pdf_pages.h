#ifndef TCC_PDF_PAGES_H
#define TCC_PDF_PAGES_H

#include <TCC_pdf_common.h>
#include <xlsxio_read.h>

void TCC_PDF_WriteTextAt(TCC_PDF_Document *doc, float x, float y, const char *text);
void TCC_PDF_EndPage(TCC_PDF_Document *doc);
void TCC_PDF_CheckNewPage(TCC_PDF_Document *doc, float *y);
int TCC_PDF_ProcessRowToPDF(TCC_PDF_Document *doc, xlsxioreadersheet sheet, float *y, float cell_padding);
int TCC_PDF_ProcessSheetToPDF(TCC_PDF_Document *doc, xlsxioreader reader, float cell_padding);

#endif