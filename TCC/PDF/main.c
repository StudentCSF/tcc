#include <stdio.h>
#include <stdlib.h>
#include <hpdf.h>
#include <pdf_generator.h>


int main() {
    create_pdf_from_text("Hello world PDF", "output_text.pdf");
    create_pdf_from_xlsx("example.xlsx", "output_xlsx.pdf");
    return 0;
}

