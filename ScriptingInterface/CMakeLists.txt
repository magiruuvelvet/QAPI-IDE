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
        target_include_directories(ScriptingInterface PRIVATE "${LLVM_INSTALL_PREFIX}/include")
        target_link_libraries(ScriptingInterface PUBLIC clingInterpreter)
        target_compile_definitions(ScriptingInterface PRIVATE "-DLLVMDIR=\"${LLVM_INSTALL_PREFIX}\"")
        set_target_properties(ScriptingInterface PROPERTIES ENABLE_EXPORTS 1)
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
# powered by duktape (https://duktape.org)
# with the help of those nice C++14 bindings (https://github.com/vmanucharyan/duktape-cpp)
# the bindings are a bit older already and not maintained anymore, but
# they are enough for the use case of this application, so use them :)
option(ENABLE_JAVASCRIPT "Build with JavaScript scripting support" OFF)
if (ENABLE_JAVASCRIPT)
    message(STATUS "Enabling JavaScript scripting support...")
    target_include_directories(ScriptingInterface PRIVATE "${PROJECT_SOURCE_DIR}/Libs/duktape")
    target_include_directories(ScriptingInterface PRIVATE "${PROJECT_SOURCE_DIR}/Libs/duktape-cpp/src")
    add_library(duktape STATIC "${PROJECT_SOURCE_DIR}/Libs/duktape/duktape.c")
    target_link_libraries(ScriptingInterface PRIVATE duktape)
    target_compile_definitions(ScriptingInterface PRIVATE "-DSCRIPTING_JAVASCRIPT_SUPPORT")
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

message(STATUS "${Esc}[1mConfigured target ScriptingInterface.${Esc}[m")
