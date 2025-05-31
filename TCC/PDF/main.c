#include <stdio.h>
#include <stdlib.h>
#include <hpdf.h>
#include <TCC_pdf_document.h>


int main() {
    TCC_PDF_CreateFromText("Hello world PDF", "output_text.pdf");
    TCC_PDF_CreatePdfFromXLSX("example.xlsx", "output_xlsx.pdf");
    return 0;
}

