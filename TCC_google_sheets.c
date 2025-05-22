#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <curl/curl.h>
#include <cJSON.h>


// Объявляем прототипы функций для Windows
#if defined(_WIN32) && !defined(strnlen)
size_t strnlen(const char* s, size_t maxlen); 
#endif

char* strndup(const char* s, size_t n); // Явное объявление прототипа

// Реализация strnlen для Windows
#if defined(_WIN32) && !defined(strnlen)
size_t strnlen(const char* s, size_t maxlen) {
    const char* end = memchr(s, 0, maxlen);
    return end ? (size_t)(end - s) : maxlen;
}
#endif

char* strndup(const char* s, size_t n) {
    size_t len = strnlen(s, n);
    char* new = malloc(len + 1);
    if (new) {
        memcpy(new, s, len);
        new[len] = '\0';
    }
    return new;
}

// Структуры данных
typedef struct {
    char* access_token;
    char* spreadsheet_id;
} GSheetClient;

typedef struct {
    char*** data;
    size_t rows;
    size_t cols;
} SheetRange;

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Получает данные
 * 
 */
static size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    char** response = (char**)userdata;
    *response = strndup(ptr, size * nmemb);
    return size * nmemb;
}

// WORK №1
// Накапливание данных
// size_t write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
//     size_t realsize = size * nmemb;
//     char **response_ptr = (char **)userdata;
    
//     // Первая итерация: создаем буфер
//     if (*response_ptr == NULL) {
//         *response_ptr = malloc(realsize + 1);
//         if (*response_ptr == NULL) return 0;
//     } 
//     // Последующие итерации: расширяем буфер
//     else {
//         size_t current_size = strlen(*response_ptr);
//         char *tmp = realloc(*response_ptr, current_size + realsize + 1);
//         if (tmp == NULL) return 0;
//         *response_ptr = tmp;
//     }
    
//     memcpy(*response_ptr + strlen(*response_ptr), ptr, realsize);
//     (*response_ptr)[strlen(*response_ptr) + realsize] = '\0';
//     return realsize;
// }

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Формирует заголовок для аутентификации в GoogleAPI
 * 
 * @param client Клиент
 * @return header Сформированный заголовок
 */
static char* TCC_buildAuthHeader(GSheetClient* client) {
    const size_t header_len = strlen("Authorization: Bearer ") + strlen(client->access_token) + 1;
    char* header = malloc(header_len);
    snprintf(header, header_len, "Authorization: Bearer %s", client->access_token);
    return header;
}

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Выводит в консоль полученные из таблицы данные
 * 
 * @param data Данные, полученные из таблицы
 * @param range Диапазон
 */
static void TCC_printDataFromGsheet(SheetRange* data, const char* range) {
    printf("Data from range %s:\n", range);
    for (size_t i = 0; i < data->rows; i++) {
        for (size_t j = 0; j < data->cols; j++) {
            printf("%-20s", data->data[i][j]);
        }
        printf("\n");
    }
}

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Очищает память в GSheetClient
 * 
 * @param client Клиент с данными
 */
void TCC_gsheetClientFree(GSheetClient* client) {
    if (!client) return;
    free(client->access_token);
    free(client->spreadsheet_id);
    free(client);
}

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Убирает все ненужные и лишние символы в JSON-ответе
 * 
 * @param response Ответ запроса в виде char* JSON
 */
void TCC_sanitizeJsonResponse(char *response) {
    if (!response) return;

    // Удаляем символы до первой '{'
    char *start = strchr(response, '{');
    if (start) {
        memmove(response, start, strlen(start) + 1);
    }

    // Удаляем символы после последней '}'
    char *end = strrchr(response, '}');
    if (end) {
        *(end + 1) = '\0';
    }
}

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Очищает память в SheetRange
 * 
 * @param range Диапазон
 */
void gsheet_free_range(SheetRange* range) {
    if (!range) return;
    for (size_t i = 0; i < range->rows; i++) {
        for (size_t j = 0; j < range->cols; j++) {
            free(range->data[i][j]);
        }
        free(range->data[i]);
    }
    free(range->data);
    free(range);
}

/**
 * Инициализирует клиента для работы с GoogleAPI
 * 
 * @param access_token Токен доступа (OAuth)
 * @param spreadsheet_id ID таблицы
 * @return GSheetClient* - Клиент с данными аутентификации
 */
GSheetClient* TCC_gsheetInit(const char* access_token, const char* spreadsheet_id) {
    GSheetClient* client = malloc(sizeof(GSheetClient));
    client->access_token = strdup(access_token);
    client->spreadsheet_id = strdup(spreadsheet_id);
    return client;
}


/**
 * Читает GSheet в заданном диапазоне
 * 
 * @param client Указатель на клиент GSheets
 * @param range Диапазон в виде "Sheet1!A1:B2"
 * @return SheetRange* - Полученные данные из таблицы
 */
SheetRange* TCC_gsheetReadRange(GSheetClient* client, const char* range) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return NULL;
    }

    char* response = NULL;
    long http_code = 0;
    CURLcode res = CURLE_OK;
    char url[512];
    struct curl_slist* headers = NULL;

    // Формирование URL
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s",
        client->spreadsheet_id, range);

    printf("Range:\n");
    printf(range);
    printf("\n");
    printf("Created URL:\n");
    printf(url);
    
    // Установка заголовков
    headers = curl_slist_append(headers, TCC_buildAuthHeader(client));
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    // Настройка параметров CURL
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Включить подробное логирование
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // Таймаут 10 секунд
    // Отключаем для проверки без SSL
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);

    // Выполнение запроса
    res = curl_easy_perform(curl);
    
    // Получение HTTP-статуса
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    // Расширенная диагностика
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        
        // Дополнительная информация для частых ошибок
        if (res == CURLE_COULDNT_RESOLVE_HOST) {
            fprintf(stderr, "Check internet connection\n");
        }
        else if (res == CURLE_SSL_CONNECT_ERROR) {
            fprintf(stderr, "SSL/TLS handshake failed\n");
        }
    }

    // Обработка HTTP-статусов
    if (http_code != 200 && http_code != 0) {
        fprintf(stderr, "HTTP Error: %ld\n", http_code);
        if (response) fprintf(stderr, "Response: %s\n", response);
    }

    // Getting rid of special symbol 
    // memmove(response, response+1, strlen(response));
    TCC_sanitizeJsonResponse(response);

    printf("Maked response -> ");
    printf(response);

    SheetRange* result = NULL;
    if (res == CURLE_OK && http_code == 200 && response) {

        // Парсинг ответа
        cJSON* root = cJSON_Parse(response);
        if (!root) {
            fprintf(stderr, "Failed to parse JSON response\n");
        }
        else {
            cJSON* values = cJSON_GetObjectItem(root, "values");
            if (!values) {
                fprintf(stderr, "No 'values' field in response\n");
                if (cJSON_HasObjectItem(root, "error")) {
                    const char* error_msg = cJSON_GetObjectItem(root, "error")->valuestring;
                    fprintf(stderr, "API Error: %s\n", error_msg);
                }
            }

            // Создание структуры данных
            result = malloc(sizeof(SheetRange));
            result->rows = cJSON_GetArraySize(values);
            result->cols = 0;
            result->data = NULL;

            if (result->rows == 0) {
                cJSON_Delete(root);
            }

            // Первый проход: определение максимального количества столбцов
            for (int i = 0; i < result->rows; i++) {
                cJSON* row = cJSON_GetArrayItem(values, i);
                if (cJSON_IsArray(row)) {
                    int cols = cJSON_GetArraySize(row);
                    if (cols > result->cols) {
                        result->cols = cols;
                    }
                }
            }

            // Выделение памяти
            result->data = malloc(sizeof(char**) * result->rows);
            for (int i = 0; i < result->rows; i++) {
                result->data[i] = malloc(sizeof(char*) * result->cols);
                // Инициализация NULL
                memset(result->data[i], 0, sizeof(char*) * result->cols);
            }

            // Второй проход: заполнение данными
            for (int i = 0; i < result->rows; i++) {
                cJSON* row = cJSON_GetArrayItem(values, i);
                if (!cJSON_IsArray(row)) continue;

                for (int j = 0; j < result->cols; j++) {
                    cJSON* cell = cJSON_GetArrayItem(row, j);
                    if (cJSON_IsString(cell)) {
                        // Копирование строки
                        result->data[i][j] = strdup(cell->valuestring);
                    } else {
                        // Пустые ячейки или нестроковые значения
                        result->data[i][j] = strdup("");
                    }
                }
            }

            cJSON_Delete(root);
        }
    }

    // Очистка ресурсов
    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
    free(response);

    return result;
}

// 1. Создать новую таблицу
char* gsheet_create_spreadsheet(GSheetClient* client, const char* title) {
    CURL* curl = curl_easy_init();
    cJSON* root = cJSON_CreateObject();
    cJSON_AddStringToObject(root, "properties.title", title);

    char* payload = cJSON_PrintUnformatted(root);
    char* response = NULL;
    
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, TCC_buildAuthHeader(client));
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, "https://sheets.googleapis.com/v4/spreadsheets");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    CURLcode res = curl_easy_perform(curl);
    char* spreadsheet_id = NULL;
    
    if (res == CURLE_OK && response) {
        cJSON* json = cJSON_Parse(response);
        spreadsheet_id = strdup(cJSON_GetStringValue(cJSON_GetObjectItem(json, "spreadsheetId")));
        cJSON_Delete(json);
    }

    cJSON_Delete(root);
    free(payload);
    curl_easy_cleanup(curl);
    return spreadsheet_id;
}


/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Отправляет запрос и возвращает ответ
 * 
 * @param client Указатель на клиент GSheets
 * @param url URL-ссылка
 * @param payload Передаваемые данные в запросе
 * @return char* - Тело ответа на запрос
 */
char* TCC_sendGsheetRequest(GSheetClient* client, char* url, char* payload) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Failed to initialize CURL\n");
        return NULL;
    }

    char* response = NULL;
    long http_code = 0;
    CURLcode res = CURLE_OK;
    struct curl_slist* headers = NULL;
    
    printf("Created URL:\n");
    printf(url);

    // Установка заголовков
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, TCC_buildAuthHeader(client));

    // Настройка параметров CURL
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Включить подробное логирование
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // Таймаут 10 секунд
    // Отключаем для проверки без SSL
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
    
    // Выполнение запроса
    // curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET"); // delete in other cases
    res = curl_easy_perform(curl);

    // Получение HTTP-статуса
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

    // TODO: 
    // Расширенная диагностика TODO: (NEW FUNC MAKE)
    if (res != CURLE_OK) {
        fprintf(stderr, "CURL error: %s\n", curl_easy_strerror(res));
        
        // Дополнительная информация для частых ошибок
        if (res == CURLE_COULDNT_RESOLVE_HOST) {
            fprintf(stderr, "Check internet connection\n");
        }
        else if (res == CURLE_SSL_CONNECT_ERROR) {
            fprintf(stderr, "SSL/TLS handshake failed\n");
        }
    }

    // Обработка HTTP-статусов (errors)
    if (http_code != 200 && http_code != 0) {
        fprintf(stderr, "HTTP Error: %ld\n", http_code);
        if (response) fprintf(stderr, "Response: %s\n", response);
    }

    // Избавляемся от "плохих" символов в ответе запроса
    TCC_sanitizeJsonResponse(response);

    // Выводим ответ запроса в консоль
    printf("Maked response: \n");
    printf(response);

    return response;
}


// 2. Добавить новый лист
bool TCC_gsheetAddSheet(GSheetClient* client, const char* sheet_title) {
    if (!client || !sheet_title) return FALSE;

    cJSON* root = cJSON_CreateObject();
    cJSON* requests = cJSON_AddArrayToObject(root, "requests");
    
    // Исправленная структура запроса
    cJSON* add_sheet = cJSON_CreateObject();
    cJSON* props = cJSON_AddObjectToObject(add_sheet, "addSheet");
    cJSON* sheet_props = cJSON_AddObjectToObject(props, "properties");
    cJSON_AddStringToObject(sheet_props, "title", sheet_title);
    cJSON_AddItemToArray(requests, add_sheet);

    char* payload = cJSON_PrintUnformatted(root);
    
    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s:batchUpdate",
        client->spreadsheet_id
    );

    // Общая функция для отправки POST-запросов
    char* response = TCC_sendGsheetRequest(client, url, payload);
    cJSON_Delete(root);
    free(payload);

    if (!response) return FALSE;

    // Парсинг ответа
    cJSON* json = cJSON_Parse(response);
    bool result = FALSE;
    
    if (cJSON_HasObjectItem(json, "error")) {
        cJSON* error = cJSON_GetObjectItem(json, "error");
        fprintf(stderr, "Error adding sheet: %s\n", 
                cJSON_GetObjectItem(error, "message")->valuestring);
    } else {
        result = TRUE;
    }

    cJSON_Delete(json);
    free(response);
    return result;
}

typedef struct {
    int sheet_id;
    char* title;
} SheetInfo;

/**
 * Получает информацию о листе (id и название)
 * 
 * @param client Указатель на клиент GSheets
 * @param count Количество листов в таблице
 * @return SheetInfo* - структуру данных с указанными параметрами
 */
SheetInfo* TCC_gsheetGetSheetInfo(GSheetClient* client, int* count) {
    if (!client || !count) return NULL;

    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s/",
        client->spreadsheet_id
    );

    char* response = TCC_sendGsheetRequest(client, url, NULL);

    if (!response) {
        printf("Response is null :(");
        return NULL;
    }

    cJSON* root = cJSON_Parse(response);
    if (!root) {
        fprintf(stderr, "JSON parse error");
        free(response);
        return NULL;
    }

    SheetInfo* sheets = NULL;
    *count = 0;

    if (cJSON_HasObjectItem(root, "sheets")) {

        cJSON* sheets_array = cJSON_GetObjectItem(root, "sheets");
        int num_sheets = cJSON_GetArraySize(sheets_array);
        
        sheets = malloc(sizeof(SheetInfo) * num_sheets);
        
        for (int i = 0; i < num_sheets; i++) {
            cJSON* sheet = cJSON_GetArrayItem(sheets_array, i);
            cJSON* props = cJSON_GetObjectItem(sheet, "properties");
            
            sheets[i].sheet_id = cJSON_GetObjectItem(props, "sheetId")->valueint;
            sheets[i].title = strdup(cJSON_GetObjectItem(props, "title")->valuestring);
            (*count)++;
        }
    } else {
        printf("No item sheets!\n");
    }

    cJSON_Delete(root);
    free(response);
    return sheets;
}

/**
 * Записывает данные в указанный диапазон таблицы
 * 
 * @param client Указатель на клиент GSheets
 * @param range Диапазон в формате "Лист!A1:B2"
 * @param data 2D массив строк (строки -> колонки)
 * @param rows Количество строк
 * @param cols Количество колонок
 * @return TRUE в случае успеха
 */
bool TCC_gsheetWriteRange(GSheetClient* client, const char* range, 
                          char*** data, int rows, int cols) {

    if (!client || !range || !data || rows < 1 || cols < 1) {
        return FALSE;
    }

    // Кодируем диапазон для URL
    CURL* curl = curl_easy_init();
    char* encoded_range = curl_easy_escape(curl, range, 0);
    char url[512];
    snprintf(url, sizeof(url),
        "https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s?valueInputOption=USER_ENTERED",
        client->spreadsheet_id, encoded_range);

    curl_free(encoded_range);
    curl_easy_cleanup(curl);

    cJSON* root = cJSON_CreateObject();
    cJSON* values = cJSON_AddArrayToObject(root, "values");

    // Формируем данные в JSON-формате для отправки
    for (int i = 0; i < rows; i++) {
        cJSON* row = cJSON_CreateArray();
        for (int j = 0; j < cols; j++) {
            cJSON_AddItemToArray(row, cJSON_CreateString(data[i][j]));
        }
        cJSON_AddItemToArray(values, row);
    }

    char* payload = cJSON_PrintUnformatted(root);
    cJSON_Delete(root);

    // Отправка PUT-запроса
    char* response = TCC_sendGsheetRequest(client, url, payload);

    printf("Request URL: %s\n", url);
    printf("Request Data: %s\n", payload);

    bool result = FALSE;

    // Получили запрос
    if (response) {

        cJSON* json = cJSON_Parse(response);

        if (cJSON_HasObjectItem(json, "updatedRange")) {
            result = TRUE;
            
            // Дополнительная проверка количества обновленных ячеек
            cJSON* updatedCells = cJSON_GetObjectItem(json, "updatedCells");

            if (updatedCells) {
                printf("Successfully updated %d cells\n", updatedCells->valueint);
            }

        } else if (cJSON_HasObjectItem(json, "error")) {
            cJSON* error = cJSON_GetObjectItem(json, "error");
            fprintf(stderr, "Write error: %s\n", 
                cJSON_GetObjectItem(error, "message")->valuestring);
        }

        cJSON_Delete(json);
        free(response);
    }

    return result;
}

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Очищает память типа SheetInfo
 * 
 * @param client Указатель на клиент GSheets
 * @param count Количество листов
 */
void TCC_gsheetFreeSheetInfo(SheetInfo* sheets, int count) {
    for (int i = 0; i < count; i++) {
        free(sheets[i].title);
    }
    free(sheets);
}



/**
 * Очищает указанный диапазон в таблице
 * 
 * @param client Указатель на клиент GSheets
 * @param range Диапазон в формате "Лист!A1:B2"
 * @return TRUE в случае успеха
 */
bool TCC_gsheetClearRange(GSheetClient* client, const char* range) {
    if (!client || !range) return FALSE;

    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s:clear",
        client->spreadsheet_id, range
    );

    char* response = TCC_sendGsheetRequest(client, url, "{}");
    if (!response) return FALSE;

    cJSON* json = cJSON_Parse(response);
    bool result = FALSE;

    if (cJSON_HasObjectItem(json, "error")) {
        cJSON* error = cJSON_GetObjectItem(json, "error");
        fprintf(stderr, "Clear range error: %s\n", 
                cJSON_GetObjectItem(error, "message")->valuestring);
    } else {
        result = TRUE;
    }

    cJSON_Delete(json);
    free(response);
    return result;
}

// 5. Удалить строку
bool gsheet_delete_row(GSheetClient* client, int sheet_id, int row) {
    cJSON* root = cJSON_CreateObject();
    cJSON* requests = cJSON_AddArrayToObject(root, "requests");
    cJSON* delete_dim = cJSON_CreateObject();
    cJSON* range = cJSON_AddObjectToObject(delete_dim, "deleteDimension");
    cJSON* dim = cJSON_AddObjectToObject(range, "dimension");
    cJSON_AddNumberToObject(dim, "sheetId", sheet_id);
    cJSON_AddStringToObject(dim, "dimension", "ROWS");
    cJSON_AddNumberToObject(dim, "startIndex", row);
    cJSON_AddNumberToObject(dim, "endIndex", row + 1);
    cJSON_AddItemToArray(requests, delete_dim);

    // Отправка batchUpdate
    // ...
    return TRUE;
}

// 6. Переименовать лист
bool gsheet_rename_sheet(GSheetClient* client, int sheet_id, const char* new_name) {
    cJSON* root = cJSON_CreateObject();
    cJSON* requests = cJSON_AddArrayToObject(root, "requests");
    cJSON* update_props = cJSON_CreateObject();
    cJSON* props = cJSON_AddObjectToObject(update_props, "updateSheetProperties");
    cJSON_AddNumberToObject(props, "sheetId", sheet_id);
    cJSON_AddStringToObject(props, "title", new_name);
    cJSON_AddItemToArray(requests, update_props);

    // Отправка batchUpdate
    // ...
    return TRUE;
}

// 8. Получить историю изменений
void gsheet_get_history(GSheetClient* client) {
    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s/revisions",
        client->spreadsheet_id
    );

    // GET-запрос и парсинг JSON
    // ...
}

// 9. Изменить форматирование ячейки
bool gsheet_format_cell(GSheetClient* client, int sheet_id, const char* cell, int bg_color) {
    cJSON* root = cJSON_CreateObject();
    cJSON* requests = cJSON_AddArrayToObject(root, "requests");
    cJSON* format_req = cJSON_CreateObject();
    cJSON* cell_format = cJSON_AddObjectToObject(format_req, "repeatCell");
    
    // Формирование JSON для формата
    // ...
    
    // Отправка batchUpdate
    return TRUE;
}

// 10. Пакетное обновление
bool gsheet_batch_update(GSheetClient* client, cJSON* requests) {
    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s:batchUpdate",
        client->spreadsheet_id
    );

    // Отправка POST-запроса с JSON из `requests`
    // ...
    return TRUE;
}

// 11. Удаляем лист
void gsheet_delete_sheet(GSheetClient* client, int sheet_id) {
    cJSON* root = cJSON_CreateObject();
    cJSON* requests = cJSON_AddArrayToObject(root, "requests");

    cJSON* delete_sheet = cJSON_CreateObject();
    cJSON_AddItemToObject(delete_sheet, "deleteSheet",
        cJSON_CreateObject());
    cJSON_AddNumberToObject(cJSON_GetObjectItem(delete_sheet, "deleteSheet"), "sheetId", sheet_id);

    cJSON_AddItemToArray(requests, delete_sheet);

    // Отправка batchUpdate
    // ... (реализация аналогично TCC_gsheetWriteRange)
}

/**
 * Читает конкретную указанную ячейку в таблице
 * 
 * @param client Указатель на клиент GSheets
 * @param row Строка
 * @param col Столбец
 * @return SheetRange* Полученные данные из ячейки
 */
SheetRange* TCC_gsheet(GSheetClient* client, int row, int col) {
    char range[32];
    snprintf(range, sizeof(range), "R%dC%d", row, col);
    return TCC_gsheetReadRange(client, range);
}

// 14. Сортировка
int gsheet_sort_range(GSheetClient* client, const char* range, int column_index) {
    cJSON* requests = cJSON_CreateArray();
    cJSON* sort_request = cJSON_CreateObject();
    
    // Создаем объект sortRange
    cJSON* sort_range = cJSON_CreateObject();
    
    // Добавляем поле range
    cJSON_AddStringToObject(sort_range, "range", range);
    
    // Создаем массив sortSpecs
    cJSON* sort_specs = cJSON_CreateArray();
    cJSON* spec = cJSON_CreateObject();
    cJSON_AddNumberToObject(spec, "dimensionIndex", column_index);
    cJSON_AddStringToObject(spec, "sortOrder", "ASCENDING");
    cJSON_AddItemToArray(sort_specs, spec);
    printf("Hello there!\n");
    
    // Добавляем sortSpecs в sortRange
    cJSON_AddItemToObject(sort_range, "sortSpecs", sort_specs);
    
    // Собираем итоговый запрос
    cJSON_AddItemToObject(sort_request, "sortRange", sort_range);
    cJSON_AddItemToArray(requests, sort_request);
    
    // Отправляем запрос и возвращаем результат
    int result = gsheet_batch_update(client, requests);
    
    // Очищаем JSON-структуры
    cJSON_Delete(requests);
    return result;
}

// 15. Установка формулы
// bool gsheet_set_formula(GSheetClient* client, const char* cell, char*** formula) {
//     SheetRange data = {
//         .rows = 1,
//         .cols = 1,
//         .data = formula
//     };
//     // 0 & 1 - default
//     return TCC_gsheetWriteRange(client, cell, &data, 0, 1);
// }

// 16. Объединение ячеек
int gsheet_merge_cells(GSheetClient* client, const char* range) {
    cJSON* requests = cJSON_CreateArray();
    
    // Создаем объект mergeCells
    cJSON* merge_request = cJSON_CreateObject();
    cJSON* merge_params = cJSON_CreateObject();
    
    // Добавляем параметры объединения
    cJSON_AddStringToObject(merge_params, "range", range);
    cJSON_AddStringToObject(merge_params, "mergeType", "MERGE_ALL");
    
    // Собираем структуру запроса
    cJSON_AddItemToObject(merge_request, "mergeCells", merge_params);
    cJSON_AddItemToArray(requests, merge_request);
    
    // Отправляем запрос и возвращаем результат
    int result = gsheet_batch_update(client, requests);
    
    // Очищаем JSON-структуры
    cJSON_Delete(requests);
    return result;
}

// 17. Поиск в таблице
char** gsheet_search(GSheetClient* client, const char* query, int* result_count) {
    CURL* curl = curl_easy_init();
    char* response = NULL;
    char** search_results = NULL;
    *result_count = 0;

    // Формируем JSON-запрос
    cJSON* requests = cJSON_CreateArray();
    cJSON* find_replace = cJSON_CreateObject();
    
    // Создаем структуру findReplace
    cJSON* params = cJSON_CreateObject();
    cJSON_AddStringToObject(params, "find", query);
    cJSON_AddBoolToObject(params, "allSheets", cJSON_True);
    cJSON_AddItemToObject(find_replace, "findReplace", params);
    cJSON_AddItemToArray(requests, find_replace);

    // Отправляем batchUpdate запрос
    if(gsheet_batch_update(client, requests)) {
        // Если запрос успешен, парсим ответ
        cJSON* json = cJSON_Parse(response);
        if(json) {
            // Извлекаем результаты поиска (примерная логика)
            cJSON* matches = cJSON_GetObjectItem(json, "matches");
            if(matches && cJSON_IsArray(matches)) {
                *result_count = cJSON_GetArraySize(matches);
                search_results = malloc(*result_count * sizeof(char*));
                
                // Парсим каждое совпадение
                for(int i = 0; i < *result_count; i++) {
                    cJSON* match = cJSON_GetArrayItem(matches, i);
                    cJSON* cell = cJSON_GetObjectItem(match, "cell");
                    if(cell) {
                        search_results[i] = strdup(cJSON_GetStringValue(cell));
                    }
                }
            }
            cJSON_Delete(json);
        }
    }

    // Очищаем ресурсы
    cJSON_Delete(requests);
    curl_easy_cleanup(curl);
    free(response);
    
    return search_results;
}

/**
 * Экспортирует данные в файл по указанному пути
 * 
 * @param client Указатель на клиент GSheets
 * @param range Диапазон чтения таблицы
 * @param filename Путь к файлу для сохранения
 * @return TRUE При успешном результате
 */
bool TCC_gsheetExportToFile(GSheetClient* client, const char* range, const char* filename) {
    SheetRange* data = TCC_gsheetReadRange(client, range);

    if (!data) {
        printf("No fetch data!\n");
        return FALSE;
    }

    FILE* fp = fopen(filename, "w");
    for (size_t i = 0; i < data->rows; i++) {
        fprintf(fp, "%s\n", data->data[i]);
    }
    fclose(fp);
    return TRUE;
}


////////////TESTS///////////////

// 1. Тест инициализации Клиента
int TCC_testGsheetInit() {
    const char* access_token = "ya29.a0AW4XtxiLn8pkAP9ig5MAxWmL1HDb4Yfn8WJjf9l_YM7mGFAkzkywTC2BFZRhheBE6Uts5yRAWu_re0uCqip4geh5Jxhn8FIOPNJ_IXpzA4AVVYPV2kNlMigNbAYE4N7JKcWOU1IISV3eWzcBspLX2_l3l9p6zsIPak17UycmaCgYKATsSARISFQHGX2MiHCFlo7QRH7L3vBlPTFuKpQ0175";
    const char* spreadsheet_id = "1gr3rYU-EHI8XVsFrkHJy2VTpQhFWEiDN1wxXwgqV1lg";
    GSheetClient* client = TCC_gsheetInit(access_token, spreadsheet_id);
    
    if (!client) {
        fprintf(stderr, "Cannot init client\n");
        return 1;
    } else {
        fprintf(stdout, "TCC_testGsheetInit - PASSED!\n");
        return 0;
    }
}

// 2. Тест чтения из таблицы
int TCC_testGsheetReadRange() {
    const char* access_token = "ya29.a0AW4XtxiLn8pkAP9ig5MAxWmL1HDb4Yfn8WJjf9l_YM7mGFAkzkywTC2BFZRhheBE6Uts5yRAWu_re0uCqip4geh5Jxhn8FIOPNJ_IXpzA4AVVYPV2kNlMigNbAYE4N7JKcWOU1IISV3eWzcBspLX2_l3l9p6zsIPak17UycmaCgYKATsSARISFQHGX2MiHCFlo7QRH7L3vBlPTFuKpQ0175";
    const char* spreadsheet_id = "1gr3rYU-EHI8XVsFrkHJy2VTpQhFWEiDN1wxXwgqV1lg";
    GSheetClient* client = TCC_gsheetInit(access_token, spreadsheet_id);

    const char* range = "Sheet1!A1:E1";
    SheetRange* data = TCC_gsheetReadRange(client, range);

    if (!data) {
        fprintf(stderr, "Cannot read data from gsheet\n");
        return 1;
    } else {
        fprintf(stdout, "TCC_testGsheetReadRange - PASSED!\n");
        return 0;
    }
}

// 3. Тест очистки диапазона
int TCC_testGsheetClearRange() {
    const char* access_token = "ya29.a0AW4XtxiLn8pkAP9ig5MAxWmL1HDb4Yfn8WJjf9l_YM7mGFAkzkywTC2BFZRhheBE6Uts5yRAWu_re0uCqip4geh5Jxhn8FIOPNJ_IXpzA4AVVYPV2kNlMigNbAYE4N7JKcWOU1IISV3eWzcBspLX2_l3l9p6zsIPak17UycmaCgYKATsSARISFQHGX2MiHCFlo7QRH7L3vBlPTFuKpQ0175";
    const char* spreadsheet_id = "1gr3rYU-EHI8XVsFrkHJy2VTpQhFWEiDN1wxXwgqV1lg";
    GSheetClient* client = TCC_gsheetInit(access_token, spreadsheet_id);

    bool success = TCC_gsheetClearRange(client, "Sheet1!A2:F2");

    if (success) {
        fprintf(stdout, "TCC_testGsheetClearRange - PASSED!\n");
    } else {
        fprintf(stderr, "Failed to clear range\n");
    }
}
////////////////////////////////

int main() {
    //Инициализация клиента
    const char* access_token = "ya29.a0AW4XtxiLn8pkAP9ig5MAxWmL1HDb4Yfn8WJjf9l_YM7mGFAkzkywTC2BFZRhheBE6Uts5yRAWu_re0uCqip4geh5Jxhn8FIOPNJ_IXpzA4AVVYPV2kNlMigNbAYE4N7JKcWOU1IISV3eWzcBspLX2_l3l9p6zsIPak17UycmaCgYKATsSARISFQHGX2MiHCFlo7QRH7L3vBlPTFuKpQ0175";
    const char* spreadsheet_id = "1gr3rYU-EHI8XVsFrkHJy2VTpQhFWEiDN1wxXwgqV1lg";
    GSheetClient* client = TCC_gsheetInit(access_token, spreadsheet_id);
    
    if (!client) {
        fprintf(stderr, "Cannot init client\n");
        return 1;
    }
}