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
# а весь домен уже должен компилироваться и проверяться тестами на каждом шаге.
add_library(salary_core STATIC ${SOURCES})
target_include_directories(salary_core PUBLIC include)

option(SALARY_ENABLE_TESTS "Собирать и запускать тесты" ON)

if(SALARY_ENABLE_TESTS)
    include(FetchContent)

    # Тесты на GoogleTest подтягивается автоматически, ставить в систему
    # ничего не нужно — сборка и локально, и в CI одинаковая.
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS FALSE
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
    foreach(test_source ${TEST_SOURCES})
        get_filename_component(test_name "${test_source}" NAME_WE)
        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} PRIVATE salary_core GTest::gtest_main)
        target_include_directories(${test_name} PRIVATE tests)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
