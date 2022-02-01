cmake_minimum_required(VERSION 3.20)
project(olimpic_slalom)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

# Часть для SFML
set(SFML_DIR "c:/SFML/lib/cmake/SFML") # Путь до папки библиотеки
set(SFML_STATIC_LIBRARIES TRUE)        # Нужно для того, чтобы не копировать dll-файлы в рабочую папку проекта
find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED) # Устанавливаем зависимые компоненты

file(GLOB vector "vectors/main.cpp")

add_executable(game main.cpp ${vector})

target_link_libraries(game sfml-graphics sfml-audio)