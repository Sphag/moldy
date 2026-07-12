module;

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <span>
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

std::shared_ptr<Logger> gLogger;
thread_local bool gHandlingAssertionFailure = false;

int log_level_rank(ELogLevel level) noexcept
{
    return static_cast<int>(level);
}

std::string format_variadic_message(const char* format, std::va_list arguments)
{
    if (format == nullptr)
    {
        return "<null format>";
    }

    std::va_list sizeArguments;
    va_copy(sizeArguments, arguments);
    const int requiredSize = std::vsnprintf(nullptr, 0, format, sizeArguments);
    va_end(sizeArguments);

    if (requiredSize < 0)
    {
        return "<formatting failed>";
    }

    std::vector<char> buffer(static_cast<std::size_t>(requiredSize) + 1);
    (void)std::vsnprintf(buffer.data(), buffer.size(), format, arguments);
    return std::string{buffer.data(), static_cast<std::size_t>(requiredSize)};
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

std::string LogRecord::formatted_message() const
{
    std::ostringstream stream;
    stream << '[' << log_level_name(level_) << ']';

    if (!category_.empty())
    {
        stream << '[' << category_ << ']';
    }

    stream << ' ' << message_;

    if (!source_location_.empty())
    {
        stream << " (" << source_location_.file() << ':' << source_location_.line();
        if (!source_location_.function().empty())
        {
            stream << ' ' << source_location_.function();
        }
        stream << ')';
    }

    return stream.str();
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

std::string_view ConsoleLogSink::name() const noexcept
{
    return "console";
}

Status ConsoleLogSink::write(const LogRecord& record)
{
    std::lock_guard lock{mutex_};

    std::ostream& output = stream_ == EConsoleStream::StandardOutput ? std::cout : std::cerr;
    output << record.formatted_message() << '\n';

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

std::string_view FileLogSink::name() const noexcept
{
    return "file";
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

    stream_ << record.formatted_message() << '\n';
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

std::string_view InMemoryLogSink::name() const noexcept
{
    return "memory-unbounded";
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

BoundedInMemoryLogSink::BoundedInMemoryLogSink(std::size_t capacity) : capacity_(capacity)
{
    records_.reserve(capacity_);
}

std::string_view BoundedInMemoryLogSink::name() const noexcept
{
    return "memory-bounded";
}

Status BoundedInMemoryLogSink::write(const LogRecord& record)
{
    std::lock_guard lock{mutex_};
    if (records_.size() == capacity_)
    {
        return Status::error(EErrorCode::Internal, "Bounded memory log sink is full.");
    }

    records_.push_back(record);
    return Status::success();
}

std::vector<LogRecord> BoundedInMemoryLogSink::records() const
{
    std::lock_guard lock{mutex_};
    return records_;
}

std::size_t BoundedInMemoryLogSink::size() const
{
    std::lock_guard lock{mutex_};
    return records_.size();
}

std::size_t BoundedInMemoryLogSink::capacity() const noexcept
{
    return capacity_;
}

void BoundedInMemoryLogSink::clear()
{
    std::lock_guard lock{mutex_};
    records_.clear();
}

RingBufferLogSink::RingBufferLogSink(std::size_t capacity) : capacity_(capacity)
{
    records_.reserve(capacity_);
}

std::string_view RingBufferLogSink::name() const noexcept
{
    return "memory-ring";
}

Status RingBufferLogSink::write(const LogRecord& record)
{
    std::lock_guard lock{mutex_};
    if (capacity_ == 0)
    {
        return Status::error(EErrorCode::Internal, "Ring memory log sink has zero capacity.");
    }

    if (records_.size() < capacity_)
    {
        records_.push_back(record);
        return Status::success();
    }

    records_[next_record_] = record;
    next_record_ = (next_record_ + 1) % capacity_;
    return Status::success();
}

std::vector<LogRecord> RingBufferLogSink::records() const
{
    std::lock_guard lock{mutex_};
    if (records_.size() < capacity_ || next_record_ == 0)
    {
        return records_;
    }

    std::vector<LogRecord> ordered;
    ordered.reserve(records_.size());
    ordered.insert(ordered.end(), records_.begin() + static_cast<std::ptrdiff_t>(next_record_), records_.end());
    ordered.insert(ordered.end(), records_.begin(), records_.begin() + static_cast<std::ptrdiff_t>(next_record_));
    return ordered;
}

std::size_t RingBufferLogSink::size() const
{
    std::lock_guard lock{mutex_};
    return records_.size();
}

std::size_t RingBufferLogSink::capacity() const noexcept
{
    return capacity_;
}

void RingBufferLogSink::clear()
{
    std::lock_guard lock{mutex_};
    records_.clear();
    next_record_ = 0;
}

ExternalBufferLogSink::ExternalBufferLogSink(std::span<LogRecord> buffer) : buffer_(buffer) {}

std::string_view ExternalBufferLogSink::name() const noexcept
{
    return "memory-external";
}

Status ExternalBufferLogSink::write(const LogRecord& record)
{
    std::lock_guard lock{mutex_};
    if (size_ == buffer_.size())
    {
        return Status::error(EErrorCode::Internal, "External memory log sink is full.");
    }

    buffer_[size_] = record;
    ++size_;
    return Status::success();
}

std::vector<LogRecord> ExternalBufferLogSink::records() const
{
    std::lock_guard lock{mutex_};
    return {buffer_.begin(), buffer_.begin() + static_cast<std::ptrdiff_t>(size_)};
}

std::size_t ExternalBufferLogSink::size() const
{
    std::lock_guard lock{mutex_};
    return size_;
}

std::size_t ExternalBufferLogSink::capacity() const noexcept
{
    return buffer_.size();
}

void ExternalBufferLogSink::clear()
{
    std::lock_guard lock{mutex_};
    size_ = 0;
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
    std::vector<std::shared_ptr<ILogSink>> sinks;

    {
        std::lock_guard lock{mutex_};

        if (log_level_rank(record.level()) < log_level_rank(minimum_level_))
        {
            return Status::success();
        }

        sinks = sinks_;
    }

    if (sinks.empty())
    {
        return Status::success();
    }

    Status firstFailure = Status::success();
    std::ostringstream failureMessage;
    std::size_t failureCount = 0;

    for (const std::shared_ptr<ILogSink>& sink : sinks)
    {
        const Status status = sink->write(record);
        if (!status.ok())
        {
            if (firstFailure.ok())
            {
                firstFailure = status;
            }

            if (failureCount != 0)
            {
                failureMessage << "; ";
            }
            failureMessage << sink->name() << " (code " << static_cast<int>(status.code()) << "): " << status.message();
            ++failureCount;
        }
    }

#if defined(MOLDY_ENABLE_ASSERTS)
    if (failureCount != 0 && !gHandlingAssertionFailure)
    {
        handle_assertion_failure("all log sinks accepted the record", failureMessage.str(), record.source_location());
    }
#endif

    return firstFailure;
}

Status Logger::log(ELogLevel level, std::string category, std::string message, LogSourceLocation sourceLocation)
{
    return log(LogRecord{level, std::move(category), std::move(message), steady_now(), std::move(sourceLocation)});
}

void initialize_logging(std::shared_ptr<Logger> logger)
{
    gLogger = std::move(logger);
}

void reset_logging()
{
    gLogger.reset();
}

bool is_logging_initialized()
{
    return gLogger != nullptr;
}

std::shared_ptr<Logger> current_logger()
{
    return gLogger;
}

Status log_message(ELogLevel level, std::string category, std::string message, LogSourceLocation sourceLocation)
{
    const std::shared_ptr<Logger> logger = current_logger();
    if (logger == nullptr)
    {
        const Status status = Status::error(EErrorCode::Internal, "Logging is not initialized.");
#if defined(MOLDY_ENABLE_ASSERTS)
        if (!gHandlingAssertionFailure)
        {
            handle_assertion_failure("logger != nullptr", status.message(), std::move(sourceLocation));
        }
#endif
        return status;
    }

    return logger->log(level, std::move(category), std::move(message), std::move(sourceLocation));
}

Status log_formatted_message(ELogLevel level, std::string category, LogSourceLocation sourceLocation,
                             const char* format, ...)
{
    std::va_list arguments;
    va_start(arguments, format);
    std::string message = format_variadic_message(format, arguments);
    va_end(arguments);
    return log_message(level, std::move(category), std::move(message), std::move(sourceLocation));
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
    gHandlingAssertionFailure = true;
    const std::string recordMessage = assertion_message(expression, message);
    (void)log_message(ELogLevel::Critical, "assert", recordMessage, std::move(sourceLocation));

#if defined(_MSC_VER)
    __debugbreak();
#elif defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#endif

    std::abort();
}

void handle_formatted_assertion_failure(std::string_view expression, LogSourceLocation sourceLocation,
                                        const char* format, ...)
{
    std::va_list arguments;
    va_start(arguments, format);
    std::string message = format_variadic_message(format, arguments);
    va_end(arguments);
    handle_assertion_failure(expression, message, std::move(sourceLocation));
}

} // namespace core
