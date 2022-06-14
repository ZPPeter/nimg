cmake_minimum_required(VERSION 3.16)

SET(LIB_NAME nimg)


project(${LIB_NAME})
set (CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/png)
add_subdirectory(lib/zlib)
add_subdirectory(lib/jpeg)

set(pwd )

execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )

string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})


include_directories(
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_JS_INC} 
    ${NODE_ADDON_API_DIR}
)

add_library(${LIB_NAME} SHARED 
    nimg.cpp
    ${CMAKE_JS_SRC}
)

target_link_libraries(${LIB_NAME} zlib png15_static jpeg)
set_target_properties(${LIB_NAME} PROPERTIES PREFIX "" SUFFIX ".node")



