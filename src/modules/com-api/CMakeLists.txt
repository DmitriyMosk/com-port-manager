file(GLOB COM_API_SOURCES "*.cpp")

add_library(com-api STATIC ${COM_API_SOURCES})

target_include_directories(com-api PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)