//
//  main.c
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <stdio.h>
#include <user.h>
//MARK: – указатели
//TODO: -  убрать галочки добавить cmake
//TODO: -  именования добавить
#include <Create_pdf_file.h>
#include <My_display_pdf_page.h>
#include <Print_pdf_content.h>
#include <My_get_pdf_document_ref.h>
#include <Check_fonts_on_page.h>
#include <CoreGraphics/CoreGraphics.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <copyfile.h>

void Print_pdf_page_count(const char* filename) {
    CFStringRef filenameStr = CFStringCreateWithCString(kCFAllocatorDefault, filename, kCFStringEncodingUTF8);
    CFURLRef url = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, filenameStr, kCFURLPOSIXPathStyle, false);
    
    CGPDFDocumentRef doc = CGPDFDocumentCreateWithURL(url);
    
    if (!doc) {
        printf("Error: Could not open PDF file '%s'\n", filename);
        goto cleanup;
    }
    
    if (CGPDFDocumentIsEncrypted(doc) && !CGPDFDocumentUnlockWithPassword(doc, "")) {
        printf("Document '%s' is encrypted and requires a password\n", filename);
        goto cleanup;
    }
    
    size_t pageCount = CGPDFDocumentGetNumberOfPages(doc);
    printf("PDF '%s' contains %zu pages\n", filename, pageCount);
//TODO: - remove goto
cleanup:
    if (doc) CGPDFDocumentRelease(doc);
    if (url) CFRelease(url);
    if (filenameStr) CFRelease(filenameStr);
}

CGPDFPageRef Get_pdf_page_from_file(const char* filename, size_t pageNumber) {
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
 
void Extract_pdf_meta_data(const char* filename) {
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

bool Is_pdf_encrypted(const char *filename) {
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
void Check_pdf_encryption(const char *filename) {
    bool encrypted = Is_pdf_encrypted(filename);
    printf("PDF '%s' is %sencrypted\n",
           filename,
           encrypted ? "" : "not ");
}

void Check_pdf_version(const char* filename) {
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

void Process_ocg(CGPDFDictionaryRef ocgDict, int level);

void Check_pdf_layers(const char* filename) {
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
            Process_ocg(ocg, 0);
        }
    }
    
    CGPDFDocumentRelease(doc);
}

void Process_ocg(CGPDFDictionaryRef ocgDict, int level) {
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
                Process_ocg(child, level + 1);
            }
        }
    }
}

//MARK: - Check VerifyPDFIntegrity

bool Verify_pdf_integrity(const char* filename) {
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

bool Delete_pdf_file(const char* filename) {
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

bool Duplicate_pdf_file(const char* originalPath) {
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

//MARK: - Perform

void Perform_pdf(void) {
    CGRect pdfRect = CGRectMake(0, 0, 595, 842); // A4 size
    const char *filename = "/Users/daniilshmoylov/Downloads/TCC.pdf";
    User userInfo = Create_user();
    Create_pdf_file(pdfRect, filename, userInfo);
    
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
    
    My_display_pdf_page(context, pageNumber, filename);
    
    CGPDFDocumentRef doc = My_get_pdf_document_ref(filename);
    
    CGPDFPageRef page = CGPDFDocumentGetPage(doc, 1);
    if (page) {
        Check_fonts_on_page(page);
    }
    
    CGPDFDocumentRelease(doc);
    Extract_pdf_meta_data(filename);
    Print_pdf_content(filename);
    Check_pdf_layers(filename);
    Verify_pdf_integrity(filename);
//    DeletePDFFile(filename);
    Duplicate_pdf_file(filename);
}

int main(int argc, const char * argv[]) {
    Perform_pdf();
    return 0;
}

