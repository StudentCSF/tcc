cmake_minimum_required(VERSION 3.17)
project(test C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/local/include)
include_directories(PDFGenerator)

link_directories(/usr/local/lib)

add_executable(test
        main.c
        PDFGenerator/TCC_pdf_document.c
        PDFGenerator/TCC_pdf_fonts.c
        PDFGenerator/TCC_pdf_pages.c
        )

target_link_libraries(test xlsxio_read hpdf)
