#include "custom_assert.hpp"

namespace math_custom_assert_fixture
{

namespace
{

int recorded_failure_count = 0;
std::string_view recorded_message;

} // namespace

void handle_failure(const char* message) noexcept
{
    ++recorded_failure_count;
    recorded_message = message;
}

void reset() noexcept
{
    recorded_failure_count = 0;
    recorded_message = {};
}

int failure_count() noexcept
{
    return recorded_failure_count;
}

std::string_view last_message() noexcept
{
    return recorded_message;
}

} // namespace math_custom_assert_fixture
