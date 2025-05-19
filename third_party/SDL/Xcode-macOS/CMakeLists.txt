cmake_minimum_required(VERSION 3.10)
project(exporter-tcc)

set(CMAKE_C_STANDARD 11)

# Подключение системных фреймворков
find_library(COREFOUNDATION CoreFoundation)
find_library(COREGRAPHICS CoreGraphics)
find_library(CORETEXT CoreText)
find_library(APPKIT AppKit)

include_directories(
    core       
    tools      
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(exporter-tcc
    main.c
    User.c
    core/Check_fonts_on_page.c
    core/Create_pdf_file.c
    
    core/My_display_pdf_page.c
    core/My_get_pdf_document_ref.c
    core/Print_pdf_content.c
   
    tools/Make_png_image_from_pdf.c
)

target_link_libraries(exporter-tcc
    ${COREFOUNDATION}
    ${COREGRAPHICS}
    ${CORETEXT}
    ${APPKIT}
)
