//
//  createDPFFile.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <stdbool.h>
#include <TCC_user_User.h>

#ifdef __cplusplus
extern "C" {
#endif

void TCC_PDF_MyDrawContent(CGContextRef context, TCC_user_User userInfo);

void TCC_PDF_CreatePdfFile(CGRect pageRect, const char *filename, TCC_user_User userInfo);

// MARK: - Basic PDF Operations

/// Prints the page count of a PDF file
void TCC_PDF_PrintPdfPageCount(const char* filename);

/// Retrieves a specific page from a PDF file
/// @param pageNumber Page number (1-based index)
CGPDFPageRef TCC_PDF_GetPdfPageFromFile(const char* filename, size_t pageNumber);

/// Extracts and prints PDF metadata
void TCC_PDF_ExtractPdfMetaData(const char* filename);

// MARK: - Security Checks

/// Checks if a PDF is encrypted
bool TCC_PDF_IsPdfEncrypted(const char* filename);

/// Helper function to print encryption status
void TCC_PDF_CheckPdfEncryption(const char* filename);

// MARK: - Document Validation

/// Verifies PDF structural integrity
bool TCC_PDF_VerifyPdfIntegrity(const char* filename);

// MARK: - File Operations

/// Deletes a PDF file by path
bool TCC_PDF_DeletePdfFile(const char* filename);

/// Creates a duplicate of a PDF file
bool TCC_PDF_DuplicatePdfFile(const char* originalPath);

// MARK: - Advanced Features

/// Checks PDF version information
void TCC_PDF_CheckPdfVersion(const char* filename);

/// Analyzes PDF layers (Optional Content Groups)
void TCC_PDF_CheckPdfLayers(const char* filename);

/// Internal function for processing OCG entries
void TCC_PDF_ProcessOcg(CGPDFDictionaryRef ocgDict, int level);

#ifdef __cplusplus
}
#endif
