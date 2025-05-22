//
//  getDPFDocumentRef.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>

CGPDFDocumentRef TCC_PDF_MyGetPdfDocumentRef (const char *filename)
{
    CFStringRef path;
    CFURLRef url;
    CGPDFDocumentRef document;
    size_t count;
 
    path = CFStringCreateWithCString (NULL, filename,
                         kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath (NULL, path,
                        kCFURLPOSIXPathStyle, 0);
    CFRelease (path);
    document = CGPDFDocumentCreateWithURL (url);// 2
    CFRelease(url);
    count = CGPDFDocumentGetNumberOfPages (document);// 3
    if (count == 0) {
        printf("`%s' needs at least one page!", filename);
        return NULL;
    }
    
    return document;
}
