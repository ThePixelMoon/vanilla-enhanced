cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

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

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(CONFIGURE_LOG "CMAKE_INSTALL_PREFIX not set, using ./out")
    set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "./out" FORCE)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(external/discord-rpc EXCLUDE_FROM_ALL)
add_subdirectory(src/ImageLib/zlib EXCLUDE_FROM_ALL)

target_include_directories(discord-rpc
    PRIVATE
        ${CMAKE_SOURCE_DIR}/external/discord-rpc/thirdparty/rapidjson-1.1.0/include
)

add_subdirectory(src/SexyAppFramework)

# Moon: installing
install(TARGETS LawnProject RUNTIME DESTINATION .)
install(FILES $<TARGET_FILE:ZLIB::ZLIB> DESTINATION .)
