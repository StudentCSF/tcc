#include <TCC_pdf_fonts.h>
#include <TCC_pdf_pages.h>
#include <TCC_pdf_document.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xlsxio_read.h>
#include <stdbool.h>


bool VTL_test_CheckCondition(bool test_condition, const char* test_fail_message) {
    if(!test_condition) {
        printf("[ОШИБКА] %s\n", test_fail_message);
    }
    return test_condition;
}

bool TCC_PDF_test_CreateFromText() {
    const char *text = "Это строка 1\nЭто строка 2\nЭто строка 3";
    const char *output_file = "test_output_text.pdf";

    printf("[ТЕСТ] TCC_PDF_CreateFromText - валидный ввод...\n");
    int result = TCC_PDF_CreateFromText(text, output_file);
    return VTL_test_CheckCondition(result == 0, "TCC_PDF_CreateFromText вернул ошибку");
}

bool TCC_PDF_test_CreateFromText_null() {
    printf("[ТЕСТ] TCC_PDF_CreateFromText - NULL параметры...\n");
    int result = TCC_PDF_CreateFromText(NULL, NULL);
    return VTL_test_CheckCondition(result != 0, "Ожидалась ошибка, но функция вернула успех");
}

bool TCC_PDF_test_CreatePdfFromXLSX() {
    const char *xlsx_input = "example.xlsx";
    const char *pdf_output = "test_output_xlsx.pdf";

    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - валидный файл Excel...\n");
    int result = TCC_PDF_CreatePdfFromXLSX(xlsx_input, pdf_output);
    return VTL_test_CheckCondition(result == 0, "TCC_PDF_CreatePdfFromXLSX вернул ошибку");
}

bool TCC_PDF_test_CreatePdfFromXLSX_invalid_file() {
    const char *xlsx_input = "nonexistent.xlsx";

    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - несуществующий файл...\n");
    int result = TCC_PDF_CreatePdfFromXLSX(xlsx_input, "out.pdf");
    return VTL_test_CheckCondition(result != 0, "Ожидалась ошибка, но функция вернула успех");
}

bool TCC_PDF_test_CreateFromText_empty_string() {
    printf("[ТЕСТ] TCC_PDF_CreateFromText - пустая строка...\n");
    int result = TCC_PDF_CreateFromText("", "test_empty_text.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при генерации PDF из пустой строки");
}


bool TCC_PDF_test_CreatePdfFromXLSX_empty_file() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - пустой XLSX файл...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("empty.xlsx", "pdf_from_empty.xlsx.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке пустого Excel файла");
}

bool TCC_PDF_test_CreatePdfFromXLSX_no_extension() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - файл без расширения...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("testdata", "out_no_ext.pdf");
    return VTL_test_CheckCondition(result != 0, "Ожидалась ошибка для файла без расширения");
}

bool TCC_PDF_test_CreatePdfFromXLSX_null_args() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - один аргумент NULL...\n");
    int result1 = TCC_PDF_CreatePdfFromXLSX(NULL, "some.pdf");
    int result2 = TCC_PDF_CreatePdfFromXLSX("test.xlsx", NULL);
    return VTL_test_CheckCondition(result1 != 0 && result2 != 0, "Ожидалась ошибка при NULL аргументах");
}

bool TCC_PDF_test_CreatePdfFromXLSX_single_cell() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - файл с одной ячейкой...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("single_cell.xlsx", "single_cell_out.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке Excel с одной ячейкой");
}

bool TCC_PDF_test_CreateFromText_special_chars() {
    const char *text = "Спецсимволы: !@#$%^&*()\nÜñîçødë текста\n日本語\nالعربية";
    printf("[ТЕСТ] TCC_PDF_CreateFromText - специальные символы...\n");
    int result = TCC_PDF_CreateFromText(text, "test_special_chars.pdf");
    return VTL_test_CheckCondition(result == 0, "Не удалось создать PDF со спецсимволами");
}


bool TCC_PDF_test_CreatePdfFromXLSX_large_file() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - большой Excel файл...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("large_data.xlsx", "large_out.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке большого Excel файла");
}

bool TCC_PDF_test_CreatePdfFromXLSX_multisheet() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - файл с несколькими листами...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("multisheet.xlsx", "multisheet_out.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке Excel с несколькими листами");
}

bool TCC_PDF_test_CreatePdfFromXLSX_different_data_types() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - разные типы данных...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("mixed_data_types.xlsx", "mixed_types_out.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке разных типов данных");
}

bool TCC_PDF_test_CreateFromText_invalid_output_path() {
    printf("[ТЕСТ] TCC_PDF_CreateFromText - невалидный путь для вывода...\n");
    int result = TCC_PDF_CreateFromText("Test text", "/nonexistent/path/output.pdf");
    return VTL_test_CheckCondition(result != 0, "Ожидалась ошибка при записи по несуществующему пути");
}

bool TCC_PDF_test_CreatePdfFromXLSX_protected_file() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - защищенный паролем файл...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("protected.xlsx", "protected_out.pdf");
    return VTL_test_CheckCondition(result != 0, "Ожидалась ошибка при открытии защищенного файла");
}

bool TCC_PDF_test_CreateFromText_formatting() {
    const char *text = "Заголовок\n\nОсновной текст\n\nСноска";
    printf("[ТЕСТ] TCC_PDF_CreateFromText - проверка форматирования...\n");
    int result = TCC_PDF_CreateFromText(text, "test_formatting.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при создании PDF с форматированием");
}

bool TCC_PDF_test_CreatePdfFromXLSX_empty_cells() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - файл с пустыми ячейками...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("empty_cells.xlsx", "empty_cells_out.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке Excel с пустыми ячейками");
}

bool TCC_PDF_test_CreatePdfFromXLSX_merged_cells() {
    printf("[ТЕСТ] TCC_PDF_CreatePdfFromXLSX - объединенные ячейки...\n");
    int result = TCC_PDF_CreatePdfFromXLSX("merged_cells.xlsx", "merged_cells_out.pdf");
    return VTL_test_CheckCondition(result == 0, "Ошибка при обработке Excel с объединенными ячейками");
}
