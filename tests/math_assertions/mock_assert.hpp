#pragma once

namespace math_assertion_fixture
{

inline constexpr int failure_exit_code = 86;

[[noreturn]] void handle_failure(const char* message) noexcept;

} // namespace math_assertion_fixture

#define MOLDY_MATH_ASSERT(expression, message)                                                                         \
    ((expression) ? (void)0 : ::math_assertion_fixture::handle_failure(message))
