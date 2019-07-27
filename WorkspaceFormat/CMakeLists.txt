# WorkspaceFormat

set(CURRENT_TARGET "WorkspaceFormat")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} STATIC WorkspaceFormat 17)
IncludeHeaderLibs(${CURRENT_TARGET})

set(WORKSPACEFORMAT_INCLUDE_DIR "${CURRENT_TARGET_DIR}")
set(WORKSPACEFORMAT_INCLUDE_DIR "${CURRENT_TARGET_DIR}" PARENT_SCOPE)
message(STATUS "${CURRENT_TARGET} include directory: ${WORKSPACEFORMAT_INCLUDE_DIR}")

target_link_libraries(${CURRENT_TARGET}
PRIVATE
    nlohmann_json::nlohmann_json
    LoggerInterface
)

add_library(WorkspaceFormatInterface INTERFACE)
target_link_libraries(WorkspaceFormatInterface INTERFACE
    ${CURRENT_TARGET}
    nlohmann_json::nlohmann_json
    LoggerInterface
)
target_include_directories(WorkspaceFormatInterface INTERFACE "${WORKSPACEFORMAT_INCLUDE_DIR}")

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
