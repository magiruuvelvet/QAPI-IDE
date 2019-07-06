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

# url-cpp
message(STATUS "${Esc}[1m > Configuring sub target url-cpp...${Esc}[m")
file(GLOB_RECURSE SourceList_UrlCpp
    "${PROJECT_SOURCE_DIR}/Libs/url-cpp/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/Libs/url-cpp/include/*.h"
)
add_library(UrlCpp STATIC ${SourceList_UrlCpp})
target_include_directories(UrlCpp PRIVATE "${PROJECT_SOURCE_DIR}/Libs/url-cpp/include")
SetCppStandard(UrlCpp 17)
set_target_properties(UrlCpp PROPERTIES PREFIX "")
set_target_properties(UrlCpp PROPERTIES OUTPUT_NAME "UrlCpp")
message(STATUS "${Esc}[1m > Configured sub target url-cpp.${Esc}[m")


add_library(RequestLib STATIC ${SourceList_RequestLib})

SetCppStandard(RequestLib 17)
set_target_properties(RequestLib PROPERTIES PREFIX "")
set_target_properties(RequestLib PROPERTIES OUTPUT_NAME "RequestLib")

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

message(STATUS "${Esc}[1mConfigured target RequestLib.${Esc}[m")
