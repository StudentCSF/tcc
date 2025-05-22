#include <pdf_generator.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xlsxio_read.h>


TCC_pdfgen_HandleError
void handle_pdf_error(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    fprintf(stderr, "PDF ERROR: error_no=0x%04X, detail_no=%u\n", (unsigned)error_no, (unsigned)detail_no);
    exit(1);
}

PdfDocument pdf_init_document(void (*error_handler)(HPDF_STATUS, HPDF_STATUS, void *)) {
    PdfDocument doc;
    doc.pdf = HPDF_New(error_handler, NULL);
    if (!doc.pdf) {
        fprintf(stderr, "Ошибка создания PDF документа\n");
        exit(1);
    }
    doc.page = NULL;
    doc.font = NULL;
    doc.page_width = 0;
    doc.page_height = 0;
    return doc;
}

void pdf_finalize_document(PdfDocument *doc, const char *filename) {
    if (!doc || !doc->pdf) return;

    if (HPDF_SaveToFile(doc->pdf, filename) != HPDF_OK) {
        fprintf(stderr, "Ошибка сохранения PDF файла: %s\n", filename);
    }
    HPDF_Free(doc->pdf);
    doc->pdf = NULL;
}

void pdf_add_page_with_font(PdfDocument *doc, const char *font_name, float font_size) {
    if (!doc || !doc->pdf) return;

    doc->page = HPDF_AddPage(doc->pdf);
    HPDF_Page_SetSize(doc->page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
    doc->page_width = HPDF_Page_GetWidth(doc->page);
    doc->page_height = HPDF_Page_GetHeight(doc->page);
    doc->font = HPDF_GetFont(doc->pdf, font_name, NULL);
    HPDF_Page_SetFontAndSize(doc->page, doc->font, font_size);
    HPDF_Page_BeginText(doc->page);
}

void pdf_write_text_at(PdfDocument *doc, float x, float y, const char *text) {
    if (!doc || !doc->page || !text) return;
    HPDF_Page_TextOut(doc->page, x, y, text);
}

void pdf_end_page(PdfDocument *doc) {
    if (doc && doc->page) {
        HPDF_Page_EndText(doc->page);
    }
}

float pdf_line_height(void) {
    return 18.0f;
}

void pdf_check_new_page(PdfDocument *doc, float *y) {
    if (*y < 50.0f) {
        pdf_end_page(doc);
        pdf_add_page_with_font(doc, "Helvetica", 12);
        *y = doc->page_height - 50;
    }
}

int create_pdf_from_text(const char *text, const char *pdf_file) {
    if (!text || !pdf_file) {
        fprintf(stderr, "Ошибка: текст или имя PDF-файла не указаны\n");
        return -1;
    }

    PdfDocument doc = pdf_init_document(handle_pdf_error);
    if (!doc.pdf) {
        fprintf(stderr, "Ошибка: не удалось инициализировать PDF-документ\n");
        return -1;
    }

    pdf_add_page_with_font(&doc, "Helvetica", 12);
    if (!doc.page || !doc.font) {
        fprintf(stderr, "Ошибка: не удалось создать страницу или шрифт\n");
        HPDF_Free(doc.pdf);
        return -1;
    }

    float x = 50.0f;
    float y = doc.page_height - 50.0f;
    pdf_write_text_at(&doc, x, y, text);
    pdf_end_page(&doc);

    // Попытка сохранить файл
    HPDF_STATUS status = HPDF_SaveToFile(doc.pdf, pdf_file);
    if (status != HPDF_OK) {
        fprintf(stderr, "Ошибка сохранения PDF файла: %s\n", pdf_file);
        HPDF_Free(doc.pdf);
        return -1;
    }

    pdf_finalize_document(&doc, pdf_file);

    printf("PDF создан: %s\n", pdf_file);
    return 0;
}


xlsxioreader open_xlsx_reader(const char *xlsx_file) {
    if (!xlsx_file) {
        fprintf(stderr, "Ошибка: путь к Excel файлу не задан\n");
        return NULL;
    }

    xlsxioreader reader = xlsxioread_open(xlsx_file);
    if (!reader) {
        fprintf(stderr, "Ошибка открытия Excel файла: %s\n", xlsx_file);
    }

    return reader;
}


int process_row_to_pdf(PdfDocument *doc, xlsxioreadersheet sheet, float *y, float cell_padding) {
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
            pdf_write_text_at(doc, x, *y, value);
        }
        x += cell_padding;
        free(value);
    }

    *y -= pdf_line_height();
    pdf_check_new_page(doc, y);
    return 0;
}

int process_sheet_to_pdf(PdfDocument *doc, xlsxioreader reader, float cell_padding) {
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
        if (process_row_to_pdf(doc, sheet, &y, cell_padding) != 0) {
            xlsxioread_sheet_close(sheet);
            return -1;
        }
    }

    xlsxioread_sheet_close(sheet);
    return 0;
}

int create_pdf_from_xlsx(const char *xlsx_file, const char *pdf_file) {
    if (!xlsx_file || !pdf_file) {
        fprintf(stderr, "Ошибка: не указаны имена входного или выходного файлов\n");
        return -1;
    }

    xlsxioreader reader = open_xlsx_reader(xlsx_file);
    if (!reader) return -1;

    PdfDocument doc = pdf_init_document(handle_pdf_error);
    if (!doc.pdf) {
        xlsxioread_close(reader);
        return -1;
    }

    pdf_add_page_with_font(&doc, "Helvetica", 12);
    if (!doc.page || !doc.font) {
        fprintf(stderr, "Ошибка: не удалось создать страницу или шрифт\n");
        HPDF_Free(doc.pdf);
        xlsxioread_close(reader);
        return -1;
    }

    if (process_sheet_to_pdf(&doc, reader, 100.0f) != 0) {
        fprintf(stderr, "Ошибка при обработке листа Excel\n");
        HPDF_Free(doc.pdf);
        xlsxioread_close(reader);
        return -1;
    }

    xlsxioread_close(reader);
    pdf_end_page(&doc);

    pdf_finalize_document(&doc, pdf_file);
    printf("PDF создан из Excel: %s\n", pdf_file);
    return 0;
}

