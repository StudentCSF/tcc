#include <TCC_pdf_fonts.h>
#include <stdio.h>

void TCC_PDF_AddPageWithFont(TCC_PDF_Document *doc, const char *font_name, float font_size) {

    if (!doc || !doc->pdf) return;

    doc->page = HPDF_AddPage(doc->pdf);
    HPDF_Page_SetSize(doc->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    doc->page_width = HPDF_Page_GetWidth(doc->page);
    doc->page_height = HPDF_Page_GetHeight(doc->page);
    doc->font = HPDF_GetFont(doc->pdf, font_name, NULL);
    HPDF_Page_SetFontAndSize(doc->page, doc->font, font_size);
    HPDF_Page_BeginText(doc->page);
}

float TCC_PDF_LineHeight(void) {
    return 18.0f;
}
