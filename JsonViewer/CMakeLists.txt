# JsonViewer

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target JsonViewer...${Esc}[m")

file(GLOB_RECURSE SourceList_JsonViewer
    "*.cpp"
    "*.hpp"
)

# nlohmann json library
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory("${PROJECT_SOURCE_DIR}/Libs/json" "${PROJECT_BINARY_DIR}/Libs/json" EXCLUDE_FROM_ALL)

add_library(JsonViewer STATIC ${SourceList_JsonViewer})

SetCppStandard(JsonViewer 17)
set_target_properties(JsonViewer PROPERTIES PREFIX "")
set_target_properties(JsonViewer PROPERTIES OUTPUT_NAME "JsonViewer")

include_fmt_lib(JsonViewer)

target_include_directories(JsonViewer PRIVATE "${PROJECT_SOURCE_DIR}/JsonViewer")
set(JSONVIEWER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/JsonViewer" PARENT_SCOPE)

target_link_libraries(JsonViewer PRIVATE nlohmann_json::nlohmann_json fmt)
