
configure_file(cfg/config.h.in ${PROJECT_SOURCE_DIR}/include/config/config.h @ONLY)
# ------------------------ LIBRARY ------------------------ #
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_SHARED_LIBRARY_PREFIX "")
set(SHARED_NAME_LIBRARY time_info)  # Имя динамической библиотеки
# Исходные файлы динамической библиотеки
set(LIB_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/lib/time_info.cpp)
add_library(${SHARED_NAME_LIBRARY} SHARED ${LIB_SOURCES})
# ------------------------ LIBRARY ------------------------ #
set(SOURCES ${PROJECT_SOURCE_DIR}/src/main.cpp)                 # Исходные файлы
set(HEADERS)                                                    # Заголовочные файлы
# Компилируем исполняемый файл с заданным именем
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})
# Линкуем динамическую библиотеку к исполняемому файлу
target_link_libraries(${PROJECT_NAME} PRIVATE ${SHARED_NAME_LIBRARY})
# Переопределние пути до заголовочных файлов
target_include_directories(
    ${PROJECT_NAME}        PRIVATE
    ${PROJECT_SOURCE_DIR}/include)
# ---------------------------------------------------------------------------- #
set_target_properties(${PROJECT_NAME}      PROPERTIES
    CXX_STANDARD ${${META_PROJECT_NAME}_CXX_STANDARD}   # CXX - стандарт
    CXX_STANDARD_REQUIRED                          ON   # Обязательная поддержка стандарта
    # Задает рабочий каталог локального отладчика
    # для целевых объектов Visual Studio C++
    VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
