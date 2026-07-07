cmake_minimum_required(VERSION 3.28)

project(moldy LANGUAGES CXX)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

function(configure_project_target target_name)
    set_target_properties(${target_name}
        PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive- /Zc:__cplusplus)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(core STATIC)
add_library(project::core ALIAS core)

target_sources(core
    PRIVATE
        src/core/build_info.cpp
        src/core/status.cpp
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS src/core
        FILES
            src/core/core.cppm
)

target_compile_definitions(core
    PRIVATE
        CORE_BUILD_CONFIGURATION="$<CONFIG>"
        CORE_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
)

configure_project_target(core)

add_executable(smoke
    apps/smoke/main.cpp
)

target_link_libraries(smoke PRIVATE project::core)
configure_project_target(smoke)

add_executable(core_tests
    tests/core/test_main.cpp
)

target_link_libraries(core_tests PRIVATE project::core)
configure_project_target(core_tests)

add_test(NAME core_tests COMMAND core_tests)
