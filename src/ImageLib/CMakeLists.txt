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

# moved from ../../CMakeLists.txt
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build as DLL" FORCE)
set(BUILD_EXECUTABLES OFF CACHE BOOL "Build executables" FORCE)
set(BUILD_STATIC_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(BUILD_TESTS "Build test executables" OFF CACHE BOOL "BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS;BUILD_EXECUTABLES" FORCE)
add_subdirectory(./jpeg)
add_subdirectory(./png)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build as DLL" FORCE)

# dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    jpeg
    png_shared
)