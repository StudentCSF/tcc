cmake_minimum_required(VERSION 3.10)
project(my_pdf_tests C)


# Подключаем директории с заголовками
include_directories(/usr/local/include)
include_directories(../PDFGenerator)

# Линкуем библиотеки
link_directories(/usr/local/lib)

# Создаём исполняемый файл
add_executable(my_pdf_tests main.c
        TCC_tests.c
        ../PDFGenerator/TCC_pdf_document.c
        ../PDFGenerator/TCC_pdf_fonts.c
        ../PDFGenerator/TCC_pdf_pages.c
        )

# Линкуем с собранными локально библиотеками, если нужно
target_link_libraries(my_pdf_tests
        xlsxio_read
        hpdf
        )
