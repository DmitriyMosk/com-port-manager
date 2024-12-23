add_library(term-io STATIC
    term-io.cpp
)

target_include_directories(term-io PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src/hardware
)

target_link_libraries(term-io PRIVATE
    hardware
)