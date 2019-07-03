# User Interface

include(SetCppStandard)

file(GLOB_RECURSE SourceList_UserInterface
    "*.cpp"
    "*.hpp"
)

add_executable(UserInterface ${SourceList_UserInterface})

SetCppStandard(UserInterface 17)
set_target_properties(UserInterface PROPERTIES PREFIX "")
set_target_properties(UserInterface PROPERTIES OUTPUT_NAME "qapi-ide")

include_fmt_lib(UserInterface)

target_link_libraries(UserInterface
PRIVATE
    RequestLib
    JsonViewer
    fmt
)

target_include_directories(UserInterface PRIVATE "${PROJECT_SOURCE_DIR}/UserInterface")
target_include_directories(UserInterface PRIVATE "${REQUESTLIB_INCLUDE_DIR}")
target_include_directories(UserInterface PRIVATE "${JSONVIEWER_INCLUDE_DIR}")
