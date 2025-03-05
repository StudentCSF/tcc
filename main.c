#include <TCC/TCC_money.h>
#include <TCC/TCC_STS.h>
//#include <TCC/TCC_compute_shader.h>

//для printf
#include <stdio.h>

// //для таймера
#include <TCC/utils/TCC_timer.h>

int main(void)
{
	//test();
	size_t initLength = 100000000;
	TCC_MoneyNotes * moneyNotes;
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