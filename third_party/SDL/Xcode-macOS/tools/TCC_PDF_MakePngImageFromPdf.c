//
//  makePNGImageFromPDF.m
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <ImageIO/ImageIO.h>
#include <TCC_PDF_DisplayPdfPage.h>

void TCC_PDF_MakePngImageFromPdf(char *path) {
    const size_t pageNumber = 1;
    
    CGRect mediaBox = CGRectMake(0, 0, 595, 842); // Размер A4
    CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
    CGContextRef context = CGBitmapContextCreate(
        NULL,
        mediaBox.size.width,
        mediaBox.size.height,
        8,
        0,
        colorSpace,
        kCGImageAlphaPremultipliedLast
    );
    
    TCC_PDF_DisplayPdfPage(context, pageNumber, path);
    
    CGImageRef image = CGBitmapContextCreateImage(context);
    CFURLRef outputURL = CFURLCreateWithFileSystemPath(
        NULL,
        CFSTR("/Users/daniilshmoylov/Desktop/output.png"),
        kCFURLPOSIXPathStyle,
        false
    );
    
    CFStringRef fileType = CFSTR("public.png");
    
    CGImageDestinationRef destination = CGImageDestinationCreateWithURL(
        outputURL,
        fileType,
        1,
        NULL
    );
    
    CGImageDestinationAddImage(destination, image, NULL);
    CGImageDestinationFinalize(destination);
    
    CFRelease(outputURL);
    CFRelease(destination);
    CGImageRelease(image);
    CGContextRelease(context);
    CGColorSpaceRelease(colorSpace);
    
    printf("PDF page saved to /tmp/output.png\n");
}

