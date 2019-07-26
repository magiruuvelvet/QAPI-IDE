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

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
