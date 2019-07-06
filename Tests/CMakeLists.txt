# Unit Tests

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target Tests...${Esc}[m")

file(GLOB_RECURSE SourceList_Tests
    "*.cpp"
    "*.hpp"
)

add_executable(Tests ${SourceList_Tests})

SetCppStandard(Tests 17)
set_target_properties(Tests PROPERTIES PREFIX "")
set_target_properties(Tests PROPERTIES OUTPUT_NAME "qapi-ide-tests")

target_link_libraries(Tests
PRIVATE
    Logger
    RequestLib
    JsonViewer
    fmt
)

target_include_directories(Tests PRIVATE "${PROJECT_SOURCE_DIR}/Tests")
target_include_directories(Tests PRIVATE "${PROJECT_SOURCE_DIR}/Libs/bandit")
target_include_directories(Tests PRIVATE "${REQUESTLIB_INCLUDE_DIR}")
target_include_directories(Tests PRIVATE "${JSONVIEWER_INCLUDE_DIR}")
target_include_directories(Tests PRIVATE "${LOGGER_INCLUDE_DIR}")

message(STATUS "${Esc}[1mConfigured target Tests.${Esc}[m")
