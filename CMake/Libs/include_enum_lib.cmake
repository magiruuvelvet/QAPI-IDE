macro (include_enum_lib TargetName)
    target_include_directories(${TargetName} PRIVATE "${PROJECT_SOURCE_DIR}/Libs/enum")
endmacro()
