cmake_minimum_required(VERSION 3.28)

project(moldy LANGUAGES CXX)

enable_testing()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(MOLDY_MATH_ASSERT_BACKEND "core" CACHE STRING "Assertion backend used while building moldy.math")
set_property(CACHE MOLDY_MATH_ASSERT_BACKEND PROPERTY STRINGS core standalone custom)
set(MOLDY_MATH_ASSERT_HEADER "" CACHE STRING "Header included by the custom moldy.math assertion backend")
option(MOLDY_MATH_BUILD_CUSTOM_ASSERT_FIXTURE "Build the repository custom math assertion fixture" OFF)

set(_moldy_math_assert_backends core standalone custom)
if(NOT MOLDY_MATH_ASSERT_BACKEND IN_LIST _moldy_math_assert_backends)
    message(FATAL_ERROR
        "Unsupported MOLDY_MATH_ASSERT_BACKEND='${MOLDY_MATH_ASSERT_BACKEND}'. "
        "Expected one of: core, standalone, custom."
    )
endif()

if(MOLDY_MATH_ASSERT_BACKEND STREQUAL "custom" AND MOLDY_MATH_ASSERT_HEADER STREQUAL "")
    message(FATAL_ERROR
        "MOLDY_MATH_ASSERT_HEADER is required when MOLDY_MATH_ASSERT_BACKEND=custom."
    )
endif()

if(MOLDY_MATH_BUILD_CUSTOM_ASSERT_FIXTURE AND NOT MOLDY_MATH_ASSERT_BACKEND STREQUAL "custom")
    message(FATAL_ERROR
        "MOLDY_MATH_BUILD_CUSTOM_ASSERT_FIXTURE requires MOLDY_MATH_ASSERT_BACKEND=custom."
    )
endif()

function(configure_project_target target_name)
    set_target_properties(${target_name}
        PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
    )

    if(MSVC)
        target_compile_options(${target_name} PRIVATE /W4 /permissive- /Zc:__cplusplus /Zc:preprocessor)
    else()
        target_compile_options(${target_name} PRIVATE -Wall -Wextra -Wpedantic)
    endif()
endfunction()

add_library(core STATIC)
add_library(project::core ALIAS core)

target_sources(core
    PRIVATE
        src/core/build_info.cpp
        src/core/lifecycle.cpp
        src/core/logging.cpp
        src/core/status.cpp
        src/core/time.cpp
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS src/core
        FILES
            src/core/core.cppm
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS src/core/include
        FILES
            src/core/include/moldy/core_macros.hpp
)

target_compile_definitions(core
    PUBLIC
        $<$<CONFIG:Debug>:MOLDY_ENABLE_ASSERTS>
        $<$<CONFIG:RelWithDebInfo>:MOLDY_ENABLE_ASSERTS>
    PRIVATE
        CORE_BUILD_CONFIGURATION="$<CONFIG>"
        CORE_COMPILER_ID="${CMAKE_CXX_COMPILER_ID}"
)

configure_project_target(core)

add_library(math STATIC)
add_library(project::math ALIAS math)

target_sources(math
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS src/math
        FILES
            src/math/math.cppm
)

configure_project_target(math)

if(MOLDY_MATH_ASSERT_BACKEND STREQUAL "core")
    target_compile_definitions(math PRIVATE MOLDY_MATH_ASSERT_BACKEND_CORE)
    target_link_libraries(math PRIVATE project::core)
elseif(MOLDY_MATH_ASSERT_BACKEND STREQUAL "standalone")
    target_compile_definitions(math
        PRIVATE
            MOLDY_MATH_ASSERT_BACKEND_STANDALONE
            $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:MOLDY_MATH_ENABLE_ASSERTS>
    )
else()
    target_compile_definitions(math
        PRIVATE
            MOLDY_MATH_ASSERT_BACKEND_CUSTOM
            MOLDY_MATH_ASSERT_HEADER="${MOLDY_MATH_ASSERT_HEADER}"
    )
endif()

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

add_executable(math_tests
    tests/math/test_main.cpp
)

target_link_libraries(math_tests PRIVATE project::math)
configure_project_target(math_tests)

add_test(NAME math_tests COMMAND math_tests)

add_library(math_assertion_fixture_backend STATIC
    tests/math_assertions/mock_assert.cpp
)

configure_project_target(math_assertion_fixture_backend)

add_library(math_assertion_fixture STATIC)

target_sources(math_assertion_fixture
    PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS src/math
        FILES
            src/math/math.cppm
)

file(TO_CMAKE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/tests/math_assertions/mock_assert.hpp"
    _moldy_math_assert_fixture_header
)

target_compile_definitions(math_assertion_fixture
    PRIVATE
        MOLDY_MATH_ASSERT_BACKEND_CUSTOM
        MOLDY_MATH_ASSERT_HEADER="${_moldy_math_assert_fixture_header}"
)

target_link_libraries(math_assertion_fixture PRIVATE math_assertion_fixture_backend)
configure_project_target(math_assertion_fixture)

add_executable(math_assertion_tests
    tests/math_assertions/test_main.cpp
)

target_link_libraries(math_assertion_tests PRIVATE math_assertion_fixture)
configure_project_target(math_assertion_tests)

foreach(assertion_case
        zero_divisor
        zero_vector_length
        zero_axis
        zero_quaternion_normalize
        zero_quaternion_inverse
        zero_quaternion_rotate
        zero_quaternion_to_matrix
        non_orthonormal_matrix
        reflection_matrix)
    add_test(
        NAME math_assertion_${assertion_case}
        COMMAND math_assertion_tests --expect-failure ${assertion_case}
    )
endforeach()

if(MOLDY_MATH_BUILD_CUSTOM_ASSERT_FIXTURE)
    add_library(math_custom_assert_fixture_backend STATIC
        tests/math_custom_assert/backend.cpp
    )

    configure_project_target(math_custom_assert_fixture_backend)
    target_link_libraries(math PRIVATE math_custom_assert_fixture_backend)

    add_executable(math_custom_assert_tests
        tests/math_custom_assert/test_main.cpp
    )

    target_link_libraries(math_custom_assert_tests PRIVATE project::math)
    configure_project_target(math_custom_assert_tests)

    add_test(NAME math_custom_assert_tests COMMAND math_custom_assert_tests)
endif()

add_executable(math_policy_tests
    tests/math_policy/test_main.cpp
)

configure_project_target(math_policy_tests)

add_test(NAME math_policy_tests COMMAND math_policy_tests)
