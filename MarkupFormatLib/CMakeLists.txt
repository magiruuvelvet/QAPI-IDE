# MarkupFormatLib
#
# Library to work with several markup formats.
# Currently only targeting JSON.
# More formats are planned.
#

set(CURRENT_TARGET "MarkupFormatLib")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} STATIC MarkupFormatLib 17)
IncludeHeaderLibs(${CURRENT_TARGET})

set(MARKUPFORMATLIB_INCLUDE_DIR "${CURRENT_TARGET_DIR}")
set(MARKUPFORMATLIB_INCLUDE_DIR "${CURRENT_TARGET_DIR}" PARENT_SCOPE)
message(STATUS "${CURRENT_TARGET} include directory: ${MARKUPFORMATLIB_INCLUDE_DIR}")

target_link_libraries(${CURRENT_TARGET}
PRIVATE
    nlohmann_json::nlohmann_json
    LoggerInterface
)

add_library(MarkupFormatLibInterface INTERFACE)
target_link_libraries(MarkupFormatLibInterface INTERFACE
    ${CURRENT_TARGET}
    nlohmann_json::nlohmann_json
    LoggerInterface
)
target_include_directories(MarkupFormatLibInterface INTERFACE "${MARKUPFORMATLIB_INCLUDE_DIR}")

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
