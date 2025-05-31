cmake_minimum_required(VERSION 3.10)
project(exporter-tcc)

set(CMAKE_C_STANDARD 11)

# Подключение системных фреймворков
find_library(COREFOUNDATION CoreFoundation)
find_library(COREGRAPHICS CoreGraphics)
find_library(CORETEXT CoreText)
find_library(APPKIT AppKit)

include_directories(       
    tools      
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(exporter-tcc
    main.c
    TCC_user_User.c
    tools/TCC_PDF_CheckFontsOnPage.c
    tools/TCC_PDF_CreatePdfFile.c
    
    tools/TCC_PDF_DisplayPdfPage.c
    tools/TCC_PDF_GetPdfDocumentRef.c
    tools/TCC_PDF_PrintPdfContent.c
   
    tools/TCC_PDF_MakePngImageFromPdf.c
)

target_link_libraries(exporter-tcc
    ${COREFOUNDATION}
    ${COREGRAPHICS}
    ${CORETEXT}
    ${APPKIT}
)
