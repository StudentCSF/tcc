//
//  createPDFFile.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <CoreFoundation/CoreFoundation.h>
#include <CoreText/CoreText.h>
#include <TCC_user_User.h>

// Вспомогательная функция для создания CFString из строки
CFStringRef TCC_PDF_CreateCfStringFromCstring(const char* cstr) {
    return CFStringCreateWithCString(NULL, cstr, kCFStringEncodingUTF8);
}

// Функция для отрисовки текстового элемента
CGFloat TCC_PDF_DrawTextField(CGContextRef context, CGFloat yPos, CFDictionaryRef attrs, CFStringRef label, CFStringRef value) {
    // Создаем строку вида "Label: Value"
    CFStringRef fullString = CFStringCreateWithFormat(NULL, NULL, CFSTR("%@: %@"), label, value);
    
    // Создаем атрибутированную строку
    CFAttributedStringRef attrString = CFAttributedStringCreate(NULL, fullString, attrs);
    
    // Создаем и рисуем линию
    CTLineRef line = CTLineCreateWithAttributedString(attrString);
    CGContextSetTextPosition(context, 50, yPos);
    CTLineDraw(line, context);
    
    // Рассчитываем высоту строки
    CGFloat ascent, descent, leading;
    CTLineGetTypographicBounds(line, &ascent, &descent, &leading);
    CGFloat lineHeight = ascent + descent + leading + 10; // Добавляем отступ 10pt
    
    // Освобождаем ресурсы
    CFRelease(line);
    CFRelease(attrString);
    CFRelease(fullString);
    
    return yPos - lineHeight; // Возвращаем новую позицию Y
}

// Основная функция отрисовки
void TCC_PDF_MyDrawContent(CGContextRef context, TCC_user_User userInfo) {
    CGContextSaveGState(context);
    
    const CGFloat pageWidth = 595.0;
    const CGFloat pageHeight = 842.0;
    
    // Применяем трансформации
    CGContextTranslateCTM(context, pageWidth, pageHeight);
    CGContextScaleCTM(context, -1.0, -1.0);
    CGAffineTransform transform = CGAffineTransformMake(-1, 0, 0, -1, pageWidth, pageHeight);
    CGContextConcatCTM(context, transform);

    // Начальная позиция
    CGFloat currentY = pageHeight - 100;
    
    // Стиль для заголовка
    CTFontRef titleFont = CTFontCreateWithName(CFSTR("Helvetica-Bold"), 24.0, NULL);
    CFStringRef titleKeys[] = { kCTFontAttributeName };
    CFTypeRef titleValues[] = { titleFont };
    CFDictionaryRef titleAttrs = CFDictionaryCreate(NULL, titleKeys, titleValues, 1,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
    
    // Стиль для основного текста
    CTFontRef bodyFont = CTFontCreateWithName(CFSTR("Helvetica"), 14.0, NULL);
    CFStringRef bodyKeys[] = { kCTFontAttributeName };
    CFTypeRef bodyValues[] = { bodyFont };
    CFDictionaryRef bodyAttrs = CFDictionaryCreate(NULL, bodyKeys, bodyValues, 1,
        &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);

    // Заголовок
    currentY = TCC_PDF_DrawTextField(context, currentY, titleAttrs,
        CFSTR("User Profile"), CFSTR(""));

    // Основные поля
    char buffer[50];
    
    // ID
    snprintf(buffer, sizeof(buffer), "%d", userInfo.id);
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("ID"), TCC_PDF_CreateCfStringFromCstring(buffer));
    
    // Username
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Username"), TCC_PDF_CreateCfStringFromCstring(userInfo.TCC_kUsername));
    
    // Email
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Email"), TCC_PDF_CreateCfStringFromCstring(userInfo.TCC_kEmail));
    
    // Age
    snprintf(buffer, sizeof(buffer), "%d years", userInfo.TCC_kAge);
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Age"), TCC_PDF_CreateCfStringFromCstring(buffer));
    
    // Registration Date
    snprintf(buffer, sizeof(buffer), "%02d/%02d/%d",
           userInfo.TCC_kRegistration_date.TCC_kDay,
           userInfo.TCC_kRegistration_date.TCC_kMonth,
           userInfo.TCC_kRegistration_date.TCC_kYear);
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Registration Date"), TCC_PDF_CreateCfStringFromCstring(buffer));
    
    // Status
    const char* status = userInfo.TCC_kIs_active ? "Active" : "Inactive";
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Status"), TCC_PDF_CreateCfStringFromCstring(status));
    
    // Rating
    snprintf(buffer, sizeof(buffer), "%.1f/5.0", userInfo.TCC_kRating);
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Rating"), TCC_PDF_CreateCfStringFromCstring(buffer));
    
    // Salary
    snprintf(buffer, sizeof(buffer), "$%.2f", userInfo.TCC_kSalary);
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Salary"), TCC_PDF_CreateCfStringFromCstring(buffer));
    
    // Gender
    const char* genderStr = "Other";
    switch(userInfo.TCC_kGender) {
        case     TCC_kMales: genderStr = "Male"; break;
        case     TCC_kFemales: genderStr = "Female"; break;
        case     TCC_kOther: genderStr = "Not Found"; break;
    }
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Gender"), TCC_PDF_CreateCfStringFromCstring(genderStr));
    
    // Address
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("Address"), CFSTR(""));
    
    // Street
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("  Street"), TCC_PDF_CreateCfStringFromCstring(userInfo.TCC_kAddress.TCC_kStreet));
    
    // City
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("  City"), TCC_PDF_CreateCfStringFromCstring(userInfo.TCC_kAddress.TCC_kCity));
    
    // Postal Code
    currentY = TCC_PDF_DrawTextField(context, currentY, bodyAttrs,
        CFSTR("  Postal Code"), TCC_PDF_CreateCfStringFromCstring(userInfo.TCC_kAddress.TCC_kPostal_code));

    CFRelease(titleAttrs);
    CFRelease(titleFont);
    CFRelease(bodyAttrs);
    CFRelease(bodyFont);
    
    CGContextRestoreGState(context);
}

void TCC_PDF_CreatePdfFile (CGRect pageRect, const char *filename, TCC_user_User userInfo)// 1
{
    CGContextRef pdfContext;
    CFStringRef path;
    CFURLRef url;
    CFDataRef boxData = NULL;
    CFMutableDictionaryRef myDictionary = NULL;
    CFMutableDictionaryRef pageDictionary = NULL;
 
    path = CFStringCreateWithCString (NULL, filename, // 2
                                kCFStringEncodingUTF8);
    url = CFURLCreateWithFileSystemPath (NULL, path, // 3
                     kCFURLPOSIXPathStyle, 0);
    CFRelease (path);
    myDictionary = CFDictionaryCreateMutable(NULL, 0,
                        &kCFTypeDictionaryKeyCallBacks,
                        &kCFTypeDictionaryValueCallBacks); // 4
    CFDictionarySetValue(myDictionary, kCGPDFContextTitle, CFSTR("My PDF File"));
    CFDictionarySetValue(myDictionary, kCGPDFContextCreator, CFSTR("My Name"));
    pdfContext = CGPDFContextCreateWithURL (url, &pageRect, myDictionary); // 5
    CFRelease(myDictionary);
    CFRelease(url);
    pageDictionary = CFDictionaryCreateMutable(NULL, 0,
                        &kCFTypeDictionaryKeyCallBacks,
                        &kCFTypeDictionaryValueCallBacks); // 6
    boxData = CFDataCreate(NULL,(const UInt8 *)&pageRect, sizeof (CGRect));
    CFDictionarySetValue(pageDictionary, kCGPDFContextMediaBox, boxData);
    CGPDFContextBeginPage (pdfContext, pageDictionary); // 7
    TCC_PDF_MyDrawContent (pdfContext, userInfo);// 8
    CGPDFContextEndPage (pdfContext);// 9
    CGContextRelease (pdfContext);// 10
    CFRelease(pageDictionary); // 11
    CFRelease(boxData);
    TCC_user_FreeUser(&userInfo);
}
