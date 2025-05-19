//
//  displayMyDPFPage.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#ifndef MY_PDF_RENDERER_H
#define MY_PDF_RENDERER_H

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
//#include "PDFUtils.h"

/**
 Рендерит указанную страницу PDF-документа в графический контекст.
 
 @param myContext   Графический контекст для отрисовки (например, растровый контекст изображения).
 @param pageNumber  Номер страницы (начинается с 1).
 @param filename    Путь к PDF-файлу в формате C-строки.
 
 @note Документ автоматически освобождается после отрисовки.
*/
void My_display_pdf_page(CGContextRef myContext, size_t pageNumber, const char *filename);

CFStringRef Extract_text_from_pdf_page(CGPDFPageRef page);
#endif // MY_PDF_RENDERER_H
