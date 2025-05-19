//
//  сheckFontsOnPage.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 17.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <stdio.h>

static void Process_font_entry(
                             const char *key,
                             CGPDFObjectRef value,
                             void *info
                             ) {
    CGPDFDictionaryRef fontDict;
    
    // Проверяем, что объект является словарем
    if (CGPDFObjectGetType(value) != kCGPDFObjectTypeDictionary) return;
    
    // Получаем словарь шрифта
    CGPDFObjectGetValue(value, kCGPDFObjectTypeDictionary, &fontDict);
    
    // Извлекаем базовую информацию
    const char* baseFont = "Unknown";
    const char* subtype = "Unknown";
    
    // Получаем BaseFont
    CGPDFStringRef baseFontStr;
    if (CGPDFDictionaryGetString(fontDict, "BaseFont", &baseFontStr)) {
        baseFont = (const char*)CGPDFStringGetBytePtr(baseFontStr);
    }
    
    // Получаем Subtype
    const char* subtypeName;
    if (CGPDFDictionaryGetName(fontDict, "Subtype", &subtypeName)) {
        subtype = subtypeName;
    }
    
    printf("Font '%s':\n", key);
    printf("  BaseFont: %s\n", baseFont);
    printf("  Subtype: %s\n", subtype);
    printf("-------------------\n");
}

void Check_fonts_on_page(CGPDFPageRef page) {
    // Получаем словарь страницы
    CGPDFDictionaryRef pageDict = CGPDFPageGetDictionary(page);
    if (!pageDict) {
        printf("Page dictionary not found\n");
        return;
    }
    
    // Получаем ресурсы страницы
    CGPDFDictionaryRef resources;
    if (!CGPDFDictionaryGetDictionary(pageDict, "Resources", &resources)) {
        printf("No resources found\n");
        return;
    }
    
    // Получаем словарь шрифтов
    CGPDFDictionaryRef fontsDict;
    if (!CGPDFDictionaryGetDictionary(resources, "Font", &fontsDict)) {
        printf("No fonts found\n");
        return;
    }
    
    // Обрабатываем каждый элемент словаря шрифтов
    printf("Found fonts:\n");
    CGPDFDictionaryApplyFunction(fontsDict, Process_font_entry, NULL);
}
