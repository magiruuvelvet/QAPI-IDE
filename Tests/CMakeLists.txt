# Unit Tests

set(CURRENT_TARGET "UnitTests")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} EXECUTABLE qapi-ide-tests 17)
IncludeHeaderLibs(${CURRENT_TARGET})

target_link_libraries(${CURRENT_TARGET}
PRIVATE
    Logger
    RequestLib
    MarkupFormatLib
    WorkspaceFormat
    fmt
)

target_include_directories(${CURRENT_TARGET}
PRIVATE
    "${PROJECT_SOURCE_DIR}/Libs/bandit"
    "${REQUESTLIB_INCLUDE_DIR}"
    "${MARKUPFORMATLIB_INCLUDE_DIR}"
    "${WORKSPACEFORMAT_INCLUDE_DIR}"
    "${LOGGER_INCLUDE_DIR}"
)

# required for HTTP request library testing
target_include_directories(${CURRENT_TARGET} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/cpp-httplib")
if (UNIX)
    # libpthread or libpthread-stubs on BSDs
    target_link_libraries(${CURRENT_TARGET} PUBLIC "-lpthread")
endif()

set(UNIT_TEST_TEMPORARY_DIR "${CMAKE_CURRENT_BINARY_DIR}/tmp")
set(UNIT_TEST_REQUESTLIB_CERT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/request-lib")

file(MAKE_DIRECTORY "${UNIT_TEST_TEMPORARY_DIR}")

target_compile_definitions(${CURRENT_TARGET} PRIVATE "-DUNIT_TEST_TEMPORARY_DIR=\"${UNIT_TEST_TEMPORARY_DIR}\"")
target_compile_definitions(${CURRENT_TARGET} PRIVATE "-DUNIT_TEST_REQUESTLIB_CERT_DIR=\"${UNIT_TEST_REQUESTLIB_CERT_DIR}\"")

message(STATUS "Temporary directory for unit tests: ${UNIT_TEST_TEMPORARY_DIR}")
message(STATUS "RequestLib certificate directory: ${UNIT_TEST_REQUESTLIB_CERT_DIR}")

if (ENABLE_SCRIPTING)
    message(STATUS "Scripting enabled in target ${CURRENT_TARGET}.")
    target_include_directories(${CURRENT_TARGET} PRIVATE "${SCRIPTINGINTERFACE_INCLUDE_DIR}")
    target_link_libraries(${CURRENT_TARGET} PRIVATE ScriptingInterface)
    target_compile_definitions(${CURRENT_TARGET} PRIVATE "-DHAS_SCRIPTING_INTERFACE")
endif()

message(STATUS "Configured target ${CURRENT_TARGET}.\n")
