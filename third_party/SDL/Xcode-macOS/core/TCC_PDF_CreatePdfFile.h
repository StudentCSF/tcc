//
//  createDPFFile.h
//  exporter-tcc
//
//  Created by Daniil Shmoylov on 16.05.2025.
//

#include <CoreFoundation/CoreFoundation.h>
#include <CoreGraphics/CoreGraphics.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void TCC_PDF_MyDrawContent(CGContextRef context, User userInfo);

void TCC_PDF_CreatePdfFile(CGRect pageRect, const char *filename, User userInfo);

// MARK: - Basic PDF Operations

/// Prints the page count of a PDF file
void TCC_PDF_Print_pdf_page_count(const char* filename);

/// Retrieves a specific page from a PDF file
/// @param pageNumber Page number (1-based index)
CGPDFPageRef TCC_PDF_Get_pdf_page_from_file(const char* filename, size_t pageNumber);

/// Extracts and prints PDF metadata
void TCC_PDF_Extract_pdf_meta_data(const char* filename);

// MARK: - Security Checks

/// Checks if a PDF is encrypted
bool TCC_PDF_Is_pdf_encrypted(const char* filename);

/// Helper function to print encryption status
void TCC_PDF_Check_pdf_encryption(const char* filename);

// MARK: - Document Validation

/// Verifies PDF structural integrity
bool TCC_PDF_Verify_pdf_integrity(const char* filename);

// MARK: - File Operations

/// Deletes a PDF file by path
bool TCC_PDF_Delete_pdf_file(const char* filename);

/// Creates a duplicate of a PDF file
bool TCC_PDF_Duplicate_pdf_file(const char* originalPath);

// MARK: - Advanced Features

/// Checks PDF version information
void TCC_PDF_Check_pdf_version(const char* filename);

/// Analyzes PDF layers (Optional Content Groups)
void TCC_PDF_Check_pdf_layers(const char* filename);

/// Internal function for processing OCG entries
void TCC_PDF_Process_ocg(CGPDFDictionaryRef ocgDict, int level);

#ifdef __cplusplus
}
#endif

#endif /* TCCPDFUtils_h */
