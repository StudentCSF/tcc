cmake_minimum_required(VERSION 3.25)
project(TCC_google_sheets C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Указываем пути к cURL
set(CURL_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/curl")
set(CURL_INCLUDE_DIR "${CURL_ROOT}/include")  # Путь до include (без /curl!)
set(CURL_LIBRARY "${CURL_ROOT}/bin/libcurl.dll")  # Для MSVC

# Указываем пути к cjson
set(CJSON_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/third_party/cJSON-master")
#set(CJSON_LIBRARY "${CJSON_ROOT}/cJSON.c")

# Проверка существования файлов (CURL)
if(NOT EXISTS "${CURL_INCLUDE_DIR}/curl/curl.h")
    message(FATAL_ERROR "Файл curl.h не найден! Проверьте: ${CURL_INCLUDE_DIR}/curl/curl.h")
endif()

if(NOT EXISTS "${CURL_LIBRARY}")
    message(FATAL_ERROR "Библиотека не найдена: ${CURL_LIBRARY}")
endif()



# Проверка существования файлов (cjson)
if(NOT EXISTS "${CJSON_ROOT}/cJSON.h")
    message(FATAL_ERROR "Файл cJSON.h не найден! ")
endif()

# Создание исполняемого файла
add_executable(TCC_google_sheets 
    TCC_google_sheets.c
    "${CMAKE_CURRENT_SOURCE_DIR}/third_party/cJSON-master/cJSON.c"
)

# Подключение заголовков
target_include_directories(TCC_google_sheets PRIVATE 
    ${CURL_INCLUDE_DIR}  # Путь должен указывать на папку include (не include/curl!)
    ${CJSON_ROOT}  
)

# Линковка библиотеки
target_link_libraries(TCC_google_sheets PRIVATE 
    ${CURL_LIBRARY}
    ${CJSON_LIBRARY}
)

# Для Windows
if(WIN32)
    target_link_libraries(TCC_google_sheets PRIVATE
        wldap32
        ws2_32
        crypt32
        advapi32
    )
    target_compile_definitions(TCC_google_sheets PRIVATE -D_WIN32)
endif()