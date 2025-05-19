//
//  createDPFFile.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#ifndef PDF_CREATOR_H
#define PDF_CREATOR_H

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

void My_draw_content(CGContextRef context, User userInfo);

void Create_pdf_file(CGRect pageRect, const char *filename, User userInfo);

#endif
