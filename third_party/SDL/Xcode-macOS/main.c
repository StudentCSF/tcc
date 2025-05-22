//
//  main.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <stdio.h>
#include <TCC_user_User.h>
//MARK: – указатели
//TODO: -  убрать галочки добавить cmake
//TODO: -  именования добавить
#include <TCC_PDF_CreatePdfFile.h>
#include <TCC_PDF_MyDisplayPdfPage.h>
#include <TCC_PDF_PrintPdfContent.h>
#include <TCC_PDF_MyGetPdfDocumentRef.h>
#include <TCC_PDF_CheckFontsOnPage.h>
#include <CoreGraphics/CoreGraphics.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <copyfile.h>

//MARK: - Perform

void TCC_PDF_PerformPdf(void) {
    CGRect pdfRect = CGRectMake(0, 0, 595, 842); // A4 size
    const char *filename = "/Users/daniilshmoylov/Downloads/TCC.pdf";
    TCC_user_User userInfo = TCC_user_CreateUser();
    TCC_PDF_CreatePdfFile(pdfRect, filename, userInfo);
    
    const size_t pageNumber = 1; // Номер страницы (начинается с 1)
    
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(
        NULL,
        pdfRect.size.width,
        pdfRect.size.height,
        8,
        0,
        colorSpace,
        kCGImageAlphaPremultipliedLast
    );
    
    TCC_PDF_MyDisplayPdfPage(context, pageNumber, filename);
    
    CGPDFDocumentRef doc = TCC_PDF_MyGetPdfDocumentRef(filename);
    
    CGPDFPageRef page = CGPDFDocumentGetPage(doc, 1);
    if (page) {
        TCC_PDF_CheckFontsOnPage(page);
    }
    
    CGPDFDocumentRelease(doc);
    // perform 
}

int main(int argc, const char * argv[]) {
    TCC_PDF_PerformPdf();
    return 0;
}

