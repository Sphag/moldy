module;

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#if defined(_MSC_VER)
#include <intrin.h>
#endif

module moldy.core;

namespace core
{
namespace
{

std::mutex gLoggingMutex;
std::shared_ptr<Logger> gLogger;

int log_level_rank(ELogLevel level) noexcept
{
    return static_cast<int>(level);
}

std::string format_log_record(const LogRecord& record)
{
    std::ostringstream stream;
    stream << '[' << log_level_name(record.level()) << ']';

    if (!record.category().empty())
    {
        stream << '[' << record.category() << ']';
    }

    stream << ' ' << record.message();

    const LogSourceLocation& sourceLocation = record.source_location();
    if (!sourceLocation.empty())
    {
        stream << " (" << sourceLocation.file() << ':' << sourceLocation.line();
        if (!sourceLocation.function().empty())
        {
            stream << ' ' << sourceLocation.function();
        }
        stream << ')';
    }

    return stream.str();
}

std::string assertion_message(std::string_view expression, std::string_view message)
{
    std::string result = "Assertion failed";

    if (!expression.empty())
    {
        result += ": ";
        result += expression;
    }

    if (!message.empty())
    {
        result += " - ";
        result += message;
    }

    return result;
}

} // namespace

LogSourceLocation::LogSourceLocation() = default;

LogSourceLocation::LogSourceLocation(std::string file, int line, std::string function)
    : file_(std::move(file)), line_(line), function_(std::move(function))
{
}

std::string_view LogSourceLocation::file() const noexcept
{
    return file_;
}

int LogSourceLocation::line() const noexcept
{
    return line_;
}

std::string_view LogSourceLocation::function() const noexcept
{
    return function_;
}

bool LogSourceLocation::empty() const noexcept
{
    return file_.empty() && function_.empty() && line_ == 0;
}

LogRecord::LogRecord(ELogLevel level, std::string category, std::string message)
    : LogRecord(level, std::move(category), std::move(message), steady_now(), LogSourceLocation{})
{
}

LogRecord::LogRecord(ELogLevel level, std::string category, std::string message, SteadyTimePoint timestamp,
                     LogSourceLocation sourceLocation)
    : level_(level), category_(std::move(category)), message_(std::move(message)), timestamp_(timestamp),
      source_location_(std::move(sourceLocation))
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

SteadyTimePoint LogRecord::timestamp() const noexcept
{
    return timestamp_;
}

const LogSourceLocation& LogRecord::source_location() const noexcept
{
    return source_location_;
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

ILogSink::~ILogSink() = default;

ConsoleLogSink::ConsoleLogSink(EConsoleStream stream) : stream_(stream) {}

Status ConsoleLogSink::write(const LogRecord& record)
{
    std::ostream& output = stream_ == EConsoleStream::StandardOutput ? std::cout : std::cerr;
    output << format_log_record(record) << '\n';

    if (!output)
    {
        return Status::error(EErrorCode::Internal, "Failed to write log record to console sink.");
    }

    return Status::success();
}

FileLogSink::FileLogSink(std::string path, bool append) : path_(std::move(path)), open_status_(Status::success())
{
    const std::ios_base::openmode mode = append ? std::ios_base::app : std::ios_base::trunc;
    stream_.open(path_, std::ios_base::out | mode);

    if (!stream_.is_open())
    {
        open_status_ = Status::error(EErrorCode::InvalidArgument, "Failed to open log file sink.");
    }
}

std::string_view FileLogSink::path() const noexcept
{
    return path_;
}

Status FileLogSink::open_status() const
{
    return open_status_;
}

Status FileLogSink::write(const LogRecord& record)
{
    std::lock_guard lock{mutex_};

    if (!open_status_.ok())
    {
        return open_status_;
    }

    stream_ << format_log_record(record) << '\n';
    stream_.flush();

    if (!stream_)
    {
        return Status::error(EErrorCode::Internal, "Failed to write log record to file sink.");
    }

    return Status::success();
}

Status InMemoryLogSink::write(const LogRecord& record)
{
    std::lock_guard lock{mutex_};
    records_.push_back(record);
    return Status::success();
}

std::vector<LogRecord> InMemoryLogSink::records() const
{
    std::lock_guard lock{mutex_};
    return records_;
}

std::size_t InMemoryLogSink::size() const
{
    std::lock_guard lock{mutex_};
    return records_.size();
}

void InMemoryLogSink::clear()
{
    std::lock_guard lock{mutex_};
    records_.clear();
}

Logger::Logger(ELogLevel minimumLevel) : minimum_level_(minimumLevel) {}

void Logger::set_minimum_level(ELogLevel minimumLevel)
{
    std::lock_guard lock{mutex_};
    minimum_level_ = minimumLevel;
}

ELogLevel Logger::minimum_level() const
{
    std::lock_guard lock{mutex_};
    return minimum_level_;
}

bool Logger::should_log(ELogLevel level) const
{
    std::lock_guard lock{mutex_};
    return log_level_rank(level) >= log_level_rank(minimum_level_);
}

Status Logger::add_sink(std::shared_ptr<ILogSink> sink)
{
    if (!sink)
    {
        return Status::error(EErrorCode::InvalidArgument, "Logger sink cannot be null.");
    }

    std::lock_guard lock{mutex_};
    sinks_.push_back(std::move(sink));
    return Status::success();
}

void Logger::clear_sinks()
{
    std::lock_guard lock{mutex_};
    sinks_.clear();
}

std::size_t Logger::sink_count() const
{
    std::lock_guard lock{mutex_};
    return sinks_.size();
}

Status Logger::log(LogRecord record)
{
    std::lock_guard lock{mutex_};

    if (log_level_rank(record.level()) < log_level_rank(minimum_level_))
    {
        return Status::success();
    }

    Status firstFailure = Status::success();

    for (const std::shared_ptr<ILogSink>& sink : sinks_)
    {
        const Status status = sink->write(record);
        if (!status.ok() && firstFailure.ok())
        {
            firstFailure = status;
        }
    }

    return firstFailure;
}

Status Logger::log(ELogLevel level, std::string category, std::string message, LogSourceLocation sourceLocation)
{
    return log(LogRecord{level, std::move(category), std::move(message), steady_now(), std::move(sourceLocation)});
}

void initialize_logging(std::shared_ptr<Logger> logger)
{
    std::lock_guard lock{gLoggingMutex};
    gLogger = std::move(logger);
}

void reset_logging()
{
    std::lock_guard lock{gLoggingMutex};
    gLogger.reset();
}

bool is_logging_initialized()
{
    std::lock_guard lock{gLoggingMutex};
    return static_cast<bool>(gLogger);
}

std::shared_ptr<Logger> current_logger()
{
    std::lock_guard lock{gLoggingMutex};
    return gLogger;
}

Status log_message(ELogLevel level, std::string category, std::string message, LogSourceLocation sourceLocation)
{
    const std::shared_ptr<Logger> logger = current_logger();
    if (!logger)
    {
        return Status::success();
    }

    return logger->log(level, std::move(category), std::move(message), std::move(sourceLocation));
}

ScopedLoggingOverride::ScopedLoggingOverride(std::shared_ptr<Logger> logger) : previous_logger_(current_logger())
{
    initialize_logging(std::move(logger));
}

ScopedLoggingOverride::~ScopedLoggingOverride()
{
    initialize_logging(std::move(previous_logger_));
}

void handle_assertion_failure(std::string_view expression, std::string_view message, LogSourceLocation sourceLocation)
{
    const std::string recordMessage = assertion_message(expression, message);
    (void)log_message(ELogLevel::Critical, "assert", recordMessage, std::move(sourceLocation));

#if defined(_MSC_VER)
    __debugbreak();
#elif defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#endif

    std::abort();
}

} // namespace core
