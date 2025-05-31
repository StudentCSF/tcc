//
//  getDPFDocumentRef.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#ifndef MY_PDF_UTILS_H
#define MY_PDF_UTILS_H

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

/**
 Создает ссылку на PDF-документ из файла.
 
 @param filename Путь к PDF-файлу в формате C-строки.
 @return CGPDFDocumentRef ссылка на документ или NULL при ошибке.
 
 @note Вызывающая сторона должна освободить ресурсы через CGPDFDocumentRelease().
*/
CGPDFDocumentRef TCC_PDF_MyGetPdfDocumentRef(const char *filename);

#endif // MY_PDF_UTILS_H
