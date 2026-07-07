module;

#include <string_view>

#ifndef CORE_BUILD_CONFIGURATION
#define CORE_BUILD_CONFIGURATION "unknown"
#endif

#ifndef CORE_COMPILER_ID
#define CORE_COMPILER_ID "unknown"
#endif

module moldy.core;

namespace core
{

std::string_view build_configuration() noexcept
{
    constexpr std::string_view configuration{CORE_BUILD_CONFIGURATION};
    if constexpr (configuration.empty())
    {
        return "unspecified";
    }

    return configuration;
}

std::string_view compiler_id() noexcept
{
    constexpr std::string_view compiler{CORE_COMPILER_ID};
    if constexpr (compiler.empty())
    {
        return "unknown";
    }

    return compiler;
}

long language_standard() noexcept
{
    return __cplusplus;
}

} // namespace core
