cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(PlantsVsZombies C CXX)

set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

include(CMake/Utils.cmake)
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

# Moon: disable discord rpc examples and stuff
set(BUILD_EXAMPLES OFF CACHE BOOL "disable building examples in discord-rpc" FORCE)
set(CLANG_FORMAT_CMD OFF CACHE BOOL "disable clang-format" FORCE)

set(ZLIB_BUILD_TESTING OFF CACHE BOOL "Enable Zlib Examples as tests" FORCE)
set(ZLIB_INSTALL OFF CACHE BOOL "Enable installation of zlib" FORCE)

set(PNG_SHARED ON CACHE BOOL "" FORCE)
set(PNG_STATIC OFF CACHE BOOL "" FORCE)
set(PNG_EXECUTABLES OFF CACHE BOOL "" FORCE)
set(PNG_TESTS OFF CACHE BOOL "" FORCE)
set(PNG_TOOLS OFF CACHE BOOL "" FORCE)
set(SKIP_INSTALL_ALL ON CACHE BOOL "" FORCE)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(CONFIGURE_LOG "CMAKE_INSTALL_PREFIX not set, using ./out")
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "./out" FORCE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(external/discord-rpc EXCLUDE_FROM_ALL)

add_subdirectory(src/ImageLib/zlib EXCLUDE_FROM_ALL)
set(ZLIB_ROOT src/ImageLib/zlib)
set(ZLIB_LIBRARY ZLIB::ZLIB)
set(ZLIB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/src/ImageLib/zlib")

set(BUILD_SHARED_LIBS ON CACHE BOOL "Build as DLL" FORCE)
set(BUILD_EXECUTABLES OFF CACHE BOOL "Build executables" FORCE)
set(BUILD_STATIC_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(BUILD_TESTS "Build test executables" OFF CACHE BOOL "BUILD_SHARED_LIBS OR BUILD_STATIC_LIBS;BUILD_EXECUTABLES" FORCE)
add_subdirectory(src/ImageLib/jpeg)
add_subdirectory(src/ImageLib/png)
add_subdirectory(external/ogg EXCLUDE_FROM_ALL)
add_subdirectory(src/SexyAppFramework/ogg EXCLUDE_FROM_ALL)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build as DLL" FORCE)

target_include_directories(discord-rpc
    PRIVATE
        ${CMAKE_SOURCE_DIR}/external/discord-rpc/thirdparty/rapidjson-1.1.0/include
)

add_subdirectory(src/ImageLib EXCLUDE_FROM_ALL)
add_subdirectory(src/PakLib EXCLUDE_FROM_ALL)
add_subdirectory(src/SexyAppFramework)

# Moon: installing
install(TARGETS LawnProject RUNTIME DESTINATION .)
install(FILES $<TARGET_FILE:ZLIB::ZLIB> DESTINATION .)
install(FILES $<TARGET_FILE:Vorbis::vorbis> DESTINATION .)
install(FILES $<TARGET_FILE:Vorbis::vorbisfile> DESTINATION .)
install(FILES $<TARGET_FILE:ogg> DESTINATION .)
install(FILES $<TARGET_FILE:jpeg> DESTINATION .)
install(FILES $<TARGET_FILE:png_shared> DESTINATION .)
