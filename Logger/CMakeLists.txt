# Logger

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target Logger...${Esc}[m")

file(GLOB_RECURSE SourceList_Logger
    "*.cpp"
    "*.hpp"
)

# fmt formatting library
message(STATUS "${Esc}[1m > Configuring sub target fmt...${Esc}[m")
add_subdirectory("${PROJECT_SOURCE_DIR}/Libs/fmt" "${PROJECT_BINARY_DIR}/Libs/fmt")
message(STATUS "${Esc}[1m > Configured sub target fmt.${Esc}[m")

# globally include fmt headers
include_directories("${PROJECT_SOURCE_DIR}/Libs/fmt/include")


add_library(Logger STATIC ${SourceList_Logger})

SetCppStandard(Logger 17)
set_target_properties(Logger PROPERTIES PREFIX "")
set_target_properties(Logger PROPERTIES OUTPUT_NAME "Logger")

target_include_directories(Logger PRIVATE "${PROJECT_SOURCE_DIR}/Logger")
set(LOGGER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Logger" PARENT_SCOPE)

target_link_libraries(Logger PRIVATE fmt)

message(STATUS "${Esc}[1mConfigured target Logger.${Esc}[m")
