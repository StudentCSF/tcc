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
} TCC_gsheet_Client;

typedef struct {
    char*** data;
    size_t rows;
    size_t cols;
} TCC_gsheet_Range;

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Получает данные
 * 
 */
static size_t TCC_gsheet_WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    char** response = (char**)userdata;
    *response = strndup(ptr, size * nmemb);
    return size * nmemb;
}

/**
 * ВСПОМОГАТЕЛЬНАЯ ФУНКЦИЯ
 * 
 * Формирует заголовок для аутентификации в GoogleAPI
 * 
 * @param client Клиент
 * @return header Сформированный заголовок
 */
static char* TCC_gsheet_BuildAuthHeader(TCC_gsheet_Client* client) {
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

static void TCC_gsheet_PrintData(TCC_gsheet_Range* data, const char* range) {
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
 * Очищает память в TCC_gsheet_Client
 * 
 * @param client Клиент с данными
 */
void TCC_gsheet_ClientFree(TCC_gsheet_Client* client) {
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
void TCC_gsheet_SanitizeJsonResponse(char *response) {
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
 * Очищает память в TCC_gsheet_Range
 * 
 * @param range Диапазон
 */
void TCC_gsheet_FreeRange(TCC_gsheet_Range* range) {
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
 * @return TCC_gsheet_Client* - Клиент с данными аутентификации
 */
TCC_gsheet_Client* TCC_gsheet_Init(const char* access_token, const char* spreadsheet_id) {
    TCC_gsheet_Client* client = malloc(sizeof(TCC_gsheet_Client));
    client->access_token = strdup(access_token);
    client->spreadsheet_id = strdup(spreadsheet_id);
    return client;
}


/**
 * Читает GSheet в заданном диапазоне
 * 
 * @param client Указатель на клиент GSheets
 * @param range Диапазон в виде "Sheet1!A1:B2"
 * @return TCC_gsheet_Range* - Полученные данные из таблицы
 */
TCC_gsheet_Range* TCC_gsheet_ReadRange(TCC_gsheet_Client* client, const char* range) {
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
    headers = curl_slist_append(headers, TCC_gsheet_BuildAuthHeader(client));
    headers = curl_slist_append(headers, "Content-Type: application/json");
    
    // Настройка параметров CURL
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TCC_gsheet_WriteCallback);
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
    TCC_gsheet_SanitizeJsonResponse(response);

    printf("Maked response -> ");
    printf(response);

    TCC_gsheet_Range* result = NULL;
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
            result = malloc(sizeof(TCC_gsheet_Range));
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
char* TCC_gsheet_SendRequest(TCC_gsheet_Client* client, char* url, char* payload) {
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
    headers = curl_slist_append(headers, TCC_gsheet_BuildAuthHeader(client));

    // Настройка параметров CURL
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, TCC_gsheet_WriteCallback);
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
    TCC_gsheet_SanitizeJsonResponse(response);

    // Выводим ответ запроса в консоль
    printf("Maked response: \n");
    printf(response);

    return response;
}

typedef struct {
    int sheet_id;
    char* title;
} TCC_gsheet_Info;

/**
 * Получает информацию о листе (id и название)
 * 
 * @param client Указатель на клиент GSheets
 * @param count Количество листов в таблице
 * @return TCC_gsheet_Info* - структуру данных с указанными параметрами
 */
TCC_gsheet_Info* TCC_gsheet_GetSheetInfo(TCC_gsheet_Client* client, int* count) {
    if (!client || !count) return NULL;

    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s/",
        client->spreadsheet_id
    );

    char* response = TCC_gsheet_SendRequest(client, url, NULL);

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

    TCC_gsheet_Info* sheets = NULL;
    *count = 0;

    if (cJSON_HasObjectItem(root, "sheets")) {

        cJSON* sheets_array = cJSON_GetObjectItem(root, "sheets");
        int num_sheets = cJSON_GetArraySize(sheets_array);
        
        sheets = malloc(sizeof(TCC_gsheet_Info) * num_sheets);
        
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
bool TCC_gsheet_WriteRange(TCC_gsheet_Client* client, const char* range, 
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
    char* response = TCC_gsheet_SendRequest(client, url, payload);

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
 * Очищает память типа TCC_gsheet_Info
 * 
 * @param client Указатель на клиент GSheets
 * @param count Количество листов
 */
void TCC_gsheet_FreeSheetInfo(TCC_gsheet_Info* sheets, int count) {
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
bool TCC_gsheet_ClearRange(TCC_gsheet_Client* client, const char* range) {
    if (!client || !range) return FALSE;

    char url[256];
    snprintf(url, sizeof(url), 
        "https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s:clear",
        client->spreadsheet_id, range
    );

    char* response = TCC_gsheet_SendRequest(client, url, "{}");
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

/**
 * Читает конкретную указанную ячейку в таблице
 * 
 * @param client Указатель на клиент GSheets
 * @param row Строка
 * @param col Столбец
 * @return TCC_gsheet_Range* Полученные данные из ячейки
 */
TCC_gsheet_Range* TCC_gsheet(TCC_gsheet_Client* client, int row, int col) {
    char range[32];
    snprintf(range, sizeof(range), "R%dC%d", row, col);
    return TCC_gsheet_ReadRange(client, range);
}

/**
 * Экспортирует данные в файл по указанному пути
 * 
 * @param client Указатель на клиент GSheets
 * @param range Диапазон чтения таблицы
 * @param filename Путь к файлу для сохранения
 * @return TRUE При успешном результате
 */
bool TCC_gsheet_ExportToFile(TCC_gsheet_Client* client, const char* range, const char* filename) {
    TCC_gsheet_Range* data = TCC_gsheet_ReadRange(client, range);

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
int TCC_gsheet_TestInit() {
    const char* access_token = ".";
    const char* spreadsheet_id = "-";
    TCC_gsheet_Client* client = TCC_gsheet_Init(access_token, spreadsheet_id);
    
    if (!client) {
        fprintf(stderr, "Cannot init client\n");
        return 1;
    } else {
        fprintf(stdout, "TCC_testGsheetInit - PASSED!\n");
        return 0;
    }
}

// 2. Тест чтения из таблицы
int TCC_gsheet_TestReadRange() {
    const char* access_token = ".";
    const char* spreadsheet_id = "-";
    TCC_gsheet_Client* client = TCC_gsheet_Init(access_token, spreadsheet_id);

    const char* range = "Sheet1!A1:E1";
    TCC_gsheet_Range* data = TCC_gsheet_ReadRange(client, range);

    if (!data) {
        fprintf(stderr, "Cannot read data from gsheet\n");
        return 1;
    } else {
        fprintf(stdout, "TCC_testGsheetReadRange - PASSED!\n");
        return 0;
    }
}

// 3. Тест очистки диапазона
int TCC_gsheet_TestClearRange() {
    const char* access_token = "";
    const char* spreadsheet_id = "";
    TCC_gsheet_Client* client = TCC_gsheet_Init(access_token, spreadsheet_id);

    bool success = TCC_gsheet_ClearRange(client, "Sheet1!A2:F2");

    if (success) {
        fprintf(stdout, "TCC_testGsheetClearRange - PASSED!\n");
    } else {
        fprintf(stderr, "Failed to clear range\n");
    }
}
////////////////////////////////

int main() {
    //Инициализация клиента
    const char* access_token = "token";
    const char* spreadsheet_id = "";
    TCC_gsheet_Client* client = TCC_gsheet_Init(access_token, spreadsheet_id);
    
    if (!client) {
        fprintf(stderr, "Cannot init client\n");
        return 1;
    }
}