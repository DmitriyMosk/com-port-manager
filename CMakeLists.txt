cmake_minimum_required(VERSION 3.30)

project(com-port-manager LANGUAGES CXX)

set(PJ_INCLUDE_DIR "include")
set(PJ_MODULES_DIR "src/modules")
set(PJ_HARDWARE_DIR "src/hardware")

set(PJ_MODULE_COM_API "com-api")
set(PJ_MODULE_TERM_IO "term-io")

find_program(CMAKE_CXX_COMPILER NAMES g++)
if(NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "C++ compiler not found")
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PJ_INCLUDE_DIR}
    ${PJ_HARDWARE_DIR}
    ${PJ_MODULES_DIR}
)

add_library(hardware STATIC
    ${PJ_HARDWARE_DIR}/hardware.cpp
)

add_subdirectory(${PJ_MODULES_DIR}/${PJ_MODULE_COM_API})
add_subdirectory(${PJ_MODULES_DIR}/${PJ_MODULE_TERM_IO})

add_executable(${PROJECT_NAME} 
    src/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    hardware
    com-api
    term-io
    setupapi
)