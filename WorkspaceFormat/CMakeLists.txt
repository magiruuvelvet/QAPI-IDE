# WorkspaceFormat

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target WorkspaceFormat...${Esc}[m")

file(GLOB_RECURSE SourceList_WorkspaceFormat
    "*.cpp"
    "*.hpp"
)

add_library(WorkspaceFormat STATIC ${SourceList_WorkspaceFormat})

SetCppStandard(WorkspaceFormat 17)
set_target_properties(WorkspaceFormat PROPERTIES PREFIX "")
set_target_properties(WorkspaceFormat PROPERTIES OUTPUT_NAME "WorkspaceFormat")

target_include_directories(WorkspaceFormat PRIVATE "${PROJECT_SOURCE_DIR}/WorkspaceFormat")
target_include_directories(WorkspaceFormat PRIVATE "${LOGGER_INCLUDE_DIR}")
set(WORKSPACEFORMAT_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/WorkspaceFormat" PARENT_SCOPE)

target_link_libraries(WorkspaceFormat
PRIVATE
    nlohmann_json::nlohmann_json
    Logger
    fmt
)

message(STATUS "${Esc}[1mConfigured target WorkspaceFormat.${Esc}[m")
