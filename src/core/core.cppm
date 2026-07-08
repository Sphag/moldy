module;

#include <chrono>
#include <concepts>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>

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

        Result(SuccessTag, TValue value) : value_(std::move(value)) {}

        Result(FailureTag, Status error) : error_(std::move(error)) {}

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

    class LogRecord
    {
    public:
        LogRecord(ELogLevel level, std::string category, std::string message);

        [[nodiscard]] ELogLevel level() const noexcept;
        [[nodiscard]] std::string_view category() const noexcept;
        [[nodiscard]] std::string_view message() const noexcept;

    private:
        ELogLevel level_{ELogLevel::Info};
        std::string category_;
        std::string message_;
    };

    [[nodiscard]] std::string_view log_level_name(ELogLevel level) noexcept;

    using SteadyTimePoint = std::chrono::steady_clock::time_point;
    using Duration = std::chrono::nanoseconds;

    [[nodiscard]] SteadyTimePoint steady_now() noexcept;
    [[nodiscard]] Duration elapsed_since(SteadyTimePoint start) noexcept;

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
