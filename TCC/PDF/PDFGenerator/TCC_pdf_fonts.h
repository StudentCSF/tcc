#ifndef TCC_PDF_FONTS_H
#define TCC_PDF_FONTS_H

#include <TCC_pdf_common.h>

void TCC_PDF_AddPageWithFont(TCC_PDF_Document *doc, const char *font_name, float font_size);
float TCC_PDF_LineHeight(void);

#endif