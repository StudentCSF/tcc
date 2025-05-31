#ifndef TCC_PDF_COMMON_H
#define TCC_PDF_COMMON_H

#include <hpdf.h>
#include <xlsxio_read.h>

typedef struct {
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Font font;
    float page_width;
    float page_height;
} TCC_PDF_Document;

#endif
