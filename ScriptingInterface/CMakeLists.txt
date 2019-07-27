# ScriptingInterface

set(CURRENT_TARGET "ScriptingInterface")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} STATIC ScriptingInterface 17)
IncludeHeaderLibs(${CURRENT_TARGET})

# C++ scripting support (Macro: SCRIPTING_CPP_SUPPORT)
# https://root.cern.ch/cling-build-instructions
# -DCling_DIR=/path/to/cling/install
option(ENABLE_CLING "Build with C++ scripting support" OFF)
if (ENABLE_CLING)
    find_package(Cling)
    if (LLVM_FOUND)
        message(STATUS "Enabling C++ scripting support...")
        set(LLVM_NO_DEAD_STRIP 1) # keep symbols for JIT resolution
        target_include_directories(${CURRENT_TARGET} PRIVATE "${LLVM_INSTALL_PREFIX}/include")
        target_link_libraries(${CURRENT_TARGET} PUBLIC clingInterpreter)
        target_compile_definitions(${CURRENT_TARGET} PRIVATE "-DLLVMDIR=\"${LLVM_INSTALL_PREFIX}\"")
        set_target_properties(${CURRENT_TARGET} PROPERTIES ENABLE_EXPORTS 1)
        target_compile_definitions(${CURRENT_TARGET} PRIVATE "-DSCRIPTING_CPP_SUPPORT")
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
    target_include_directories(${CURRENT_TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/duktape")
    target_include_directories(${CURRENT_TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/duktape-cpp/src")
    add_library(duktape STATIC "${PROJECT_SOURCE_DIR}/Libs/duktape/duktape.c")
    target_link_libraries(${CURRENT_TARGET} PRIVATE duktape)
    target_compile_definitions(${CURRENT_TARGET} PRIVATE "-DSCRIPTING_JAVASCRIPT_SUPPORT")
else()
    message(STATUS "JavaScript scripting support disabled.")
    set(ENABLE_JAVASCRIPT OFF CACHE "Build with JavaScript scripting support" INTERNAL FORCE)
endif()

if (NOT ENABLE_CLING AND NOT ENABLE_JAVASCRIPT)
    message(WARNING "Scripting support is enabled, but no single language backend is.")
    message(WARNING "Scripting support will be disabled.")
    set(ENABLE_SCRIPTING OFF CACHE "Build with scripting support" INTERNAL FORCE)
endif()

set(SCRIPTINGINTERFACE_INCLUDE_DIR "${CURRENT_TARGET_DIR}")
set(SCRIPTINGINTERFACE_INCLUDE_DIR "${CURRENT_TARGET_DIR}" PARENT_SCOPE)
message(STATUS "${CURRENT_TARGET} include directory: ${SCRIPTINGINTERFACE_INCLUDE_DIR}")

target_link_libraries(${CURRENT_TARGET}
PRIVATE
    LoggerInterface
)

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
