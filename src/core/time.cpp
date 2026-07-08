module;

#include <chrono>

module moldy.core;

namespace core
{

SteadyTimePoint steady_now() noexcept
{
    return std::chrono::steady_clock::now();
}

Duration elapsed_since(SteadyTimePoint start) noexcept
{
    return std::chrono::duration_cast<Duration>(std::chrono::steady_clock::now() - start);
}

} // namespace core
