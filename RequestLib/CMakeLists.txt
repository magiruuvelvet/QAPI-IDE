# RequestLib

set(CURRENT_TARGET "RequestLib")

message(STATUS "\nConfiguring target ${CURRENT_TARGET}...")

CreateTarget(${CURRENT_TARGET} STATIC RequestLib 17)
IncludeHeaderLibs(${CURRENT_TARGET})

# find OpenSSL
find_package(OpenSSL REQUIRED)

# find zlib
find_package(ZLIB REQUIRED)

target_include_directories(${CURRENT_TARGET}
PRIVATE
    "${PROJECT_SOURCE_DIR}/Libs/cpp-httplib"
    "${PROJECT_SOURCE_DIR}/Libs/url-cpp/include"
    "${OPENSSL_INCLUDE_DIR}"
    "${ZLIB_INCLUDE_DIRS}"
)

set(REQUESTLIB_INCLUDE_DIR "${CURRENT_TARGET_DIR}")
set(REQUESTLIB_INCLUDE_DIR "${CURRENT_TARGET_DIR}" PARENT_SCOPE)
message(STATUS "${CURRENT_TARGET} include directory: ${REQUESTLIB_INCLUDE_DIR}")

target_link_libraries(${CURRENT_TARGET}
PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    ${ZLIB_LIBRARIES}
PRIVATE
    LoggerInterface
    UtilsInterface
    UrlCpp
)

add_library(RequestLibInterface INTERFACE)
target_link_libraries(RequestLibInterface INTERFACE
    ${CURRENT_TARGET}
    OpenSSL::SSL
    OpenSSL::Crypto
    ${ZLIB_LIBRARIES}
    LoggerInterface
    UtilsInterface
    UrlCpp
)
target_include_directories(RequestLibInterface INTERFACE "${REQUESTLIB_INCLUDE_DIR}")

message(STATUS "Configured target RequestLib.\n")
