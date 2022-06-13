cmake_minimum_required(VERSION 3.1)

SET(LIB_NAME nimg)


project(${LIB_NAME})
set (CMAKE_CXX_STANDARD 11)


find_package(ImageMagick COMPONENTS Magick++)

set(pwd ${CMAKE_CURRENT_LIST_DIR})

execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )

string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})


include_directories(
    ${pwd}
    ${CMAKE_JS_INC} 
    ${ImageMagick_Magick++_INCLUDE_DIRS}
    ${pwd}/lib
    ${NODE_ADDON_API_DIR}
)
add_definitions(
    -DMAGICKCORE_QUANTUM_DEPTH=16
    -DMAGICKCORE_HDRI_ENABLE=1
    -DV8PP_HEADER_ONLY=1
    )

set(LIB_SRC
    ${pwd}/nimg.cpp
)


# add_definitions(-DNAPI_VERSION=3)

add_library(${LIB_NAME} SHARED ${LIB_SRC} ${CMAKE_JS_SRC})

target_link_libraries(${LIB_NAME} ${ImageMagick_LIBRARIES})
set_target_properties(${LIB_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

