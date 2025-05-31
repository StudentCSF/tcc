//
//  сheckFontsOnPage.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 17.05.2025.
//

#include <CoreGraphics/CoreGraphics.h>
#include <stdio.h>
#include <TCC_PDF_CreatePdfFile.h>
#include <TCC_PDF_DisplayPdfPage.h>
#include <TCC_PDF_PrintPdfContent.h>
#include <TCC_PDF_GetPdfDocumentRef.h>
#include <TCC_PDF_CheckFontsOnPage.h>
#include <CoreGraphics/CoreGraphics.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <copyfile.h>

static void TCC_PDF_ProcessFontEntry(
                               //TODO: - повторить про const
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

void TCC_PDF_CheckFontsOnPage(CGPDFPageRef page) {
    // Получаем словарь страницы
    // указатель на неизменяемые данные.
    CGPDFDictionaryRef pageDict = CGPDFPageGetDictionary(page);
    if (!pageDict) {
        printf("Page dictionary not found\n");
        return;
    }
    
    // Получаем ресурсы страницы
    // доступ к полям структуры через указатель
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
    CGPDFDictionaryApplyFunction(fontsDict, TCC_PDF_ProcessFontEntry, NULL);
}

void TCC_PDF_PrintPdfPageCount(const char* filename) {
    CFStringRef filenameStr = NULL;
    CFURLRef url = NULL;
    CGPDFDocumentRef doc = NULL;
    
    filenameStr = CFStringCreateWithCString(kCFAllocatorDefault, filename, kCFStringEncodingUTF8);
    if (!filenameStr) {
        printf("Error creating CFString\n");
        return;
    }
    
    url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, filenameStr, kCFURLPOSIXPathStyle, false);
    if (!url) {
        printf("Error creating URL\n");
        CFRelease(filenameStr);
        return;
    }
    
    doc = CGPDFDocumentCreateWithURL(url);
    if (!doc) {
        printf("Error: Could not open PDF file '%s'\n", filename);
        CFRelease(url);
        CFRelease(filenameStr);
        return;
    }
    
    if (CGPDFDocumentIsEncrypted(doc) && !CGPDFDocumentUnlockWithPassword(doc, "")) {
        printf("Document '%s' is encrypted and requires a password\n", filename);
        CGPDFDocumentRelease(doc);
        CFRelease(url);
        CFRelease(filenameStr);
        return;
    }
    
    size_t pageCount = CGPDFDocumentGetNumberOfPages(doc);
    printf("PDF '%s' contains %zu pages\n", filename, pageCount);
    
    // Cleanup
    CGPDFDocumentRelease(doc);
    CFRelease(url);
    CFRelease(filenameStr);
}

CGPDFPageRef TCC_PDF_GetPdfPageFromFile(const char* filename, size_t pageNumber) {
    if (!filename || pageNumber < 1) {
        printf("Invalid input parameters\n");
        return NULL;
    }

    // Создаем CFURL из имени файла
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                      filename,
                                                      kCFStringEncodingUTF8);
    if (!filenameStr) {
        printf("Error creating CFString\n");
        return NULL;
    }
    
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               filenameStr,
                                               kCFURLPOSIXPathStyle,
                                               false);
    if (!url) {
        CFRelease(filenameStr);
        printf("Error creating CFURL\n");
        return NULL;
    }
    
    // Открываем PDF документ
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    CFRelease(url);
    CFRelease(filenameStr);
    
    if (!doc) {
        printf("Failed to open PDF document\n");
        return NULL;
    }
    
    if (CGPDFDocumentIsEncrypted(doc)) {
        if (!CGPDFDocumentUnlockWithPassword(doc, "")) {
            printf("Document is encrypted. Password required.\n");
            CGPDFDocumentRelease(doc);
            return NULL;
        }
    }

    size_t totalPages = CGPDFDocumentGetNumberOfPages(doc);
    
    if (pageNumber > totalPages) {
        printf("Page number %zu exceeds total pages (%zu)\n", pageNumber, totalPages);
        CGPDFDocumentRelease(doc);
        return NULL;
    }
    
    CGPDFPageRef page = CGPDFDocumentGetPage(doc, pageNumber);
    CGPDFDocumentRelease(doc);
    
    return page;
}
 
void TCC_PDF_ExtractPdfMetaData(const char* filename) {
    CFStringRef filenameCFStr = CFStringCreateWithCString(kCFAllocatorDefault, filename, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, filenameCFStr, kCFURLPOSIXPathStyle, false);
    
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    
    CFRelease(url);
    CFRelease(filenameCFStr);
    
    if (!doc) {
        printf("Failed to open PDF document\n");
        return;
    }
    
    if (CGPDFDocumentIsEncrypted(doc) && !CGPDFDocumentUnlockWithPassword(doc, "")) {
        printf("Document is encrypted. Password required.\n");
        CGPDFDocumentRelease(doc);
        return;
    }
    
    CGPDFDictionaryRef info = CGPDFDocumentGetInfo(doc);
    if (!info) {
        printf("No metadata found\n");
        CGPDFDocumentRelease(doc);
        return;
    }
    
    const char* title = "N/A";
    const char* author = "N/A";
    const char* subject = "N/A";
    const char* creator = "N/A";
    const char* producer = "N/A";
    
    CGPDFStringRef titleStr;
    if (CGPDFDictionaryGetString(info, "Title", &titleStr)) {
        title = (const char*)CGPDFStringGetBytePtr(titleStr);
    }
    
    CGPDFStringRef authorStr;
    if (CGPDFDictionaryGetString(info, "Author", &authorStr)) {
        author = (const char*)CGPDFStringGetBytePtr(authorStr);
    }
    
    CGPDFStringRef subjectStr;
    if (CGPDFDictionaryGetString(info, "Subject", &subjectStr)) {
        subject = (const char*)CGPDFStringGetBytePtr(subjectStr);
    }
    
    CGPDFStringRef creatorStr;
    if (CGPDFDictionaryGetString(info, "Creator", &creatorStr)) {
        creator = (const char*)CGPDFStringGetBytePtr(creatorStr);
    }
    
    CGPDFStringRef producerStr;
    if (CGPDFDictionaryGetString(info, "Producer", &producerStr)) {
        producer = (const char*)CGPDFStringGetBytePtr(producerStr);
    }
    
    printf("PDF Metadata for '%s':\n", filename);
    printf("  Title:    %s\n", title);
    printf("  Author:   %s\n", author);
    printf("  Subject:  %s\n", subject);
    printf("  Creator:  %s\n", creator);
    printf("  Producer: %s\n", producer);
    
    CGPDFDocumentRelease(doc);
}

bool TCC_PDF_IsPdfEncrypted(const char *filename) {
    // Создаем CFURL из имени файла
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                      filename,
                                                      kCFStringEncodingUTF8);
    if (!filenameStr) return false;
    
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               filenameStr,
                                               kCFURLPOSIXPathStyle,
                                               false);
    CFRelease(filenameStr);
    if (!url) return false;
    
    // Открываем PDF документ
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    CFRelease(url);
    
    if (!doc) return false;
    
    // Проверяем шифрование
    bool isEncrypted = CGPDFDocumentIsEncrypted(doc);
    
    // Дополнительная проверка на необходимость пароля
    if (isEncrypted) {
        if (!CGPDFDocumentUnlockWithPassword(doc, "")) {
            printf("Document is encrypted with password\n");
        }
    }
    
    CGPDFDocumentRelease(doc);
    return isEncrypted;
}

// Пример использования
void TCC_PDF_CheckPdfEncryption(const char *filename) {
    bool encrypted = TCC_PDF_IsPdfEncrypted(filename);
    printf("PDF '%s' is %sencrypted\n",
           filename,
           encrypted ? "" : "not ");
}

void TCC_PDF_CheckPdfVersion(const char* filename) {
    // Создаем CFURL из имени файла
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                     filename,
                                                     kCFStringEncodingUTF8);
    if (!filenameStr) return;
    
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               filenameStr,
                                               kCFURLPOSIXPathStyle,
                                               false);
    CFRelease(filenameStr);
    
    if (!url) return;
    
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    CFRelease(url);
    
    if (!doc) {
        printf("Failed to open document\n");
        return;
    }
    
    if (CGPDFDocumentIsEncrypted(doc)) {
        if (!CGPDFDocumentUnlockWithPassword(doc, "")) {
            CGPDFDocumentRelease(doc);
            return;
        }
    }
    

    CGPDFReal majorVersion = 0.0, minorVersion = 0.0;
    CGPDFDocumentGetVersion(doc, &majorVersion, &minorVersion);
    
    printf("PDF Version: %.1f.%.0f\n", majorVersion, minorVersion);
    
    CGPDFDocumentRelease(doc);
}
        
//MARK: - Orientation

void TCC_PDF_ProcessOcg(CGPDFDictionaryRef ocgDict, int level);

void TCC_PDF_CheckPdfLayers(const char* filename) {
    // Создание URL из имени файла
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                      filename,
                                                      kCFStringEncodingUTF8);
    if (!filenameStr) return;
    
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               filenameStr,
                                               kCFURLPOSIXPathStyle,
                                               false);
    CFRelease(filenameStr);
    if (!url) return;
    
    // Открытие документа
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    CFRelease(url);
    
    if (!doc) {
        printf("Failed to open document\n");
        return;
    }
    
    // Получение каталога документа
    CGPDFDictionaryRef catalog = CGPDFDocumentGetCatalog(doc);
    if (!catalog) {
        CGPDFDocumentRelease(doc);
        return;
    }
    
    // Получение OCProperties
    CGPDFDictionaryRef ocProperties;
    if (!CGPDFDictionaryGetDictionary(catalog, "OCProperties", &ocProperties)) {
        printf("No optional content found\n");
        CGPDFDocumentRelease(doc);
        return;
    }
    
    // Получение списка OCGs
    CGPDFArrayRef ocgs;
    if (!CGPDFDictionaryGetArray(ocProperties, "OCGs", &ocgs)) {
        printf("No OCG groups found\n");
        CGPDFDocumentRelease(doc);
        return;
    }
    
    printf("Optional Content Groups in '%s':\n", filename);
    
    // Обработка каждой OCG
    size_t count = CGPDFArrayGetCount(ocgs);
    for (size_t i = 0; i < count; i++) {
        CGPDFDictionaryRef ocg;
        if (CGPDFArrayGetDictionary(ocgs, i, &ocg)) {
            TCC_PDF_ProcessOcg(ocg, 0);
        }
    }
    
    CGPDFDocumentRelease(doc);
}

void TCC_PDF_ProcessOcg(CGPDFDictionaryRef ocgDict, int level) {
    // Получение имени группы
    const char* name = "Unnamed";
    CGPDFStringRef nameStr;
    if (CGPDFDictionaryGetString(ocgDict, "Name", &nameStr)) {
        name = (const char*)CGPDFStringGetBytePtr(nameStr);
    }
    
    // Отступ для вложенности
    for (int i = 0; i < level; i++) printf("  ");
    
    // Получение состояния (исправленная часть)
    const char* state = "Unknown";
    CGPDFObjectRef stateObj;
    if (CGPDFDictionaryGetObject(ocgDict, "Usage", &stateObj)) {
        if (CGPDFObjectGetType(stateObj) == kCGPDFObjectTypeName) {
            const char* stateName;
            CGPDFObjectGetValue(stateObj, kCGPDFObjectTypeName, &stateName);
            state = stateName;
        }
    }
    
    printf("• %s [State: %s]\n", name, state);
    
    // Обработка вложенных групп (без изменений)
    CGPDFArrayRef children;
    if (CGPDFDictionaryGetArray(ocgDict, "OCGs", &children)) {
        size_t childCount = CGPDFArrayGetCount(children);
        for (size_t i = 0; i < childCount; i++) {
            CGPDFDictionaryRef child;
            if (CGPDFArrayGetDictionary(children, i, &child)) {
                TCC_PDF_ProcessOcg(child, level + 1);
            }
        }
    }
}

//MARK: - Check VerifyPDFIntegrity

bool TCC_PDF_VerifyPdfIntegrity(const char* filename) {
    // Создаем CFURL из имени файла
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                     filename,
                                                     kCFStringEncodingUTF8);
    if (!filenameStr) {
        printf("[Error] Failed to create CFString\n");
        return false;
    }
    
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               filenameStr,
                                               kCFURLPOSIXPathStyle,
                                               false);
    CFRelease(filenameStr);
    
    if (!url) {
        printf("[Error] Failed to create CFURL\n");
        return false;
    }
    
    // Открываем PDF документ
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    CFRelease(url);
    
    if (!doc) {
        printf("[Critical] Failed to create PDF document\n");
        return false;
    }
    
    // Проверка базовой структуры
    CGPDFDictionaryRef catalog = CGPDFDocumentGetCatalog(doc);
    if (!catalog) {
        printf("[Critical] Missing catalog dictionary\n");
        CGPDFDocumentRelease(doc);
        return false;
    }
    
    // Проверка обязательных записей
    CGPDFObjectRef typeObj;
    if (!CGPDFDictionaryGetObject(catalog, "Type", &typeObj)) {
        printf("[Warning] Missing Type in catalog\n");
    } else {
        const char* typeName;
        if (CGPDFObjectGetValue(typeObj, kCGPDFObjectTypeName, &typeName)) {
            if (strcmp(typeName, "Catalog") != 0) {
                printf("[Critical] Invalid catalog type: %s\n", typeName);
                CGPDFDocumentRelease(doc);
                return false;
            }
        }
    }
    
    // Попытка чтения метаданных
    CGPDFDictionaryRef info = CGPDFDocumentGetInfo(doc);
    if (!info) {
        printf("[Warning] Missing document info\n");
    }
    
    // Проверка страниц
    size_t pageCount = CGPDFDocumentGetNumberOfPages(doc);
    if (pageCount == 0) {
        printf("[Critical] Document has 0 pages\n");
        CGPDFDocumentRelease(doc);
        return false;
    }
    
    printf("Checking %zu pages...\n", pageCount);
    
    // Подробная проверка каждой страницы
    for (size_t i = 1; i <= pageCount; i++) {
        CGPDFPageRef page = CGPDFDocumentGetPage(doc, i);
        if (!page) {
            printf("[Critical] Missing page %zu\n", i);
            CGPDFDocumentRelease(doc);
            return false;
        }
        
        // Проверка MediaBox
        CGRect mediaBox = CGPDFPageGetBoxRect(page, kCGPDFMediaBox);
        if (CGRectIsEmpty(mediaBox)) {
            printf("[Warning] Page %zu has empty MediaBox\n", i);
        }
        
        // Проверка содержимого потока
        CGPDFContentStreamRef contentStream = CGPDFContentStreamCreateWithPage(page);
        if (!contentStream) {
            printf("[Critical] Broken content stream in page %zu\n", i);
            CGPDFDocumentRelease(doc);
            return false;
        }
        CFRelease(contentStream);
    }
    
    // Расширенные проверки
    CGPDFDictionaryRef pagesTree;
    if (!CGPDFDictionaryGetDictionary(catalog, "Pages", &pagesTree)) {
        printf("[Critical] Missing pages tree\n");
        CGPDFDocumentRelease(doc);
        return false;
    }
    
    // Проверка структуры дерева страниц
    CGPDFInteger pageCountFromTree;
    if (!CGPDFDictionaryGetInteger(pagesTree, "Count", &pageCountFromTree)) {
        printf("[Critical] Missing page count in pages tree\n");
        CGPDFDocumentRelease(doc);
        return false;
    }
    
    if (pageCountFromTree != pageCount) {
        printf("[Critical] Page count mismatch: %ld vs %zu\n",
              pageCountFromTree, pageCount);
        CGPDFDocumentRelease(doc);
        return false;
    }
    
    CGPDFDocumentRelease(doc);
    printf("[Success] Document appears to be valid\n");
    return true;
}

//MARK: - Delete by path

bool TCC_PDF_DeletePdfFile(const char* filename) {
    // Проверка входных параметров
    if (!filename || strlen(filename) == 0) {
        printf("Error: Invalid filename\n");
        return false;
    }

    // Создание CFURL из имени файла
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                      filename,
                                                      kCFStringEncodingUTF8);
    if (!filenameStr) {
        printf("Error: Failed to create CFString\n");
        return false;
    }

    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                               filenameStr,
                                               kCFURLPOSIXPathStyle,
                                               false);
    CFRelease(filenameStr);

    if (!url) {
        printf("Error: Failed to create CFURL\n");
        return false;
    }

    // Проверка существования файла
    if (access(filename, F_OK) != 0) {
        printf("Error: File does not exist\n");
        CFRelease(url);
        return false;
    }

    // Попытка удаления файла
    int result = remove(filename);
    CFRelease(url);

    if (result != 0) {
        perror("Error deleting file");
        return false;
    }

    printf("File '%s' successfully deleted\n", filename);
    return true;
}

//MARK: - Duplicate

bool TCC_PDF_DuplicatePdfFile(const char* originalPath) {
    // Проверка входных параметров
    if (!originalPath || strlen(originalPath) == 0) {
        printf("Error: Invalid input path\n");
        return false;
    }

    // Создание CFURL для оригинального файла
    CFStringRef originalPathStr = CFStringCreateWithCString(kCFAllocatorDefault,
                                                          originalPath,
                                                          kCFStringEncodingUTF8);
    if (!originalPathStr) {
        printf("Error: Failed to create CFString\n");
        return false;
    }

    CFURLRef originalURL = CFURLCreateWithFileSystemPath(kCFAllocatorDefault,
                                                       originalPathStr,
                                                       kCFURLPOSIXPathStyle,
                                                       false);
    CFRelease(originalPathStr);

    if (!originalURL) {
        printf("Error: Failed to create source URL\n");
        return false;
    }

    // Получение компонентов пути
    CFStringRef filename = CFURLCopyLastPathComponent(originalURL);
    CFStringRef extension = CFURLCopyPathExtension(originalURL);
    
    // Создание нового имени файла
    CFStringRef newName = CFStringCreateWithFormat(kCFAllocatorDefault,
                                                 NULL,
                                                 CFSTR("%@ – дубликат"),
                                                 filename);
    
    if (extension) {
        CFStringRef tempName = CFStringCreateWithFormat(kCFAllocatorDefault,
                                                      NULL,
                                                      CFSTR("%@.%@"),
                                                      newName,
                                                      extension);
        CFRelease(newName);
        newName = tempName;
    }

    // Создание URL для нового файла
    CFURLRef directoryURL = CFURLCreateCopyDeletingLastPathComponent(kCFAllocatorDefault, originalURL);
    CFURLRef newURL = CFURLCreateCopyAppendingPathComponent(kCFAllocatorDefault,
                                                          directoryURL,
                                                          newName,
                                                          false);
    
    // Конвертация URL в системный путь
    char newPath[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(newURL, true, (UInt8*)newPath, PATH_MAX)) {
        printf("Error: Failed to get new path\n");
        CFRelease(originalURL);
        CFRelease(newURL);
        return false;
    }

    // Копирование файла
    int result = copyfile(originalPath, newPath, NULL, COPYFILE_ALL);
    if (result != 0) {
        perror("Error copying file");
        CFRelease(originalURL);
        CFRelease(newURL);
        return false;
    }

    // Освобождение ресурсов
    CFRelease(originalURL);
    CFRelease(newURL);
    CFRelease(filename);
    if (extension) CFRelease(extension);
    CFRelease(newName);
    CFRelease(directoryURL);

    printf("File successfully duplicated to: %s\n", newPath);
    return true;
}
