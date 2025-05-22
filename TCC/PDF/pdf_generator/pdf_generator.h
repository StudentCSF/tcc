#ifndef PDF_GENERATOR_H
#define PDF_GENERATOR_H

#include <hpdf.h>
#include <xlsxio_read.h>

typedef struct {
    HPDF_Doc pdf;
    HPDF_Page page;
    HPDF_Font font;
    float page_width;
    float page_height;
} PdfDocument;

void handle_pdf_error(HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data);

PdfDocument pdf_init_document(void (*error_handler)(HPDF_STATUS, HPDF_STATUS, void *));

void pdf_finalize_document(PdfDocument *doc, const char *filename);

void pdf_add_page_with_font(PdfDocument *doc, const char *font_name, float font_size);

void pdf_write_text_at(PdfDocument *doc, float x, float y, const char *text);

void pdf_end_page(PdfDocument *doc);

float pdf_line_height(void);

void pdf_check_new_page(PdfDocument *doc, float *y);

int create_pdf_from_text(const char *text, const char *pdf_file);

int create_pdf_from_xlsx(const char *xlsx_file, const char *pdf_file);

xlsxioreader open_xlsx_reader(const char *xlsx_file);
int process_sheet_to_pdf(PdfDocument *doc, xlsxioreader reader, float cell_padding);
int process_row_to_pdf(PdfDocument *doc, xlsxioreadersheet sheet, float *y, float cell_padding);


#endif
