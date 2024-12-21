cmake_minimum_required(VERSION 3.30)

project(com-port-manager LANGUAGES CXX)

# init sets
set(PJ_INCLUDE_DIR, "include")
set(PJ_MODULES_DIR, "src/modules")

## modules 
set(PJ_MODULE_COM_API, "com-api")
set(PJ_MODULE_TERM_IO, "term-io")

## compiler check
find_program(CMAKE_CXX_COMPILER NAMES g++)

if(NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "C++ compiler not found")
endif() 

## compiler settings
set(CMAKE_CXX_COMPILER ${CMAKE_CXX_COMPILER})
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PJ_INCLUDE_DIR})

add_subdirectory(${PJ_MODULES_DIR}/${PJ_MODULE_COM_API} ${CMAKE_BINARY_DIR}/src/modules/com-api)
add_subdirectory(${PJ_MODULES_DIR}/${PJ_MODULE_TERM_IO} ${CMAKE_BINARY_DIR}/src/modules/com-api)

file(GLOB SOURCES "src/main.cpp")

add_executable(com-port-manager ${SOURCES})
target_link_libraries(com-port-manager ${PJ_MODULE_COM_API} ${PJ_MODULE_TERM_IO})

# enable_testing()
# add_subdirectory(tests)