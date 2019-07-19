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
    MarkupFormatLib
    fmt
)

target_include_directories(Tests PRIVATE "${PROJECT_SOURCE_DIR}/Tests")
target_include_directories(Tests PRIVATE "${PROJECT_SOURCE_DIR}/Libs/bandit")
target_include_directories(Tests PRIVATE "${REQUESTLIB_INCLUDE_DIR}")
target_include_directories(Tests PRIVATE "${JSONVIEWER_INCLUDE_DIR}")
target_include_directories(Tests PRIVATE "${LOGGER_INCLUDE_DIR}")

# required for HTTP request library testing
target_include_directories(Tests PRIVATE "${PROJECT_SOURCE_DIR}/Libs/cpp-httplib")
if (UNIX)
    # libpthread or libpthread-stubs on BSDs
    target_link_libraries(Tests PUBLIC "-lpthread")
endif()

set(UNIT_TEST_TEMPORARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tmp")
set(UNIT_TEST_REQUESTLIB_CERT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/request-lib")
file(MAKE_DIRECTORY "${UNIT_TEST_TEMPORARY_DIR}")
target_compile_definitions(Tests PRIVATE "-DUNIT_TEST_TEMPORARY_DIR=\"${UNIT_TEST_TEMPORARY_DIR}\"")
target_compile_definitions(Tests PRIVATE "-DUNIT_TEST_REQUESTLIB_CERT_DIR=\"${UNIT_TEST_REQUESTLIB_CERT_DIR}\"")
message(STATUS "Temporary directory for unit tests: ${UNIT_TEST_TEMPORARY_DIR}")
message(STATUS "RequestLib certificate directory: ${UNIT_TEST_REQUESTLIB_CERT_DIR}")

if (ENABLE_SCRIPTING)
    message(STATUS "Scripting enabled in target Tests.")
    target_include_directories(Tests PRIVATE "${SCRIPTINGINTERFACE_INCLUDE_DIR}")
    target_link_libraries(Tests PRIVATE ScriptingInterface)
    target_compile_definitions(Tests PRIVATE "-DHAS_SCRIPTING_INTERFACE")
endif()

message(STATUS "${Esc}[1mConfigured target Tests.${Esc}[m")
