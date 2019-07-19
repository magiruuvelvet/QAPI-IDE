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
find_package(Qt5LinguistTools)

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

# translations (optimal, only built when linguist tools are found)
# TODO: should a cmake option be added for this? -DBUILD_TRANSLATIONS
if (Qt5LinguistTools_FOUND)
    message(STATUS "Qt linguist tools found. Enable building of translations...")

    file(GLOB_RECURSE UI_TRANSLATION_FILES
        "i18n/*.ts"
    )

    # set output location of built translation files
    set_source_files_properties(${UI_TRANSLATION_FILES} PROPERTIES OUTPUT_LOCATION "${CMAKE_CURRENT_BINARY_DIR}/i18n")

    # copy qrc file
    configure_file("i18n/translations.qrc" "${CMAKE_CURRENT_BINARY_DIR}/i18n/translations.qrc" COPYONLY)

    qt5_create_translation(QM_FILES
        ${SourceList_UserInterface} # source files; tr()
        ${UI_TRANSLATION_FILES}     # translation files (.ts)
    )

    qt5_add_resources(RCC_TRANSLATION_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/i18n/translations.qrc")
    add_custom_target(GenerateEmbeddedTranslations DEPENDS ${RCC_TRANSLATION_SOURCES})
else()
    message(WARNING "Qt linguist tools not found. Translations will be skipped.")

    # create empty variables to satisfy the target dependencies
    set(RCC_TRANSLATION_SOURCES "")
    set(QM_FILES "")
    add_custom_target(GenerateEmbeddedTranslations)
endif()

add_executable(UserInterface ${SourceList_UserInterface} ${RCC_TRANSLATION_SOURCES} ${QM_FILES})
add_dependencies(UserInterface GenerateEmbeddedTranslations)

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
    MarkupFormatLib
    fmt
)

target_include_directories(UserInterface PRIVATE "${PROJECT_SOURCE_DIR}/UserInterface")
target_include_directories(UserInterface PRIVATE "${REQUESTLIB_INCLUDE_DIR}")
target_include_directories(UserInterface PRIVATE "${JSONVIEWER_INCLUDE_DIR}")
target_include_directories(UserInterface PRIVATE "${LOGGER_INCLUDE_DIR}")

if (ENABLE_SCRIPTING)
    message(STATUS "Scripting enabled in target UserInterface.")
    target_include_directories(UserInterface PRIVATE "${SCRIPTINGINTERFACE_INCLUDE_DIR}")
    target_link_libraries(UserInterface PRIVATE ScriptingInterface)
    target_compile_definitions(UserInterface PRIVATE "-DHAS_SCRIPTING_INTERFACE")
endif()

message(STATUS "${Esc}[1mConfigured target UserInterface.${Esc}[m")
