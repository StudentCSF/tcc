#include <TCC/TCC_money.h>
#include <TCC/TCC_STS.h>
// #include <TCC/TCC_compute_shader.h>

// для printf
#include <stdio.h>

// для таймера
#include <TCC/utils/TCC_timer.h>

#include <TCC/excel/TCC_excel_reader.h>
#include <TCC/excel/TCC_excel_page.h>
#include <TCC/excel/TCC_excel_writer.h>

int main(void)
{
	test_excel();
	size_t initLength = 100000000;
	TCC_MoneyNotes *moneyNotes;
	TCC_moneyNotesCreate(&moneyNotes, initLength);
	TCC_moneyNotesLoadTmp(moneyNotes);
	SDL_Init(SDL_INIT_TIMER);
	Uint32 startTime = SDL_GetTicks();
	TCC_Money res = TCC_STSprofitsTax(moneyNotes, 2000);
	Uint32 endTime = SDL_GetTicks();
	printf("\n res: %llu\n", res);
	TCC_moneyNotesDel(&moneyNotes);
	printf("\n eff: %llu\n", TCC_TIMER_getDiffUS(startTime, endTime));
	SDL_Quit();

	return 0;
}

//char* curr_dir() {
//    char cwd[100];
//    if (getcwd(cwd, sizeof(cwd)) != NULL) {
//        printf("Current working dir: %s\n", cwd);
//    } else {
//        perror("getcwd() error");
//        return 1;
//    }
//    return &cwd;
//}

char* curr_dir(char *cwd, int pathSize) {
    if (getcwd(cwd, sizeof(cwd)*pathSize) != NULL) {
        printf("Current working dir: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    return &cwd;
}

int test_excel()
{
	printf("\n TEST EXCEL: %s\n", "START");
    int pathSize = 100;
    TCC_Path currDirInput = (char*)malloc(sizeof(char)*pathSize);
    TCC_Path currDirOutput = (char*)malloc(sizeof(char)*pathSize);
    curr_dir(currDirInput, pathSize);
    strcpy(currDirOutput, currDirInput);
    TCC_Path pathInput = strcat(currDirInput, "\\test\\tcc_excel_test.xlsx");
    TCC_Format format = "xlsx";
    TCC_Path pathOutput = strcat(currDirOutput, "\\test\\tcc_excel_test_output");

    printf("\n\tCurr input: %s\n", pathInput);
    printf("\n\tCurr output: %s\n", pathOutput);

    TCC_ExcelFile *pExcelFile = (TCC_ExcelFile*)malloc(sizeof(TCC_ExcelFile));
    TCC_Error e = TCC_excel_reader_Read(pExcelFile, pathInput);
	printf("\n\tTest read code: %d\n", e);

	TCC_Error e2 = TCC_excel_writer_Write(pExcelFile, pathOutput, format);
	printf("\n\tTest write code: %d\n", e2);

    printf("\n TEST EXCEL: %s\n", "END");
    return 0;
}
