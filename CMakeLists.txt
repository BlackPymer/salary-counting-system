cmake_minimum_required(VERSION 3.16)

project(salary_counting_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

# Ядро системы собирается в отдельную библиотеку: main.cpp появится позже,
# а весь домен уже должен компилироваться и проверяться на каждом шаге.
add_library(salary_core STATIC ${SOURCES})
target_include_directories(salary_core PUBLIC include)
