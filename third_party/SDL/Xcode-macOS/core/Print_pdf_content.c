//
//  printPDFContent.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 17.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <My_get_pdf_document_ref.h>
#include <My_display_pdf_page.h>

void Print_pdf_content(const char* filePath) {
    CGPDFDocumentRef document;
 
    document = My_get_pdf_document_ref(filePath);
    CGPDFPageRef page = CGPDFDocumentGetPage(document, 1);
    CFStringRef extractedText = Extract_text_from_pdf_page(page);
    
    char buffer[4096];
    CFStringGetCString(extractedText, buffer, sizeof(buffer), kCFStringEncodingUTF8);
    printf("Extracted text:\n%s\n", buffer);
    
    CFRelease(extractedText);
    
    CGPDFDocumentRelease (document);
}
