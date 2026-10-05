cmake_minimum_required(VERSION 3.16)

project(salary_counting_system LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

add_compile_options(-Wall -Wextra -Wpedantic -Wshadow)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(salary_core STATIC ${SOURCES})
target_include_directories(salary_core PUBLIC include)

add_executable(salary_sim main.cpp)
target_link_libraries(salary_sim PRIVATE salary_core)

option(SALARY_ENABLE_TESTS "Собирать и запускать тесты" ON)

if(SALARY_ENABLE_TESTS)
    find_package(UnitTest++ REQUIRED)

    enable_testing()

    file(GLOB TEST_SOURCES CONFIGURE_DEPENDS tests/*.cpp)
    foreach(test_source ${TEST_SOURCES})
        get_filename_component(test_name "${test_source}" NAME_WE)
        if(test_name STREQUAL "test_main")
            continue()
        endif()
        add_executable(${test_name} ${test_source} tests/test_main.cpp)
        target_link_libraries(${test_name} PRIVATE salary_core UnitTest++)
        target_include_directories(${test_name} PRIVATE tests)
        add_test(NAME ${test_name} COMMAND ${test_name})
    endforeach()
endif()
