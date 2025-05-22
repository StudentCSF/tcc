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
#include <TCC_PDF_MyGetPdfDocumentRef.h>

static void TCC_PDF_OpTJ(CGPDFScannerRef scanner, void *info) {
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

CFStringRef TCC_PDF_ExtractTextFromPdfPage(CGPDFPageRef page) {
    CGPDFContentStreamRef contentStream = CGPDFContentStreamCreateWithPage(page);
    
    CGPDFOperatorTableRef table = CGPDFOperatorTableCreate();
    CGPDFOperatorTableSetCallback(table, "TJ", TCC_PDF_OpTJ);
    
    CFMutableStringRef result = CFStringCreateMutable(NULL, 0);
    CGPDFScannerRef scanner = CGPDFScannerCreate(contentStream, table, result);
    
    CGPDFScannerScan(scanner);
    
    CGPDFScannerRelease(scanner);
    CGPDFOperatorTableRelease(table);
    CGPDFContentStreamRelease(contentStream);
    
    return result;
}

void TCC_PDF_MyDisplayPdfPage(CGContextRef myContext,
                    size_t pageNumber,
                    const char *filename)
{
    CGPDFDocumentRef document;
    CGPDFPageRef page;
 
    document = TCC_PDF_MyGetPdfDocumentRef (filename);
    page = CGPDFDocumentGetPage (document, pageNumber);
    CGContextDrawPDFPage (myContext, page);
    CGPDFDocumentRelease (document);
}
