module;

#include <string>
#include <string_view>
#include <utility>

module moldy.core;

namespace core
{

LogRecord::LogRecord(ELogLevel level, std::string category, std::string message)
    : level_(level), category_(std::move(category)), message_(std::move(message))
{
}

ELogLevel LogRecord::level() const noexcept
{
    return level_;
}

std::string_view LogRecord::category() const noexcept
{
    return category_;
}

std::string_view LogRecord::message() const noexcept
{
    return message_;
}

std::string_view log_level_name(ELogLevel level) noexcept
{
    switch (level)
    {
    case ELogLevel::Trace:
        return "trace";
    case ELogLevel::Debug:
        return "debug";
    case ELogLevel::Info:
        return "info";
    case ELogLevel::Warning:
        return "warning";
    case ELogLevel::Error:
        return "error";
    case ELogLevel::Critical:
        return "critical";
    }

    return "unknown";
}

} // namespace core
