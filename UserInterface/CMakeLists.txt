# User Interface

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target UserInterface...${Esc}[m")

file(GLOB_RECURSE SourceList_UserInterface
    "*.cpp"
    "*.hpp"
)

# find Qt
message(STATUS "Finding Qt...")
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Widgets REQUIRED)

# print Qt version information
message(STATUS "Qt version: ${Qt5Core_VERSION}")

# Qt automoc
message(STATUS "Enabling Qt automoc...")
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# Handle Qt qrc files automatically
message(STATUS "Enabling Qt autorcc...")
set(CMAKE_AUTORCC ON)

# Qt deprecated warnings
add_definitions(-DQT_DEPRECATED_WARNINGS)
add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)

# disable foreach macro
add_definitions(-DQT_NO_FOREACH)

add_executable(UserInterface ${SourceList_UserInterface})

SetCppStandard(UserInterface 17)
set_target_properties(UserInterface PROPERTIES PREFIX "")
set_target_properties(UserInterface PROPERTIES OUTPUT_NAME "qapi-ide")

target_link_libraries(UserInterface
PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
PRIVATE
    Logger
    RequestLib
    JsonViewer
    fmt
)

target_include_directories(UserInterface PRIVATE "${PROJECT_SOURCE_DIR}/UserInterface")
target_include_directories(UserInterface PRIVATE "${REQUESTLIB_INCLUDE_DIR}")
target_include_directories(UserInterface PRIVATE "${JSONVIEWER_INCLUDE_DIR}")
target_include_directories(UserInterface PRIVATE "${LOGGER_INCLUDE_DIR}")

message(STATUS "${Esc}[1mConfigured target UserInterface.${Esc}[m")
