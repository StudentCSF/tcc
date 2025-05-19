//
//  displayMyPDFPage.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

//MARK: - Read code

#include <CoreGraphics/CoreGraphics.h>
#include <CoreText/CoreText.h>
#include <CoreFoundation/CoreFoundation.h>
#include <My_get_pdf_document_ref.h>

static void op_TJ(CGPDFScannerRef scanner, void *info) {
    CGPDFArrayRef array;
    if (!CGPDFScannerPopArray(scanner, &array)) return;
    
    CFMutableStringRef result = (CFMutableStringRef)info;
    
    for (int i = 0; i < CGPDFArrayGetCount(array); i++) {
        CGPDFObjectRef object;
        if (!CGPDFArrayGetObject(array, i, &object)) continue;
        
        if (CGPDFObjectGetType(object) == kCGPDFObjectTypeString) {
            CGPDFStringRef pdfString;
            if (CGPDFObjectGetValue(object, kCGPDFObjectTypeString, &pdfString)) {
                CFStringRef string = CGPDFStringCopyTextString(pdfString);
                if (string) {
                    CFStringAppend(result, string);
                    CFRelease(string);
                }
            }
        }
    }
}

CFStringRef Extract_text_from_pdf_page(CGPDFPageRef page) {
    CGPDFContentStreamRef contentStream = CGPDFContentStreamCreateWithPage(page);
    
    CGPDFOperatorTableRef table = CGPDFOperatorTableCreate();
    CGPDFOperatorTableSetCallback(table, "TJ", op_TJ);
    
    CFMutableStringRef result = CFStringCreateMutable(NULL, 0);
    CGPDFScannerRef scanner = CGPDFScannerCreate(contentStream, table, result);
    
    CGPDFScannerScan(scanner);
    
    CGPDFScannerRelease(scanner);
    CGPDFOperatorTableRelease(table);
    CGPDFContentStreamRelease(contentStream);
    
    return result;
}

//#include <CoreGraphics/CoreGraphics.h>
//#include <CoreFoundation/CoreFoundation.h>

void My_display_pdf_page(CGContextRef myContext,
                    size_t pageNumber,
                    const char *filename)
{
    CGPDFDocumentRef document;
    CGPDFPageRef page;
 
    document = My_get_pdf_document_ref (filename);
    page = CGPDFDocumentGetPage (document, pageNumber);
    CGContextDrawPDFPage (myContext, page);
    CGPDFDocumentRelease (document);
}
