#include "mock_assert.hpp"

#include <cstdio>
#include <cstdlib>

namespace math_assertion_fixture
{

void handle_failure(const char* message) noexcept
{
    std::fputs("Mock math assertion: ", stderr);
    std::fputs(message, stderr);
    std::fputc('\n', stderr);
    std::fflush(stderr);
    std::exit(failure_exit_code);
}

} // namespace math_assertion_fixture
