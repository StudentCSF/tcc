cmake_minimum_required(VERSION 3.16.0)

project(TCC C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(AppName TCC)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/app)


if (MSVC)
    add_compile_options(/O2 /experimental:c11atomics)
endif ()

find_package(OpenGL REQUIRED)

add_executable(${AppName}
        TCC/core/TCC_money.c
        TCC/core/TCC_STS.c
        TCC/infra/TCC_money_load.c
        #TCC/core/TCC_compute_shader.c
        TCC/excel/TCC_excel_reader.c
        TCC/excel/TCC_excel_writer.c
        main.c
        TCC/excel/TCC_excel_file.c
        TCC/excel/TCC_excel_page.c
        TCC/excel/TCC_excel_cell.c
        TCC/excel/TCC_excel_row.c
        TCC/excel/TCC_excel_col.c)


target_include_directories(${AppName} PRIVATE ${CMAKE_SOURCE_DIR})

# # Для таймера и/или ui раскомментировать
add_subdirectory(third_party/SDL)
target_link_libraries(${AppName} PRIVATE SDL2)

# # Для excel reader

# Подключение заголовков
set(XLSXIO_DIR "${CMAKE_SOURCE_DIR}/third_party/XLSXIO_BUILD")

# 3. Подключаем заголовки FFmpeg
target_include_directories(${AppName} PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${XLSXIO_DIR}/include
    # ${OPENCL_DIR}/include
)

# Для Windows
target_link_libraries(${AppName} PRIVATE
    ${XLSXIO_DIR}/lib/libxlsxio_read.lib
    ${XLSXIO_DIR}/lib/libxlsxio_write.lib

    # ${OPENCL_DIR}/lib/OpenCL.lib
)

# Копируем DLL файлы
file(GLOB XLSXIO_DLLS "${XLSXIO_DIR}/bin/*.dll")
add_custom_command(TARGET ${AppName} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${XLSXIO_DLLS}
    # ${OPENCL_DLLS}
    $<TARGET_FILE_DIR:${AppName}>
)

# set(BUILD_EXAMPLES OFF CACHE BOOL "Disable examples" FORCE)

# # Сборка ZLIB
# add_subdirectory(third_party/ZLIB_ORIG)

# # Указываем явные пути (на случай, если CMake не найдёт автоматически)
# set(ZLIB_INCLUDE_DIRS 
#     ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ZLIB_ORIG
#     ${CMAKE_BINARY_DIR}/third_party/ZLIB_ORIG  # для zconf.h
# )
# set(ZLIB_LIBRARIES zlibstatic)  # или zlib, если shared
# if(NOT TARGET ZLIB::ZLIB)
#     add_library(ZLIB::ZLIB ALIAS zlibstatic)
# endif()

# # Отключаем ненужные компоненты libzip
# set(ENABLE_BZIP2 OFF CACHE BOOL "Disable BZIP2" FORCE)
# set(ENABLE_LZMA OFF CACHE BOOL "Disable LZMA" FORCE)
# set(ENABLE_ZSTD OFF CACHE BOOL "Disable ZSTD" FORCE)

# # Указываем явный путь к ZLIB (чтобы libzip его нашёл)
# set(ZLIB_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ZLIB_ORIG)

# # Сборка LIBZIP
# add_subdirectory(third_party/LIBZIP_ORIG)

# set(LIBZIP_INCLUDE_DIRS 
#     ${CMAKE_CURRENT_SOURCE_DIR}/third_party/LIBZIP_ORIG/lib
#     ${CMAKE_BINARY_DIR}/third_party/LIBZIP_ORIG  # для zconf.h
# )

# # Явно указываем пути для XLSXIO
# set(XLSXIO_LIBRARY_DIR "${CMAKE_BINARY_DIR}/third_party/XLSXIO_ORIG/Release")
# set(XLSXIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/XLSXIO_ORIG/include")

# # Проверяем существование библиотеки
# if(NOT EXISTS "${XLSXIO_LIBRARY_DIR}/xlsxio_read.lib")
#     message(FATAL_ERROR "xlsxio_read.lib not found at: ${XLSXIO_LIBRARY_DIR}")
# endif()

# # Добавляем пути для линковки
# link_directories(${XLSXIO_LIBRARY_DIR})
# include_directories(${XLSXIO_INCLUDE_DIR})

# # Для Visual Studio - явное указание библиотеки
# if(MSVC)
#     set(XLSXIO_LIBS "${XLSXIO_LIBRARY_DIR}/xlsxio_read.lib")
# else()
#     set(XLSXIO_LIBS xlsxio_read)
# endif()

# # Указываем пути к заголовкам
# set(LIBZIP_LIBRARY zip)
# include_directories(
#     ${ZLIB_INCLUDE_DIRS}
#     ${LIBZIP_INCLUDE_DIRS}
# )

# # Сборка Expat
# add_subdirectory(third_party/EXPAT_ORIG)

# # Указываем пути
# set(EXPAT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/EXPAT_ORIG/lib)
# set(EXPAT_LIBRARY expat)

# # Настройка XLSXIO (отключаем ненужное)
# set(WITH_WIDE OFF CACHE BOOL "Disable wide char support" FORCE)
# set(BUILD_TOOLS OFF CACHE BOOL "Disable tools" FORCE)

# # Указываем пути к зависимостям
# set(LIBZIP_INCLUDE_DIR ${LIBZIP_INCLUDE_DIRS})
# set(ZLIB_INCLUDE_DIR ${ZLIB_INCLUDE_DIRS})
# set(EXPAT_INCLUDE_DIR ${EXPAT_INCLUDE_DIR})

# # Сборка XLSXIO
# add_subdirectory(third_party/XLSXIO_ORIG)

# # Указываем пути к заголовкам XLSXIO
# set(XLSXIO_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/third_party/XLSXIO_ORIG/include)
# set(XLSXIO_LIBRARY xlsxio_read xlsxio_write)
# # set(XLSXIO_BUILD_STATIC ON CACHE BOOL "Build static lib")
# # set(XLSXIO_BUILD_SHARED OFF CACHE BOOL "Disable shared lib")
# if(MSVC)
#     set(XLSXIO_LIB_DIR "${CMAKE_BINARY_DIR}/third_party/XLSXIO_ORIG/Release")
# else()
#     set(XLSXIO_LIB_DIR "${CMAKE_BINARY_DIR}/third_party/XLSXIO_ORIG")
# endif()

# set(XLSXIO_LIB_PATH "${CMAKE_BINARY_DIR}/third_party/XLSXIO_ORIG/Release/xlsxio_read.lib")

# set(XLSXIO_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/third_party/XLSXIO_ORIG/include")
# include_directories(${XLSXIO_INCLUDE_DIR})

# # Явно указываем библиотеки
# # set(XLSXIO_LIBS 
# #     "${CMAKE_BINARY_DIR}/lib/xlsxio_read.lib"
# #     "${CMAKE_BINARY_DIR}/lib/xlsxio_write.lib"
# # )

# # Проверяем существование файла
# if(NOT EXISTS "${XLSXIO_LIB_PATH}")
#     message(FATAL_ERROR "xlsxio_read.lib not found at: ${XLSXIO_LIB_PATH}")
# else()
#     message(STATUS "Found xlsxio_read.lib at: ${XLSXIO_LIB_PATH}")
# endif()

# link_directories(${XLSXIO_LIB_DIR})

# find_library(XLSXIO_READ_LIB
#     NAMES xlsxio_read
#     PATHS "${CMAKE_BINARY_DIR}/third_party/XLSXIO_ORIG/Release"
#     NO_DEFAULT_PATH
#     REQUIRED
# )

# if(TARGET xlsxio_read)
#     target_link_libraries(TCC PRIVATE xlsxio_read)
# else()
#     target_link_libraries(TCC PRIVATE ${XLSXIO_READ_LIB})
# endif()

# target_include_directories(${AppName} PRIVATE
#     ${ZLIB_INCLUDE_DIRS}
#     ${LIBZIP_INCLUDE_DIR}
#     ${EXPAT_INCLUDE_DIR}
#     ${XLSXIO_INCLUDE_DIR}
# )

# # Линковка библиотек
# target_link_libraries(${AppName} PRIVATE
#     ${XLSXIO_LIBRARY}
#     ${LIBZIP_LIBRARY}
#     ${ZLIB_LIBRARIES}
#     ${EXPAT_LIBRARY}
# )
