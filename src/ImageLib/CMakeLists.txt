# CMakeLists.txt
project(ImageLib)

file(GLOB SOURCES 
    ./ImageLib.cpp
)

file(GLOB HEADERS 
    ./ImageLib.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC
    ../PakLib
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    HAVE_BOOLEAN
)

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})

# dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    jpeg
    png_shared
)