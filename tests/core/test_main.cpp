#include "core/build_info.hpp"

#include <iostream>
#include <string_view>

namespace {

class TestContext {
public:
    void expect(bool condition, std::string_view message)
    {
        if (!condition) {
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

} // namespace

int main()
{
    TestContext context;

    test_build_configuration_is_available(context);
    test_compiler_id_is_available(context);
    test_language_standard_is_modern_cpp(context);

    if (context.failures() != 0) {
        std::cerr << context.failures() << " test assertion(s) failed.\n";
        return 1;
    }

    std::cout << "All core tests passed.\n";
    return 0;
}
