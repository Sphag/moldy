#include "mock_assert.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>

#if !defined(_WIN32)
#include <sys/wait.h>
#endif

import moldy.math;

namespace
{

int trigger_assertion(std::string_view assertion_case)
{
    if (assertion_case == "zero_divisor")
    {
        volatile float divisor = 0.0F;
        volatile math::float3 result = math::float3{1.0F, 2.0F, 3.0F} / divisor;
        (void)result;
        return 0;
    }
    if (assertion_case == "zero_vector_length")
    {
        volatile math::float3 result = math::normalize(math::float3{});
        (void)result;
        return 0;
    }
    if (assertion_case == "zero_axis")
    {
        volatile math::quaternion result = math::quaternion_from_axis_angle(math::float3{}, 1.0F);
        (void)result;
        return 0;
    }
    if (assertion_case == "zero_quaternion_normalize")
    {
        volatile math::quaternion result = math::normalize(math::quaternion{});
        (void)result;
        return 0;
    }
    if (assertion_case == "zero_quaternion_inverse")
    {
        volatile math::quaternion result = math::inverse(math::quaternion{});
        (void)result;
        return 0;
    }
    if (assertion_case == "zero_quaternion_rotate")
    {
        volatile math::float3 result = math::rotate(math::quaternion{}, math::float3{1.0F, 0.0F, 0.0F});
        (void)result;
        return 0;
    }
    if (assertion_case == "zero_quaternion_to_matrix")
    {
        volatile math::float3x3 result = math::quaternion_to_float3x3(math::quaternion{});
        (void)result;
        return 0;
    }
    if (assertion_case == "non_orthonormal_matrix")
    {
        const math::float3x3 scaled{{2.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}};
        volatile math::quaternion result = math::float3x3_to_quaternion(scaled, 0.0001F, 0.0001F);
        (void)result;
        return 0;
    }
    if (assertion_case == "reflection_matrix")
    {
        const math::float3x3 reflection{{-1.0F, 0.0F, 0.0F}, {0.0F, 1.0F, 0.0F}, {0.0F, 0.0F, 1.0F}};
        volatile math::quaternion result = math::float3x3_to_quaternion(reflection, 10.0F, 10.0F);
        (void)result;
        return 0;
    }

    std::cerr << "Unknown assertion case: " << assertion_case << '\n';
    return 2;
}

bool exited_through_mock_assertion(int result)
{
#if defined(_WIN32)
    return result == math_assertion_fixture::failure_exit_code;
#else
    return WIFEXITED(result) && WEXITSTATUS(result) == math_assertion_fixture::failure_exit_code;
#endif
}

int expect_mock_assertion(const char* executable, std::string_view assertion_case)
{
    std::string command = "\"" + std::string{executable} + "\" --trigger ";
    command += assertion_case;

    const int result = std::system(command.c_str());
    if (result == -1)
    {
        std::cerr << "Failed to launch assertion subprocess for " << assertion_case << ".\n";
        return 1;
    }
    if (!exited_through_mock_assertion(result))
    {
        std::cerr << "Expected the assertion subprocess for " << assertion_case
                  << " to exit through the mock backend, but its status was " << result << ".\n";
        return 1;
    }

    std::cout << "Mock assertion observed for " << assertion_case << ".\n";
    return 0;
}

} // namespace

int main(int argc, char** argv)
{
    if (argc == 3 && std::string_view{argv[1]} == "--expect-failure")
    {
        return expect_mock_assertion(argv[0], argv[2]);
    }
    if (argc == 3 && std::string_view{argv[1]} == "--trigger")
    {
        return trigger_assertion(argv[2]);
    }

    std::cerr << "Usage: math_assertion_tests --expect-failure|--trigger <case>\n";
    return 2;
}
