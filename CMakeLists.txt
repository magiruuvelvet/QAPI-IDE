cmake_minimum_required(VERSION 3.12)

project(QAPI-IDE)

# Project configurations
set(CMAKE_CONFIGURATION_TYPES Debug Release RelWithDebInfo)

# Use project folders in IDE's
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Meta and Helper Modules
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}"
    "${PROJECT_SOURCE_DIR}/CMake/Modules"
    "${PROJECT_SOURCE_DIR}/CMake/Libs")

# Set C++ standard macro
include(SetCppStandard)
include(include_fmt_lib)
include(include_enum_lib)

# Folder where generated files go
set(GENERATED_DIR ${CMAKE_CURRENT_BINARY_DIR}/Generated)
include_directories(${GENERATED_DIR})

# Handle configuration types
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Set the CMake configuration type to use")
message(STATUS "Using configuration type: ${CMAKE_BUILD_TYPE}")
set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE})

# Position Independent Code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Shared Libraries Configuration
set(CMAKE_INSTALL_RPATH "${ORIGIN}")
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)

# Enable (strict) C++17 features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Release mode flags
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -DNDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -O2 -DNDEBUG")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -rpath -fPIC")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -rpath -fPIC")

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wno-unused-command-line-argument")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wno-unused-command-line-argument")

# Generic flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas -Wno-switch")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unknown-pragmas -Wno-switch")

# Debug mode flags
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(STATUS "Building with development configuration")
    add_definitions(-DDEBUG_BUILD)
endif()

# Set target destination for built targets
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib-static)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

string(ASCII 27 Esc)

# 3rd party libraries
message(STATUS "${Esc}[1mConfiguring target fmt...${Esc}[m")
add_subdirectory("${PROJECT_SOURCE_DIR}/Libs/fmt")

# Targets
add_subdirectory(Logger)
add_subdirectory(RequestLib)
add_subdirectory(JsonViewer)
add_subdirectory(UserInterface)

set(ENABLE_TESTS ON CACHE BOOL "Build the unit tests")
if (ENABLE_TESTS)
    message(STATUS "Unit tests enabled.")
    add_subdirectory(Tests)
endif()
