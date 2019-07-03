# Logger

string(ASCII 27 Esc)
message(STATUS "${Esc}[1mConfiguring target Logger...${Esc}[m")

file(GLOB_RECURSE SourceList_Logger
    "*.cpp"
    "*.hpp"
)

add_library(Logger STATIC ${SourceList_Logger})

SetCppStandard(Logger 17)
set_target_properties(Logger PROPERTIES PREFIX "")
set_target_properties(Logger PROPERTIES OUTPUT_NAME "Logger")

include_fmt_lib(Logger)

target_include_directories(Logger PRIVATE "${PROJECT_SOURCE_DIR}/Logger")
set(LOGGER_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/Logger" PARENT_SCOPE)

target_link_libraries(Logger PRIVATE fmt)
