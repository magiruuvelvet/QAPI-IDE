# RequestLib

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target RequestLib...${Esc}[m")

file(GLOB_RECURSE SourceList_RequestLib
    "*.cpp"
    "*.hpp"
)

# find curl
find_package(CURL REQUIRED)
if (NOT CURL_FOUND)
    message(FATAL_ERROR "libcurl was not found!")
endif()

# print basic curl info
message(STATUS "curl include: ${CURL_INCLUDE_DIRS}")
message(STATUS "curl library: ${CURL_LIBRARIES}")
message(STATUS "curl version: ${CURL_VERSION_STRING}")

add_library(RequestLib STATIC ${SourceList_RequestLib})

SetCppStandard(RequestLib 17)
set_target_properties(RequestLib PROPERTIES PREFIX "")
set_target_properties(RequestLib PROPERTIES OUTPUT_NAME "RequestLib")

include_fmt_lib(RequestLib)

target_include_directories(RequestLib PRIVATE "${PROJECT_SOURCE_DIR}/RequestLib")
target_include_directories(RequestLib PRIVATE "${CURL_INCLUDE_DIRS}")
set(REQUESTLIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/RequestLib" PARENT_SCOPE)

target_link_libraries(RequestLib
PUBLIC
    "${CURL_LIBRARIES}"
PRIVATE
    fmt
)
