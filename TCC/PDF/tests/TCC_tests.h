#ifndef TCC_PDF_TESTS_H
#define TCC_PDF_TESTS_H
#include <stdbool.h>

bool TCC_PDF_test_CreateFromText();
bool TCC_PDF_test_CreateFromText_null();
bool TCC_PDF_test_CreatePdfFromXLSX();
bool TCC_PDF_test_CreatePdfFromXLSX_invalid_file();

bool TCC_PDF_test_CreateFromText_empty_string();
bool TCC_PDF_test_CreateFromText_special_chars();
bool TCC_PDF_test_CreateFromText_invalid_output_path();
bool TCC_PDF_test_CreateFromText_formatting();

bool TCC_PDF_test_CreatePdfFromXLSX_empty_file();
bool TCC_PDF_test_CreatePdfFromXLSX_no_extension();
bool TCC_PDF_test_CreatePdfFromXLSX_null_args();
bool TCC_PDF_test_CreatePdfFromXLSX_single_cell();
bool TCC_PDF_test_CreatePdfFromXLSX_large_file();
bool TCC_PDF_test_CreatePdfFromXLSX_multisheet() ;
bool TCC_PDF_test_CreatePdfFromXLSX_different_data_types();
bool TCC_PDF_test_CreatePdfFromXLSX_protected_file();
bool TCC_PDF_test_CreatePdfFromXLSX_empty_cells();
bool TCC_PDF_test_CreatePdfFromXLSX_merged_cells();


#endif

