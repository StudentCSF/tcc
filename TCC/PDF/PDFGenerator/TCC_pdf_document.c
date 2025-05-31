#include <TCC_pdf_document.h>
#include <TCC_pdf_fonts.h>
#include <TCC_pdf_pages.h>
#include <stdio.h>
#include <stdlib.h>
#include <xlsxio_read.h>

void TCC_PDF_HandleError(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data) {
    fprintf(stderr, "PDF ERROR: error_no=0x%04X, detail_no=%u\n", (unsigned)error_no, (unsigned)detail_no);
    exit(1);
}

TCC_PDF_Document TCC_PDF_InitDocument(void (*error_handler)(HPDF_STATUS, HPDF_STATUS, void *)) {
    TCC_PDF_Document doc;
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

void TCC_PDF_FinalizeDocument(TCC_PDF_Document *doc, const char *filename) {
    if (!doc || !doc->pdf) return;

    if (HPDF_SaveToFile(doc->pdf, filename) != HPDF_OK) {
        fprintf(stderr, "Ошибка сохранения PDF файла: %s\n", filename);
    }
    HPDF_Free(doc->pdf);
    doc->pdf = NULL;
}

int TCC_PDF_CreateFromText(const char *text, const char *pdf_file) {
    if (!text || !pdf_file) {
        fprintf(stderr, "Ошибка: текст или имя PDF-файла не указаны\n");
        return -1;
    }

    FILE *test_fp = fopen(pdf_file, "wb");
    if (test_fp == NULL) {
        fprintf(stderr, "Ошибка: невозможно открыть файл для записи: %s\n", pdf_file);
        return -1;
    }
    fclose(test_fp);
    remove(pdf_file);

    TCC_PDF_Document doc = TCC_PDF_InitDocument(TCC_PDF_HandleError);
    if (!doc.pdf) {
        fprintf(stderr, "Ошибка: не удалось инициализировать PDF-документ\n");
        return -1;
    }

    TCC_PDF_AddPageWithFont(&doc, "Helvetica", 12);
    if (!doc.page || !doc.font) {
        fprintf(stderr, "Ошибка: не удалось создать страницу или шрифт\n");
        HPDF_Free(doc.pdf);
        return -1;
    }

    float x = 50.0f;
    float y = doc.page_height - 50.0f;
    TCC_PDF_WriteTextAt(&doc, x, y, text);
    TCC_PDF_EndPage(&doc);

    HPDF_STATUS status = HPDF_SaveToFile(doc.pdf, pdf_file);
    if (status != HPDF_OK) {
        fprintf(stderr, "Ошибка сохранения PDF файла: %s\n", pdf_file);
        HPDF_Free(doc.pdf);
        return -1;
    }

    TCC_PDF_FinalizeDocument(&doc, pdf_file);
    printf("PDF создан: %s\n", pdf_file);
    return 0;
}

xlsxioreader TCC_PDF_OpenXLSXReader(const char *xlsx_file) {
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

int TCC_PDF_CreatePdfFromXLSX(const char *xlsx_file, const char *pdf_file) {
    if (!xlsx_file || !pdf_file) {
        fprintf(stderr, "Ошибка: не указаны имена входного или выходного файлов\n");
        return -1;
    }

    xlsxioreader reader = TCC_PDF_OpenXLSXReader(xlsx_file);
    if (!reader) return -1;

    TCC_PDF_Document doc = TCC_PDF_InitDocument(TCC_PDF_HandleError);
    if (!doc.pdf) {
        xlsxioread_close(reader);
        return -1;
    }

    TCC_PDF_AddPageWithFont(&doc, "Helvetica", 12);
    if (!doc.page || !doc.font) {
        fprintf(stderr, "Ошибка: не удалось создать страницу или шрифт\n");
        HPDF_Free(doc.pdf);
        xlsxioread_close(reader);
        return -1;
    }

    if (TCC_PDF_ProcessSheetToPDF(&doc, reader, 100.0f) != 0) {
        fprintf(stderr, "Ошибка при обработке листа Excel\n");
        HPDF_Free(doc.pdf);
        xlsxioread_close(reader);
        return -1;
    }

    xlsxioread_close(reader);
    TCC_PDF_EndPage(&doc);
    TCC_PDF_FinalizeDocument(&doc, pdf_file);
    printf("PDF создан из Excel: %s\n", pdf_file);
    return 0;
}
