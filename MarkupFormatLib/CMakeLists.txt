# MarkupFormatLib
#
# Library to work with several markup formats.
# Currently only targeting JSON.
# More formats are planned.
#

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target MarkupFormatLib...${Esc}[m")

file(GLOB_RECURSE SourceList_MarkupFormatLib
    "*.cpp"
    "*.hpp"
)

# nlohmann json library
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory("${PROJECT_SOURCE_DIR}/Libs/json" "${PROJECT_BINARY_DIR}/Libs/json" EXCLUDE_FROM_ALL)

add_library(MarkupFormatLib STATIC ${SourceList_MarkupFormatLib})

SetCppStandard(MarkupFormatLib 17)
set_target_properties(MarkupFormatLib PROPERTIES PREFIX "")
set_target_properties(MarkupFormatLib PROPERTIES OUTPUT_NAME "MarkupFormatLib")

target_include_directories(MarkupFormatLib PRIVATE "${PROJECT_SOURCE_DIR}/MarkupFormatLib")
target_include_directories(MarkupFormatLib PRIVATE "${LOGGER_INCLUDE_DIR}")
set(MARKUPFORMATLIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/MarkupFormatLib" PARENT_SCOPE)

target_link_libraries(MarkupFormatLib
PRIVATE
    nlohmann_json::nlohmann_json
    Logger
    fmt
)

message(STATUS "${Esc}[1mConfigured target MarkupFormatLib.${Esc}[m")
