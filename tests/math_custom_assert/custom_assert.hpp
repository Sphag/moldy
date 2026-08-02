#pragma once

#include <string_view>

namespace math_custom_assert_fixture
{

void handle_failure(const char* message) noexcept;
void reset() noexcept;
[[nodiscard]] int failure_count() noexcept;
[[nodiscard]] std::string_view last_message() noexcept;

} // namespace math_custom_assert_fixture

#define MOLDY_MATH_ASSERT(expression, message)                                                                         \
    ((expression) ? (void)0 : ::math_custom_assert_fixture::handle_failure(message))
