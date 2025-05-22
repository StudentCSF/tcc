#ifndef TESTS_H
#define TESTS_H

void test_create_pdf_from_text();
void test_create_pdf_from_text_null();
void test_create_pdf_from_xlsx();
void test_create_pdf_from_xlsx_invalid_file();

void test_create_pdf_from_text_empty_string();
void test_create_pdf_from_text_long_string();

void test_create_pdf_from_xlsx_empty_file();
void test_create_pdf_from_xlsx_no_extension();
void test_create_pdf_from_xlsx_null_args();
void test_create_pdf_from_xlsx_single_cell();

#endif

