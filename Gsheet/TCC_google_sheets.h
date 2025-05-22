#ifndef TCC__GSHEET_H
#define TCC__GSHEET_H

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <cJSON.h>
#include <stddef.h>
#include <stdbool.h>
#include <windows.h>

// Объявление структуры клиента
typedef struct {
    char* access_token;
    char* spreadsheet_id;
} TCC_gsheet_Client;

// Объявление структуры диапазона данных
typedef struct {
    char*** data;
    size_t rows;
    size_t cols;
} TCC_gsheet_Range;

// Объявление структуры информации о листе
typedef struct {
    int sheet_id;
    char* title;
} TCC_gsheet_Info;

/* Базовые функции работы с клиентом */
TCC_gsheet_Client* TCC_gsheet_Init(const char* access_token, const char* spreadsheet_id);
void TCC_gsheet_ClientFree(TCC_gsheet_Client* client);

/* Основные операции с таблицей */
TCC_gsheet_Range* TCC_gsheet_ReadRange(TCC_gsheet_Client* client, const char* range);
bool TCC_gsheet_WriteRange(TCC_gsheet_Client* client, const char* range, 
                          char*** data, int rows, int cols);
bool TCC_gsheet_ClearRange(TCC_gsheet_Client* client, const char* range);

/* Вспомогательные функции */
static void TCC_gsheet_PrintData(TCC_gsheet_Range* data, const char* range);
void TCC_gsheet_FreeRange(TCC_gsheet_Range* range);

/* Работа с метаинформацией */
TCC_gsheet_Info* TCC_gsheet_GetSheetInfo(TCC_gsheet_Client* client, int* count);
void TCC_gsheet_FreeSheetInfo(TCC_gsheet_Info* sheets, int count);

/* Дополнительные операции */
TCC_gsheet_Range* TCC_gsheet(TCC_gsheet_Client* client, int row, int col);
bool TCC_gsheet_ExportToFile(TCC_gsheet_Client* client, const char* range, const char* filename);

#endif