# CMakeLists.txt
project(PakLib)

file(GLOB SOURCES 
    ./PakInterface.cpp
)

file(GLOB HEADERS 
    ./PakInterface.h
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

source_group("Source Files" FILES ${SOURCES})
source_group("Header Files" FILES ${HEADERS})
