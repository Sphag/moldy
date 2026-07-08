module;

#include <chrono>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

export module moldy.core;

export {
    namespace core
    {

    enum class EErrorCode : std::uint8_t
    {
        Ok,
        Unknown,
        InvalidArgument,
        NotFound,
        Unsupported,
        Internal
    };

    class Status
    {
    public:
        [[nodiscard]] bool ok() const noexcept;
        [[nodiscard]] EErrorCode code() const noexcept;
        [[nodiscard]] std::string_view message() const noexcept;

        [[nodiscard]] static Status success();
        [[nodiscard]] static Status error(EErrorCode code, std::string message);

    private:
        Status(EErrorCode code, std::string message);

        EErrorCode code_{EErrorCode::Ok};
        std::string message_;
    };

    template <typename TValue>
    concept ResultValue = !std::same_as<TValue, void> && std::movable<TValue>;

    template <ResultValue TValue> class Result
    {
    public:
        [[nodiscard]] static Result success(TValue value)
        {
            return Result(SuccessTag{}, std::move(value));
        }

        [[nodiscard]] static Result failure(Status status)
        {
            if (status.ok())
            {
                status = Status::error(EErrorCode::Internal, "Result failure requires an error status.");
            }

            return Result(FailureTag{}, std::move(status));
        }

        [[nodiscard]] bool ok() const noexcept
        {
            return value_.has_value();
        }

        [[nodiscard]] const TValue* value_if() const noexcept
        {
            return value_ ? std::addressof(*value_) : nullptr;
        }

        [[nodiscard]] TValue* value_if() noexcept
        {
            return value_ ? std::addressof(*value_) : nullptr;
        }

        [[nodiscard]] const Status* error_if() const noexcept
        {
            return error_ ? std::addressof(*error_) : nullptr;
        }

        [[nodiscard]] Status* error_if() noexcept
        {
            return error_ ? std::addressof(*error_) : nullptr;
        }

    private:
        struct SuccessTag
        {
        };

        struct FailureTag
        {
        };

        Result(SuccessTag, TValue value) : value_(std::move(value)), error_() {}

        Result(FailureTag, Status error) : value_(), error_(std::move(error)) {}

        std::optional<TValue> value_;
        std::optional<Status> error_;
    };

    enum class ELogLevel : std::uint8_t
    {
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Critical
    };

    using SteadyTimePoint = std::chrono::steady_clock::time_point;
    using Duration = std::chrono::nanoseconds;

    class LogSourceLocation
    {
    public:
        LogSourceLocation();
        LogSourceLocation(std::string file, int line, std::string function);

        [[nodiscard]] std::string_view file() const noexcept;
        [[nodiscard]] int line() const noexcept;
        [[nodiscard]] std::string_view function() const noexcept;
        [[nodiscard]] bool empty() const noexcept;

    private:
        std::string file_;
        int line_{0};
        std::string function_;
    };

    class LogRecord
    {
    public:
        LogRecord(ELogLevel level, std::string category, std::string message);
        LogRecord(ELogLevel level, std::string category, std::string message, SteadyTimePoint timestamp,
                  LogSourceLocation sourceLocation);

        [[nodiscard]] ELogLevel level() const noexcept;
        [[nodiscard]] std::string_view category() const noexcept;
        [[nodiscard]] std::string_view message() const noexcept;
        [[nodiscard]] SteadyTimePoint timestamp() const noexcept;
        [[nodiscard]] const LogSourceLocation& source_location() const noexcept;

    private:
        ELogLevel level_{ELogLevel::Info};
        std::string category_;
        std::string message_;
        SteadyTimePoint timestamp_;
        LogSourceLocation source_location_;
    };

    [[nodiscard]] std::string_view log_level_name(ELogLevel level) noexcept;

    class ILogSink
    {
    public:
        virtual ~ILogSink();

        [[nodiscard]] virtual Status write(const LogRecord& record) = 0;
    };

    enum class EConsoleStream : std::uint8_t
    {
        StandardOutput,
        StandardError
    };

    class ConsoleLogSink final : public ILogSink
    {
    public:
        explicit ConsoleLogSink(EConsoleStream stream = EConsoleStream::StandardError);

        [[nodiscard]] Status write(const LogRecord& record) override;

    private:
        EConsoleStream stream_{EConsoleStream::StandardError};
    };

    class FileLogSink final : public ILogSink
    {
    public:
        explicit FileLogSink(std::string path, bool append = true);

        [[nodiscard]] std::string_view path() const noexcept;
        [[nodiscard]] Status open_status() const;
        [[nodiscard]] Status write(const LogRecord& record) override;

    private:
        mutable std::mutex mutex_;
        std::string path_;
        std::ofstream stream_;
        Status open_status_;
    };

    class InMemoryLogSink final : public ILogSink
    {
    public:
        [[nodiscard]] Status write(const LogRecord& record) override;
        [[nodiscard]] std::vector<LogRecord> records() const;
        [[nodiscard]] std::size_t size() const;
        void clear();

    private:
        mutable std::mutex mutex_;
        std::vector<LogRecord> records_;
    };

    class Logger
    {
    public:
        explicit Logger(ELogLevel minimumLevel = ELogLevel::Trace);

        void set_minimum_level(ELogLevel minimumLevel);
        [[nodiscard]] ELogLevel minimum_level() const;
        [[nodiscard]] bool should_log(ELogLevel level) const;

        [[nodiscard]] Status add_sink(std::shared_ptr<ILogSink> sink);
        void clear_sinks();
        [[nodiscard]] std::size_t sink_count() const;

        [[nodiscard]] Status log(LogRecord record);
        [[nodiscard]] Status log(ELogLevel level, std::string category, std::string message,
                                 LogSourceLocation sourceLocation = {});

    private:
        mutable std::mutex mutex_;
        ELogLevel minimum_level_{ELogLevel::Trace};
        std::vector<std::shared_ptr<ILogSink>> sinks_;
    };

    void initialize_logging(std::shared_ptr<Logger> logger);
    void reset_logging();
    [[nodiscard]] bool is_logging_initialized();
    [[nodiscard]] std::shared_ptr<Logger> current_logger();
    [[nodiscard]] Status log_message(ELogLevel level, std::string category, std::string message,
                                     LogSourceLocation sourceLocation = {});

    class ScopedLoggingOverride final
    {
    public:
        explicit ScopedLoggingOverride(std::shared_ptr<Logger> logger);
        ~ScopedLoggingOverride();

        ScopedLoggingOverride(const ScopedLoggingOverride&) = delete;
        ScopedLoggingOverride& operator=(const ScopedLoggingOverride&) = delete;
        ScopedLoggingOverride(ScopedLoggingOverride&&) = delete;
        ScopedLoggingOverride& operator=(ScopedLoggingOverride&&) = delete;

    private:
        std::shared_ptr<Logger> previous_logger_;
    };

    [[nodiscard]] SteadyTimePoint steady_now() noexcept;
    [[nodiscard]] Duration elapsed_since(SteadyTimePoint start) noexcept;

    [[noreturn]] void handle_assertion_failure(std::string expression, std::string message,
                                               LogSourceLocation sourceLocation);

    enum class EApplicationState : std::uint8_t
    {
        Created,
        Running,
        StopRequested,
        Stopped
    };

    class ApplicationLifecycle
    {
    public:
        [[nodiscard]] EApplicationState state() const noexcept;

        [[nodiscard]] Status start();
        [[nodiscard]] Status request_stop();
        [[nodiscard]] Status stop();

    private:
        EApplicationState state_{EApplicationState::Created};
    };

    std::string_view build_configuration() noexcept;
    std::string_view compiler_id() noexcept;
    long language_standard() noexcept;

    } // namespace core
}
