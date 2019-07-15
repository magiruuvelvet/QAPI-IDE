# ScriptingInterface

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target ScriptingInterface...${Esc}[m")

file(GLOB_RECURSE SourceList_ScriptingInterface
    "*.cpp"
    "*.hpp"
)

add_library(ScriptingInterface STATIC ${SourceList_ScriptingInterface})

# C++ scripting support (Macro: SCRIPTING_CPP_SUPPORT)
# https://root.cern.ch/cling-build-instructions
# -DCling_DIR=/path/to/cling/install
option(ENABLE_CLING "Build with C++ scripting support" OFF)
if (ENABLE_CLING)
    find_package(Cling)
    if (LLVM_FOUND)
        message(STATUS "Enabling C++ scripting support...")
        set(LLVM_NO_DEAD_STRIP 1) # keep symbols for JIT resolution
        target_compile_definitions(ScriptingInterface PRIVATE "-DSCRIPTING_CPP_SUPPORT")
    else()
        message(WARNING "C++ support requested, but no cling installation found!")
        set(ENABLE_CLING OFF CACHE "Build with C++ scripting support" INTERNAL FORCE)
    endif()
else()
    message(STATUS "C++ scripting support disabled.")
    set(ENABLE_CLING OFF CACHE "Build with C++ scripting support" INTERNAL FORCE)
endif()

# JavaScript scripting support (Macro: SCRIPTING_JAVASCRIPT_SUPPORT)
option(ENABLE_JAVASCRIPT "Build with JavaScript scripting support" OFF)
if (ENABLE_JAVASCRIPT)
    # NOTE: still deciding on a JavaScript implementation
    # mujs seems pretty lightweight for embedding, just a single file "one.c", but the API sucks
    # in terms of C++17 compatibility, need to manually manage raw pointers and do old C *ugh*
    # v8 is a heavy abomination and also doesn't use CMake, sounds not very healthy to integrate,
    # but has a C++ API; once the build system problem is solved v8 seems like a nice choice
    message(WARNING "Development note: still deciding on what JavaScript VM implementation to use")
    message(WARNING "This option is a stub right now")
    set(ENABLE_JAVASCRIPT OFF CACHE "Build with JavaScript scripting support" INTERNAL FORCE)
else()
    message(STATUS "JavaScript scripting support disabled.")
    set(ENABLE_JAVASCRIPT OFF CACHE "Build with JavaScript scripting support" INTERNAL FORCE)
endif()

if (NOT ENABLE_CLING AND NOT ENABLE_JAVASCRIPT)
    message(WARNING "Scripting support is enabled, but no single language backend is.")
    message(WARNING "Scripting support will be disabled.")
    set(ENABLE_SCRIPTING OFF CACHE "Build with scripting support" INTERNAL FORCE)
endif()

SetCppStandard(ScriptingInterface 17)
set_target_properties(ScriptingInterface PROPERTIES PREFIX "")
set_target_properties(ScriptingInterface PROPERTIES OUTPUT_NAME "ScriptingInterface")

target_include_directories(ScriptingInterface PRIVATE "${PROJECT_SOURCE_DIR}/ScriptingInterface")
target_include_directories(ScriptingInterface PRIVATE "${PROJECT_SOURCE_DIR}/Libs/cpp-httplib")
target_include_directories(ScriptingInterface PRIVATE "${LOGGER_INCLUDE_DIR}")
set(SCRIPTINGINTERFACE_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/ScriptingInterface" PARENT_SCOPE)

target_link_libraries(ScriptingInterface
PRIVATE
    Logger
    fmt
)

if (LLVM_FOUND)
    target_include_directories(ScriptingInterface PRIVATE "${LLVM_INSTALL_PREFIX}/include")
    target_link_libraries(ScriptingInterface PUBLIC clingInterpreter)
    target_compile_definitions(ScriptingInterface PRIVATE "-DLLVMDIR=\"${LLVM_INSTALL_PREFIX}\"")
    set_target_properties(ScriptingInterface PROPERTIES ENABLE_EXPORTS 1)
endif()

message(STATUS "${Esc}[1mConfigured target ScriptingInterface.${Esc}[m")
