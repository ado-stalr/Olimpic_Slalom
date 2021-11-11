cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(olimpic_slalom)

set(SFML_STATIC_LIBRARIES TRUE)

find_package(Freetype REQUIRED)
find_package(JPEG REQUIRED)
find_package(SFML 2 COMPONENTS window graphics system REQUIRED)

file(GLOB vector "vectors/main.cpp")

add_executable(game main.cpp ${vector})

target_include_directories(game PRIVATE ${SFML_INCLUDE_DIR})
target_compile_features(game PUBLIC cxx_std_17)
target_compile_definitions(game PRIVATE SFML_STATIC)

target_link_libraries(game ${SFML_LIBRARIES} ${SFML_DEPENDENCIES}) 