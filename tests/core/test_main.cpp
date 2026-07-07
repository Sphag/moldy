#include <iostream>
#include <string_view>

import moldy.core;

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

    if (context.failures() != 0)
    {
        std::cerr << context.failures() << " test assertion(s) failed.\n";
        return 1;
    }

    std::cout << "All core tests passed.\n";
    return 0;
}
