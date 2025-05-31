//
//  makePNGImageFromPDF.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#ifndef PNG_EXPORT_H
#define PNG_EXPORT_H

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

/**
 Экспортирует первую страницу PDF в PNG-файл
 @param path Путь к исходному PDF-файлу
*/
void Make_png_image_from_pdf(char *path);

#endif /* PNG_EXPORT_H */
