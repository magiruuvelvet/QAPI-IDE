# Utils
#
# General utility and helper functions
# to wrap some longer C++ code into a
# one liner.
#

set(CURRENT_TARGET "Utils")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} STATIC Utils 17)
IncludeHeaderLibs(${CURRENT_TARGET})

set(UTILS_INCLUDE_DIR "${CURRENT_TARGET_DIR}")
set(UTILS_INCLUDE_DIR "${CURRENT_TARGET_DIR}" PARENT_SCOPE)
message(STATUS "${CURRENT_TARGET} include directory: ${UTILS_INCLUDE_DIR}")

add_library(UtilsInterface INTERFACE)
target_include_directories(UtilsInterface INTERFACE "${UTILS_INCLUDE_DIR}")

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
