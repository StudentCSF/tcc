#include <TCC_pdf_pages.h>
#include <TCC_pdf_fonts.h>
#include <stdio.h>
#include <stdlib.h>

void TCC_PDF_WriteTextAt(TCC_PDF_Document *doc, float x, float y, const char *text) {
    if (!doc || !doc->page || !text) return;
    HPDF_Page_TextOut(doc->page, x, y, text);
}

void TCC_PDF_EndPage(TCC_PDF_Document *doc) {
    if (doc && doc->page) {
        HPDF_Page_EndText(doc->page);
    }
}

void TCC_PDF_CheckNewPage(TCC_PDF_Document *doc, float *y) {
    if (*y < 50.0f) {
        TCC_PDF_EndPage(doc);
        TCC_PDF_AddPageWithFont(doc, "Helvetica", 12);
        *y = doc->page_height - 50;
    }
}

int TCC_PDF_ProcessRowToPDF(TCC_PDF_Document *doc, xlsxioreadersheet sheet, float *y, float cell_padding) {
    if (!doc || !doc->pdf || !sheet || !y) {
        fprintf(stderr, "Ошибка: неверные параметры для обработки строки\n");
        return -1;
    }

    char *value;
    float x = 50.0f;

    while ((value = xlsxioread_sheet_next_cell(sheet)) != NULL) {
        if (doc->page == NULL) {
            fprintf(stderr, "Ошибка: страница PDF не инициализирована\n");
            free(value);
            return -1;
        }

        if (value[0] != '\0') {
            TCC_PDF_WriteTextAt(doc, x, *y, value);
        }
        x += cell_padding;
        free(value);
    }

    *y -= TCC_PDF_LineHeight();
    TCC_PDF_CheckNewPage(doc, y);
    return 0;
}

int TCC_PDF_ProcessSheetToPDF(TCC_PDF_Document *doc, xlsxioreader reader, float cell_padding) {
    if (!doc || !reader) {
        fprintf(stderr, "Ошибка: документ или ридер не инициализирован\n");
        return -1;
    }

    xlsxioreadersheet sheet = xlsxioread_sheet_open(reader, NULL, XLSXIOREAD_SKIP_EMPTY_ROWS);
    if (!sheet) {
        fprintf(stderr, "Ошибка открытия листа Excel\n");
        return -1;
    }

    float y = doc->page_height - 50.0f;

    while (xlsxioread_sheet_next_row(sheet)) {
        if (TCC_PDF_ProcessRowToPDF(doc, sheet, &y, cell_padding) != 0) {
            xlsxioread_sheet_close(sheet);
            return -1;
        }
    }

    xlsxioread_sheet_close(sheet);
    return 0;
}
