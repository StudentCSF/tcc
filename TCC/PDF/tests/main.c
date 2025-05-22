#include <stdio.h>
#include "tests.h"

int main() {
    printf("=== Тестирование PDF генератора ===\n");

    test_create_pdf_from_text();
    test_create_pdf_from_text_null();
    test_create_pdf_from_text_empty_string();
    test_create_pdf_from_text_long_string();

    test_create_pdf_from_xlsx();
    test_create_pdf_from_xlsx_invalid_file();
    test_create_pdf_from_xlsx_empty_file();
    test_create_pdf_from_xlsx_no_extension();
    test_create_pdf_from_xlsx_null_args();
    test_create_pdf_from_xlsx_single_cell();

    printf("=== Все тесты завершены ===\n");
    return 0;
}
