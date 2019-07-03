# RequestLib

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target RequestLib...${Esc}[m")

file(GLOB_RECURSE SourceList_RequestLib
    "*.cpp"
    "*.hpp"
)

# find OpenSSL
find_package(OpenSSL REQUIRED)

# find zlib
find_package(ZLIB REQUIRED)

add_library(RequestLib STATIC ${SourceList_RequestLib})

SetCppStandard(RequestLib 17)
set_target_properties(RequestLib PROPERTIES PREFIX "")
set_target_properties(RequestLib PROPERTIES OUTPUT_NAME "RequestLib")

include_fmt_lib(RequestLib)
include_enum_lib(RequestLib)

target_include_directories(RequestLib PRIVATE "${PROJECT_SOURCE_DIR}/RequestLib")
target_include_directories(RequestLib PRIVATE "${PROJECT_SOURCE_DIR}/Libs/cpp-httplib")
target_include_directories(RequestLib PRIVATE "${PROJECT_SOURCE_DIR}/Libs/url-cpp/include")
target_include_directories(RequestLib PRIVATE "${OPENSSL_INCLUDE_DIR}")
target_include_directories(RequestLib PRIVATE "${ZLIB_INCLUDE_DIRS}")
target_include_directories(RequestLib PRIVATE "${LOGGER_INCLUDE_DIR}")
set(REQUESTLIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/RequestLib" PARENT_SCOPE)

target_link_libraries(RequestLib
PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    ${ZLIB_LIBRARIES}
PRIVATE
    Logger
    UrlCpp
    fmt
)
