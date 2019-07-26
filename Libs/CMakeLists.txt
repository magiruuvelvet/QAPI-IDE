# Dependencies

message(STATUS "\nConfiguring dependencies...")

# fmt formatting library
message(STATUS " ===> fmt")
add_subdirectory("${PROJECT_SOURCE_DIR}/Libs/fmt" "${PROJECT_BINARY_DIR}/Libs/fmt")
include_directories("${PROJECT_SOURCE_DIR}/Libs/fmt/include")

# frozen constexpr containers
message(STATUS " ===> frozen")
include_directories("${PROJECT_SOURCE_DIR}/Libs/frozen/include")

# nlohmann json library
message(STATUS " ===> json")
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory("${PROJECT_SOURCE_DIR}/Libs/json" "${PROJECT_BINARY_DIR}/Libs/json" EXCLUDE_FROM_ALL)

# url-cpp
message(STATUS " ===> url-cpp")
file(GLOB_RECURSE UrlCppSourceList
    "${PROJECT_SOURCE_DIR}/Libs/url-cpp/src/*.cpp"
    "${PROJECT_SOURCE_DIR}/Libs/url-cpp/include/*.h"
)
add_library(UrlCpp STATIC ${UrlCppSourceList})
SetCppStandard(UrlCpp 17)
target_include_directories(UrlCpp PRIVATE "${PROJECT_SOURCE_DIR}/Libs/url-cpp/include")

message(STATUS "Configured dependencies.\n")

# header-only library include macro
macro(IncludeHeaderLibs TargetName)
    target_include_directories(${TargetName} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/fmt/include")
    target_include_directories(${TargetName} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/frozen/include")
endmacro()
