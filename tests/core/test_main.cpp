#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <memory>
#include <span>
#include <string>
#include <string_view>
#include <vector>

import moldy.core;

#include <moldy/core_macros.hpp>

namespace
{

class TestContext
{
public:
    void expect(bool condition, std::string_view message)
    {
        if (!condition)
        {
            ++failures_;
            std::cerr << "FAIL: " << message << '\n';
        }
    }

    [[nodiscard]] int failures() const noexcept
    {
        return failures_;
    }

private:
    int failures_{0};
};

#if !defined(MOLDY_ENABLE_ASSERTS)
class FailingSink final : public core::ILogSink
{
public:
    [[nodiscard]] std::string_view name() const noexcept override
    {
        return "failing-test";
    }

    [[nodiscard]] core::Status write(const core::LogRecord&) override
    {
        ++attempts_;
        return core::Status::error(core::EErrorCode::Internal, "intentional sink failure");
    }

    [[nodiscard]] int attempts() const noexcept
    {
        return attempts_;
    }

private:
    int attempts_{0};
};
#endif

class ReentrantSink final : public core::ILogSink
{
public:
    explicit ReentrantSink(core::Logger& logger) : logger_(logger) {}

    [[nodiscard]] std::string_view name() const noexcept override
    {
        return "reentrant-test";
    }

    [[nodiscard]] core::Status write(const core::LogRecord&) override
    {
        ++writes_;
        if (writes_ == 1)
        {
            return logger_.log(core::ELogLevel::Info, "reentrant", "nested");
        }

        return core::Status::success();
    }

    [[nodiscard]] int writes() const noexcept
    {
        return writes_;
    }

private:
    core::Logger& logger_;
    int writes_{0};
};

int passing_assert_value()
{
    return core::language_standard() >= 202002L ? 3 : 4;
}

void test_build_configuration_is_available(TestContext& context)
{
    context.expect(!core::build_configuration().empty(), "build configuration should not be empty");
}

void test_compiler_id_is_available(TestContext& context)
{
    context.expect(!core::compiler_id().empty(), "compiler id should not be empty");
}

void test_language_standard_is_modern_cpp(TestContext& context)
{
    context.expect(core::language_standard() >= 202002L, "language standard should be at least C++20");
}

void test_success_status_reports_ok(TestContext& context)
{
    const core::Status status = core::Status::success();

    context.expect(status.ok(), "success status should report ok");
    context.expect(status.code() == core::EErrorCode::Ok, "success status should use Ok code");
    context.expect(status.message().empty(), "success status should have an empty message");
}

void test_error_status_preserves_code_and_message(TestContext& context)
{
    const core::Status status = core::Status::error(core::EErrorCode::NotFound, "asset missing");

    context.expect(!status.ok(), "error status should not report ok");
    context.expect(status.code() == core::EErrorCode::NotFound, "error status should preserve code");
    context.expect(status.message() == "asset missing", "error status should preserve message");
}

void test_success_result_exposes_value(TestContext& context)
{
    const core::Result<int> result = core::Result<int>::success(42);

    context.expect(result.ok(), "success result should report ok");
    context.expect(result.value_if() != nullptr, "success result should expose a value");
    context.expect(result.value_if() != nullptr && *result.value_if() == 42, "success result should preserve value");
    context.expect(result.error_if() == nullptr, "success result should not expose an error");
}

void test_failure_result_exposes_error(TestContext& context)
{
    const core::Result<int> result =
        core::Result<int>::failure(core::Status::error(core::EErrorCode::InvalidArgument, "bad argument"));

    context.expect(!result.ok(), "failure result should not report ok");
    context.expect(result.value_if() == nullptr, "failure result should not expose a value");
    context.expect(result.error_if() != nullptr, "failure result should expose an error");
    context.expect(result.error_if() != nullptr && result.error_if()->code() == core::EErrorCode::InvalidArgument,
                   "failure result should preserve error code");
    context.expect(result.error_if() != nullptr && result.error_if()->message() == "bad argument",
                   "failure result should preserve error message");
}

void test_failure_result_normalizes_ok_status(TestContext& context)
{
    const core::Result<int> result = core::Result<int>::failure(core::Status::success());

    context.expect(!result.ok(), "failure result built from success status should remain a failure");
    context.expect(result.value_if() == nullptr, "normalized failure should not expose a value");
    context.expect(result.error_if() != nullptr, "normalized failure should expose an error");
    context.expect(result.error_if() != nullptr && result.error_if()->code() == core::EErrorCode::Internal,
                   "normalized failure should use internal error code");
}

void test_status_result_can_store_status_payload(TestContext& context)
{
    const core::Result<core::Status> result = core::Result<core::Status>::success(core::Status::success());

    context.expect(result.ok(), "status payload result should report ok");
    context.expect(result.value_if() != nullptr, "status payload result should expose a value");
    context.expect(result.value_if() != nullptr && result.value_if()->ok(),
                   "status payload result should preserve status");
    context.expect(result.error_if() == nullptr, "status payload result should not expose an error");
}

void test_log_level_names_are_stable(TestContext& context)
{
    context.expect(core::log_level_name(core::ELogLevel::Trace) == "trace", "trace log level name should be stable");
    context.expect(core::log_level_name(core::ELogLevel::Debug) == "debug", "debug log level name should be stable");
    context.expect(core::log_level_name(core::ELogLevel::Info) == "info", "info log level name should be stable");
    context.expect(core::log_level_name(core::ELogLevel::Warning) == "warning",
                   "warning log level name should be stable");
    context.expect(core::log_level_name(core::ELogLevel::Error) == "error", "error log level name should be stable");
    context.expect(core::log_level_name(core::ELogLevel::Critical) == "critical",
                   "critical log level name should be stable");
}

void test_log_record_preserves_fields(TestContext& context)
{
    const core::SteadyTimePoint timestamp = core::steady_now();
    const core::LogSourceLocation sourceLocation{"test.cpp", 42, "test_function"};
    const core::LogRecord record{core::ELogLevel::Warning, "runtime", "shutdown requested", timestamp, sourceLocation};

    context.expect(record.level() == core::ELogLevel::Warning, "log record should preserve level");
    context.expect(record.category() == "runtime", "log record should preserve category");
    context.expect(record.message() == "shutdown requested", "log record should preserve message");
    context.expect(record.timestamp() == timestamp, "log record should preserve steady timestamp");
    context.expect(record.source_location().file() == "test.cpp", "log record should preserve source file");
    context.expect(record.source_location().line() == 42, "log record should preserve source line");
    context.expect(record.source_location().function() == "test_function",
                   "log record should preserve source function");
}

void test_log_record_allows_empty_fields(TestContext& context)
{
    const core::LogRecord record{core::ELogLevel::Info, "", ""};

    context.expect(record.level() == core::ELogLevel::Info, "empty log record should preserve level");
    context.expect(record.category().empty(), "log record should allow an empty category");
    context.expect(record.message().empty(), "log record should allow an empty message");
    context.expect(record.timestamp().time_since_epoch() >= core::Duration::zero(),
                   "default log record should capture a steady timestamp");
    context.expect(record.source_location().empty(), "default log record should have an empty source location");
}

void test_log_record_formats_for_text_sinks(TestContext& context)
{
    const core::LogRecord record{core::ELogLevel::Warning, "runtime", "shutdown", core::SteadyTimePoint{},
                                 core::LogSourceLocation{"test.cpp", 42, "test"}};
    const std::string formatted = record.formatted_message();

    context.expect(formatted.find("[warning][runtime] shutdown") != std::string::npos,
                   "formatted log record should include level, category, and message");
    context.expect(formatted.find("test.cpp:42 test") != std::string::npos,
                   "formatted log record should include source location");
}

void test_memory_sink_variants(TestContext& context)
{
    const core::LogRecord first{core::ELogLevel::Info, "memory", "first"};
    const core::LogRecord second{core::ELogLevel::Info, "memory", "second"};
    const core::LogRecord third{core::ELogLevel::Info, "memory", "third"};

    core::BoundedInMemoryLogSink bounded{2};
    context.expect(bounded.write(first).ok(), "bounded sink should accept records within capacity");
    context.expect(bounded.write(second).ok(), "bounded sink should fill its capacity");
    context.expect(!bounded.write(third).ok(), "bounded sink should reject records after reaching capacity");
    context.expect(bounded.records().front().message() == "first", "bounded sink should preserve earliest records");

    core::RingBufferLogSink ring{2};
    context.expect(ring.write(first).ok() && ring.write(second).ok() && ring.write(third).ok(),
                   "ring sink should accept records while overwriting old entries");
    const std::vector<core::LogRecord> ringRecords = ring.records();
    context.expect(ringRecords.size() == 2 && ringRecords[0].message() == "second" &&
                       ringRecords[1].message() == "third",
                   "ring sink should expose retained records in chronological order");

    std::array<core::LogRecord, 2> storage{first, first};
    core::ExternalBufferLogSink external{storage};
    context.expect(external.write(second).ok() && external.write(third).ok(),
                   "external sink should write into caller-owned storage");
    context.expect(!external.write(first).ok(), "external sink should reject records after filling the buffer");
    const std::vector<core::LogRecord> externalRecords = external.records();
    context.expect(externalRecords.size() == 2 && externalRecords[0].message() == "second" &&
                       externalRecords[1].message() == "third",
                   "external sink should expose the written portion of caller-owned storage");
    external.clear();
    context.expect(external.write(first).ok(), "external sink should accept records after clear");
    context.expect(externalRecords.size() == 2 && externalRecords[0].message() == "second",
                   "external sink record snapshots should remain stable after later writes");

    context.expect(bounded.name() == "memory-bounded" && ring.name() == "memory-ring" &&
                       external.name() == "memory-external",
                   "memory sinks should expose stable diagnostic names");
}

void test_logger_records_to_memory_sink(TestContext& context)
{
    core::Logger logger;
    const std::shared_ptr<core::InMemoryLogSink> memorySink = std::make_shared<core::InMemoryLogSink>();

    const core::Status addStatus = logger.add_sink(memorySink);
    const core::Status logStatus = logger.log(core::ELogLevel::Info, "runtime", "started");
    const std::vector<core::LogRecord> records = memorySink->records();

    context.expect(addStatus.ok(), "logger should accept a valid sink");
    context.expect(logStatus.ok(), "logger should report successful dispatch");
    context.expect(logger.sink_count() == 1, "logger should report configured sink count");
    context.expect(records.size() == 1, "in-memory sink should receive a record");
    context.expect(!records.empty() && records.front().message() == "started",
                   "in-memory sink should preserve dispatched message");
}

void test_logger_filters_by_minimum_level(TestContext& context)
{
    core::Logger logger{core::ELogLevel::Warning};
    const std::shared_ptr<core::InMemoryLogSink> memorySink = std::make_shared<core::InMemoryLogSink>();

    (void)logger.add_sink(memorySink);
    const core::Status infoStatus = logger.log(core::ELogLevel::Info, "runtime", "filtered");
    const core::Status errorStatus = logger.log(core::ELogLevel::Error, "runtime", "recorded");
    const std::vector<core::LogRecord> records = memorySink->records();

    context.expect(infoStatus.ok(), "filtered log call should still report success");
    context.expect(errorStatus.ok(), "enabled log call should report success");
    context.expect(!logger.should_log(core::ELogLevel::Info), "logger should reject levels below minimum");
    context.expect(logger.should_log(core::ELogLevel::Error), "logger should accept levels at or above minimum");
    context.expect(records.size() == 1, "only enabled records should reach sinks");
    context.expect(!records.empty() && records.front().message() == "recorded",
                   "enabled record should preserve message");
}

#if !defined(MOLDY_ENABLE_ASSERTS)
void test_logger_fans_out_and_preserves_sink_failures(TestContext& context)
{
    core::Logger logger;
    const std::shared_ptr<FailingSink> failingSink = std::make_shared<FailingSink>();
    const std::shared_ptr<core::InMemoryLogSink> memorySink = std::make_shared<core::InMemoryLogSink>();

    (void)logger.add_sink(failingSink);
    (void)logger.add_sink(memorySink);
    const core::Status logStatus = logger.log(core::ELogLevel::Critical, "runtime", "fanout");

    context.expect(!logStatus.ok(), "logger should surface the first sink failure");
    context.expect(failingSink->attempts() == 1, "failing custom sink should be called");
    context.expect(memorySink->size() == 1, "healthy sink should still receive records after another sink fails");
}
#endif

void test_logger_allows_reentrant_sink_logging(TestContext& context)
{
    core::Logger logger;
    const std::shared_ptr<ReentrantSink> reentrantSink = std::make_shared<ReentrantSink>(logger);
    const std::shared_ptr<core::InMemoryLogSink> memorySink = std::make_shared<core::InMemoryLogSink>();

    (void)logger.add_sink(reentrantSink);
    (void)logger.add_sink(memorySink);

    const core::Status logStatus = logger.log(core::ELogLevel::Info, "reentrant", "outer");
    const std::vector<core::LogRecord> records = memorySink->records();

    context.expect(logStatus.ok(), "reentrant sink logging should complete without deadlock");
    context.expect(reentrantSink->writes() == 2, "reentrant sink should receive outer and nested records");
    context.expect(records.size() == 2, "healthy sink should receive outer and nested records");
    context.expect(records.size() == 2 && records[0].message() == "nested",
                   "nested reentrant record should be dispatched");
    context.expect(records.size() == 2 && records[1].message() == "outer", "outer record should continue dispatching");
}

void test_global_logging_reports_missing_initialization(TestContext& context)
{
    core::reset_logging();

#if defined(MOLDY_ENABLE_ASSERTS)
    context.expect(!core::is_logging_initialized(), "logging should be uninitialized after reset");
#else
    const core::Status directStatus = core::log_message(core::ELogLevel::Info, "runtime", "dropped");
    const core::Status macroStatus = MOLDY_LOG_INFO("runtime", "also %s", "dropped");

    context.expect(!core::is_logging_initialized(), "logging should be uninitialized after reset");
    context.expect(!directStatus.ok(), "direct global logging before initialization should report an error");
    context.expect(!macroStatus.ok(), "macro logging before initialization should report an error");
#endif
}

void test_global_logging_macros_reach_initialized_logger(TestContext& context)
{
    core::reset_logging();

    const std::shared_ptr<core::Logger> logger = std::make_shared<core::Logger>();
    const std::shared_ptr<core::InMemoryLogSink> memorySink = std::make_shared<core::InMemoryLogSink>();
    (void)logger->add_sink(memorySink);

    {
        core::ScopedLoggingOverride override{logger};
        const core::Status status = MOLDY_LOG_WARNING("macro", "%s %d", "captured", 7);
        const std::vector<core::LogRecord> records = memorySink->records();

        context.expect(status.ok(), "macro logging should report successful dispatch");
        context.expect(records.size() == 1, "initialized macro logging should reach configured sinks");
        context.expect(!records.empty() && records.front().category() == "macro",
                       "macro logging should preserve category");
        context.expect(!records.empty() && records.front().message() == "captured 7",
                       "macro logging should format message arguments");
        context.expect(!records.empty() && !records.front().source_location().empty(),
                       "macro logging should capture source location");
    }

    core::reset_logging();
}

void test_scoped_logging_override_restores_previous_logger(TestContext& context)
{
    core::reset_logging();

    const std::shared_ptr<core::Logger> firstLogger = std::make_shared<core::Logger>();
    const std::shared_ptr<core::Logger> secondLogger = std::make_shared<core::Logger>();
    const std::shared_ptr<core::InMemoryLogSink> firstSink = std::make_shared<core::InMemoryLogSink>();
    const std::shared_ptr<core::InMemoryLogSink> secondSink = std::make_shared<core::InMemoryLogSink>();
    (void)firstLogger->add_sink(firstSink);
    (void)secondLogger->add_sink(secondSink);

    core::initialize_logging(firstLogger);

    {
        core::ScopedLoggingOverride override{secondLogger};
        (void)core::log_message(core::ELogLevel::Info, "runtime", "inside override");
    }

    (void)core::log_message(core::ELogLevel::Info, "runtime", "after override");

    context.expect(firstSink->size() == 1, "scoped override should restore previous logger");
    context.expect(secondSink->size() == 1, "scoped override should route logs inside the scope");

    core::reset_logging();
}

void test_file_log_sink_writes_records(TestContext& context)
{
    const std::string_view path = "core_file_sink_test.log";
    core::FileLogSink fileSink{std::string{path}, false};

    const core::Status openStatus = fileSink.open_status();
    const core::Status writeStatus = fileSink.write(core::LogRecord{core::ELogLevel::Error, "file", "persisted"});

    std::ifstream input{std::string{path}};
    std::string content;
    std::getline(input, content);

    context.expect(openStatus.ok(), "file sink should open a writable test log path");
    context.expect(writeStatus.ok(), "file sink should write a log record");
    context.expect(content.find("[error][file] persisted") != std::string::npos,
                   "file sink should write level, category, and message");
}

void test_assert_macros_allow_passing_assertions(TestContext& context)
{
    const int value = passing_assert_value();

    MOLDY_ASSERT(value == 3);
    MOLDY_ASSERT_MSG(value != 4, "value should remain %s", "unchanged");

    context.expect(value == 3, "passing assert macros should not change state");

#if !defined(MOLDY_ENABLE_ASSERTS)
    MOLDY_ASSERT_FAIL("compiled out in non-assert builds");
    context.expect(true, "assert fail macro should compile out when assertions are disabled");
#endif
}

void test_steady_time_helpers(TestContext& context)
{
    const core::SteadyTimePoint now = core::steady_now();
    const core::Duration elapsed = core::elapsed_since(now);

    context.expect(now.time_since_epoch() >= core::Duration::zero(), "steady_now should return a valid time point");
    context.expect(elapsed >= core::Duration::zero(), "elapsed_since should be non-negative for a captured start");

    const core::SteadyTimePoint older = now - std::chrono::milliseconds(5);
    const core::Duration syntheticElapsed = core::elapsed_since(older);

    context.expect(syntheticElapsed >= std::chrono::milliseconds(5),
                   "elapsed_since should report positive duration for an older synthetic start");
}

void test_lifecycle_initial_state_is_created(TestContext& context)
{
    const core::ApplicationLifecycle lifecycle;

    context.expect(lifecycle.state() == core::EApplicationState::Created,
                   "application lifecycle should start in Created state");
}

void test_lifecycle_valid_stop_requested_path(TestContext& context)
{
    core::ApplicationLifecycle lifecycle;

    const core::Status startStatus = lifecycle.start();
    context.expect(startStatus.ok(), "start from Created should succeed");
    context.expect(lifecycle.state() == core::EApplicationState::Running, "start should transition to Running");

    const core::Status requestStopStatus = lifecycle.request_stop();
    context.expect(requestStopStatus.ok(), "request_stop from Running should succeed");
    context.expect(lifecycle.state() == core::EApplicationState::StopRequested,
                   "request_stop should transition to StopRequested");

    const core::Status stopStatus = lifecycle.stop();
    context.expect(stopStatus.ok(), "stop from StopRequested should succeed");
    context.expect(lifecycle.state() == core::EApplicationState::Stopped, "stop should transition to Stopped");
}

void test_lifecycle_stop_from_running_succeeds(TestContext& context)
{
    core::ApplicationLifecycle lifecycle;

    const core::Status startStatus = lifecycle.start();
    const core::Status stopStatus = lifecycle.stop();

    context.expect(startStatus.ok(), "start from Created should succeed before direct stop");
    context.expect(stopStatus.ok(), "stop from Running should succeed");
    context.expect(lifecycle.state() == core::EApplicationState::Stopped, "direct stop should transition to Stopped");
}

void test_lifecycle_invalid_transitions_preserve_state(TestContext& context)
{
    core::ApplicationLifecycle lifecycle;

    const core::Status requestBeforeStart = lifecycle.request_stop();
    context.expect(!requestBeforeStart.ok(), "request_stop from Created should fail");
    context.expect(requestBeforeStart.code() == core::EErrorCode::InvalidArgument,
                   "request_stop from Created should return InvalidArgument");
    context.expect(lifecycle.state() == core::EApplicationState::Created,
                   "request_stop from Created should preserve state");

    const core::Status stopBeforeStart = lifecycle.stop();
    context.expect(!stopBeforeStart.ok(), "stop from Created should fail");
    context.expect(stopBeforeStart.code() == core::EErrorCode::InvalidArgument,
                   "stop from Created should return InvalidArgument");
    context.expect(lifecycle.state() == core::EApplicationState::Created, "stop from Created should preserve state");

    const core::Status startStatus = lifecycle.start();
    const core::Status startAgain = lifecycle.start();
    context.expect(startStatus.ok(), "first start should succeed");
    context.expect(!startAgain.ok(), "start from Running should fail");
    context.expect(startAgain.code() == core::EErrorCode::InvalidArgument,
                   "start from Running should return InvalidArgument");
    context.expect(lifecycle.state() == core::EApplicationState::Running, "start from Running should preserve state");

    const core::Status requestStopStatus = lifecycle.request_stop();
    const core::Status requestStopAgain = lifecycle.request_stop();
    context.expect(requestStopStatus.ok(), "first request_stop should succeed");
    context.expect(!requestStopAgain.ok(), "request_stop from StopRequested should fail");
    context.expect(requestStopAgain.code() == core::EErrorCode::InvalidArgument,
                   "request_stop from StopRequested should return InvalidArgument");
    context.expect(lifecycle.state() == core::EApplicationState::StopRequested,
                   "request_stop from StopRequested should preserve state");

    const core::Status stopStatus = lifecycle.stop();
    const core::Status stopAgain = lifecycle.stop();
    context.expect(stopStatus.ok(), "first stop should succeed");
    context.expect(!stopAgain.ok(), "stop from Stopped should fail");
    context.expect(stopAgain.code() == core::EErrorCode::InvalidArgument,
                   "stop from Stopped should return InvalidArgument");
    context.expect(lifecycle.state() == core::EApplicationState::Stopped, "stop from Stopped should preserve state");
}

} // namespace

int main()
{
    TestContext context;

    test_build_configuration_is_available(context);
    test_compiler_id_is_available(context);
    test_language_standard_is_modern_cpp(context);
    test_success_status_reports_ok(context);
    test_error_status_preserves_code_and_message(context);
    test_success_result_exposes_value(context);
    test_failure_result_exposes_error(context);
    test_failure_result_normalizes_ok_status(context);
    test_status_result_can_store_status_payload(context);
    test_log_level_names_are_stable(context);
    test_log_record_preserves_fields(context);
    test_log_record_allows_empty_fields(context);
    test_log_record_formats_for_text_sinks(context);
    test_memory_sink_variants(context);
    test_logger_records_to_memory_sink(context);
    test_logger_filters_by_minimum_level(context);
#if !defined(MOLDY_ENABLE_ASSERTS)
    test_logger_fans_out_and_preserves_sink_failures(context);
#endif
    test_logger_allows_reentrant_sink_logging(context);
    test_global_logging_reports_missing_initialization(context);
    test_global_logging_macros_reach_initialized_logger(context);
    test_scoped_logging_override_restores_previous_logger(context);
    test_file_log_sink_writes_records(context);
    test_assert_macros_allow_passing_assertions(context);
    test_steady_time_helpers(context);
    test_lifecycle_initial_state_is_created(context);
    test_lifecycle_valid_stop_requested_path(context);
    test_lifecycle_stop_from_running_succeeds(context);
    test_lifecycle_invalid_transitions_preserve_state(context);

    if (context.failures() != 0)
    {
        std::cerr << context.failures() << " test assertion(s) failed.\n";
        return 1;
    }

    std::cout << "All core tests passed.\n";
    return 0;
}
