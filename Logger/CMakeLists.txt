# Logger

set(CURRENT_TARGET "Logger")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} STATIC Logger 17)
IncludeHeaderLibs(${CURRENT_TARGET})

set(LOGGER_INCLUDE_DIR "${CURRENT_TARGET_DIR}")
set(LOGGER_INCLUDE_DIR "${CURRENT_TARGET_DIR}" PARENT_SCOPE)
message(STATUS "${CURRENT_TARGET} include directory: ${LOGGER_INCLUDE_DIR}")

# operating system details
configure_file("logger/version.hpp.in" "${GENERATED_DIR}/logger/version.hpp")

target_link_libraries(${CURRENT_TARGET} PRIVATE fmt)

add_library(LoggerInterface INTERFACE)
target_link_libraries(LoggerInterface INTERFACE ${CURRENT_TARGET} fmt)
target_include_directories(LoggerInterface INTERFACE "${LOGGER_INCLUDE_DIR}")

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
