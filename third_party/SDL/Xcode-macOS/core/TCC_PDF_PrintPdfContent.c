//
//  printPDFContent.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 17.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <TCC_PDF_GetPdfDocumentRef.h>
#include <TCC_PDF_DisplayPdfPage.h>

void TCC_PDF_PrintPdfContent(const char* filePath) {
    CGPDFDocumentRef document;
    document = TCC_PDF_GetPdfDocumentRef(filePath);
    CGPDFPageRef page = CGPDFDocumentGetPage(document, 1);
    CFStringRef extractedText = TCC_PDF_ExtractTextFromPdfPage(page);
    
    char buffer[4096];
    CFStringGetCString(extractedText, buffer, sizeof(buffer), kCFStringEncodingUTF8);
    printf("Extracted text:\n%s\n", buffer);
    
    CFRelease(extractedText);
    
    CGPDFDocumentRelease (document);
}
