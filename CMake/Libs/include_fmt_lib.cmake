macro (include_fmt_lib TargetName)
    target_include_directories(${TargetName} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/fmt/include")
endmacro()
