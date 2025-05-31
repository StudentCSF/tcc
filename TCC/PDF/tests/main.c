#include <stdbool.h>
#include <stdio.h>
#include <TCC_tests.h>

int main() {
    int fail = 0;

    fail += TCC_PDF_test_CreateFromText() ? 0 : 1;
    fail += TCC_PDF_test_CreateFromText_null() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_invalid_file() ? 0 : 1;
    fail += TCC_PDF_test_CreateFromText_empty_string() ? 0 : 1;
    fail += TCC_PDF_test_CreateFromText_special_chars() ? 0 : 1;
    fail += TCC_PDF_test_CreateFromText_invalid_output_path() ? 0 : 1;
    fail += TCC_PDF_test_CreateFromText_formatting() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_empty_file() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_no_extension() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_null_args() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_single_cell() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_large_file() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_multisheet() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_different_data_types() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_protected_file() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_empty_cells() ? 0 : 1;
    fail += TCC_PDF_test_CreatePdfFromXLSX_merged_cells() ? 0 : 1;

    return fail;
}