#include <pdf_generator.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xlsxio_read.h>


void test_create_pdf_from_text() {
    const char *text = "Это строка 1\nЭто строка 2\nЭто строка 3";
    const char *output_file = "test_output_text.pdf";

    printf("[ТЕСТ] create_pdf_from_text - валидный ввод...\n");
    int result = create_pdf_from_text(text, output_file);
    if (result == 0) {
        printf("[УСПЕХ] PDF успешно создан из текста: %s\n", output_file);
    } else {
        printf("[ОШИБКА] create_pdf_from_text вернул ошибку\n");
    }
}

void test_create_pdf_from_text_null() {
    printf("[ТЕСТ] create_pdf_from_text - NULL параметры...\n");
    int result = create_pdf_from_text(NULL, NULL);
    if (result != 0) {
        printf("[УСПЕХ] Обработка NULL параметров прошла корректно\n");
    } else {
        printf("[ОШИБКА] Ожидалась ошибка, но функция вернула успех\n");
    }
}

void test_create_pdf_from_xlsx() {
    const char *xlsx_input = "example.xlsx";  // Убедись, что файл существует
    const char *pdf_output = "test_output_xlsx.pdf";

    printf("[ТЕСТ] create_pdf_from_xlsx - валидный файл Excel...\n");
    int result = create_pdf_from_xlsx(xlsx_input, pdf_output);
    if (result == 0) {
        printf("[УСПЕХ] PDF успешно создан из Excel: %s\n", pdf_output);
    } else {
        printf("[ОШИБКА] create_pdf_from_xlsx вернул ошибку\n");
    }
}

void test_create_pdf_from_xlsx_invalid_file() {
    const char *xlsx_input = "nonexistent.xlsx";

    printf("[ТЕСТ] create_pdf_from_xlsx - несуществующий файл...\n");
    int result = create_pdf_from_xlsx(xlsx_input, "out.pdf");
    if (result != 0) {
        printf("[УСПЕХ] Ошибка при открытии несуществующего файла корректно обработана\n");
    } else {
        printf("[ОШИБКА] Ожидалась ошибка, но функция вернула успех\n");
    }
}

void test_create_pdf_from_text_empty_string() {
    printf("[ТЕСТ] create_pdf_from_text - пустая строка...\n");
    int result = create_pdf_from_text("", "test_empty_text.pdf");
    if (result == 0) {
        printf("[УСПЕХ] Создан PDF из пустой строки\n");
    } else {
        printf("[ОШИБКА] Ошибка при генерации PDF из пустой строки\n");
    }
}

void test_create_pdf_from_text_long_string() {
    printf("[ТЕСТ] create_pdf_from_text - очень длинный текст...\n");
    char *long_text = malloc(50000);
    if (!long_text) {
        printf("[ОШИБКА] Не удалось выделить память для длинного текста\n");
        return;
    }

    for (int i = 0; i < 49999; i++) {
        long_text[i] = 'A' + (i % 26);
    }
    long_text[49999] = '\0';

    int result = create_pdf_from_text(long_text, "test_long_text.pdf");
    if (result == 0) {
        printf("[УСПЕХ] PDF успешно создан из длинного текста\n");
    } else {
        printf("[ОШИБКА] Ошибка при генерации PDF из длинного текста\n");
    }

    free(long_text);
}


void test_create_pdf_from_xlsx_empty_file() {
    printf("[ТЕСТ] create_pdf_from_xlsx - пустой XLSX файл...\n");
    int result = create_pdf_from_xlsx("empty.xlsx", "pdf_from_empty.xlsx.pdf");
    if (result == 0) {
        printf("[УСПЕХ] PDF успешно создан из пустого файла\n");
    } else {
        printf("[ОШИБКА] Ошибка при обработке пустого Excel файла\n");
    }
}

void test_create_pdf_from_xlsx_no_extension() {
    printf("[ТЕСТ] create_pdf_from_xlsx - файл без расширения...\n");
    int result = create_pdf_from_xlsx("testdata", "out_no_ext.pdf");
    if (result != 0) {
        printf("[УСПЕХ] Корректная ошибка при отсутствии расширения\n");
    } else {
        printf("[ОШИБКА] Ожидалась ошибка для файла без расширения\n");
    }
}

void test_create_pdf_from_xlsx_null_args() {
    printf("[ТЕСТ] create_pdf_from_xlsx - один аргумент NULL...\n");
    int result1 = create_pdf_from_xlsx(NULL, "some.pdf");
    int result2 = create_pdf_from_xlsx("test.xlsx", NULL);

    if (result1 != 0 && result2 != 0) {
        printf("[УСПЕХ] NULL аргументы корректно обработаны\n");
    } else {
        printf("[ОШИБКА] Ожидалась ошибка при NULL аргументах\n");
    }
}

void test_create_pdf_from_xlsx_single_cell() {
    printf("[ТЕСТ] create_pdf_from_xlsx - файл с одной ячейкой...\n");
    int result = create_pdf_from_xlsx("single_cell.xlsx", "single_cell_out.pdf");
    if (result == 0) {
        printf("[УСПЕХ] PDF создан из Excel с одной ячейкой\n");
    } else {
        printf("[ОШИБКА] Ошибка при обработке Excel с одной ячейкой\n");
    }
}
