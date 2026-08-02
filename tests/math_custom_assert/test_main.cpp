#include "custom_assert.hpp"

#include <iostream>

import moldy.math;

int main()
{
    math_custom_assert_fixture::reset();
    volatile float divisor = 0.0F;
    volatile math::float3 result = math::float3{1.0F, 2.0F, 3.0F} / divisor;
    (void)result;

    if (math_custom_assert_fixture::failure_count() != 1)
    {
        std::cerr << "Expected the custom MOLDY_MATH_ASSERT backend to record one failure.\n";
        return 1;
    }
    if (math_custom_assert_fixture::last_message() != "Vector division requires a non-zero scalar.")
    {
        std::cerr << "The custom MOLDY_MATH_ASSERT backend did not receive the expected message.\n";
        return 1;
    }

    std::cout << "Custom math assertion backend was invoked.\n";
    return 0;
}
