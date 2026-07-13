#include <cmath>
#include <iostream>
#include <string_view>
#include <type_traits>

import moldy.math;

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

    void expect_near(float actual, float expected, float tolerance, std::string_view message)
    {
        expect(std::fabs(actual - expected) <= tolerance, message);
    }

    [[nodiscard]] int failures() const noexcept
    {
        return failures_;
    }

private:
    int failures_{0};
};

void test_float_vectors(TestContext& context)
{
    const math::float2 vector2{3.0F, 4.0F};
    context.expect(vector2 == math::float2{3.0F, 4.0F}, "float2 preserves exact structural equality.");
    context.expect(vector2.r == 3.0F && vector2.g == 4.0F && vector2.u == 3.0F && vector2.v == 4.0F,
                   "float2 exposes color and texture-coordinate aliases.");
    context.expect((vector2 + math::float2{1.0F, 2.0F}) == math::float2{4.0F, 6.0F},
                   "float2 supports vector addition.");
    context.expect((-vector2) == math::float2{-3.0F, -4.0F}, "float2 supports unary negation.");
    context.expect((2.0F * vector2) == math::float2{6.0F, 8.0F}, "float2 supports scalar multiplication.");
    context.expect(math::dot(vector2, math::float2{1.0F, 2.0F}) == 11.0F, "float2 dot product is exact.");
    context.expect(math::length_squared(vector2) == 25.0F, "float2 squared length is exact.");
    context.expect(math::length(vector2) == 5.0F, "float2 length is exact for a 3-4-5 vector.");
    context.expect(math::swizzle<1, 0>(vector2) == math::float2{4.0F, 3.0F},
                   "float2 free swizzle reorders components.");

    const auto normalized2 = math::normalize(vector2);
    context.expect(normalized2.has_value(), "Non-zero float2 normalizes.");
    context.expect(normalized2.has_value() && *normalized2 == math::float2{0.6F, 0.8F},
                   "float2 normalization has expected components.");
    context.expect(!math::normalize(math::float2{}).has_value(), "Zero float2 has no normalized value.");

    const math::float3 vector3{1.0F, 2.0F, 3.0F};
    context.expect(vector3.r == 1.0F && vector3.g == 2.0F && vector3.b == 3.0F,
                   "float3 exposes RGBA-compatible aliases.");
    context.expect(math::dot(vector3, math::float3{4.0F, 5.0F, 6.0F}) == 32.0F, "float3 dot product is exact.");
    context.expect(math::cross(math::float3{1.0F, 0.0F, 0.0F}, math::float3{0.0F, 1.0F, 0.0F}) ==
                       math::float3{0.0F, 0.0F, 1.0F},
                   "float3 cross product follows the right-hand rule.");
    context.expect(math::swizzle<2, 0, 1>(vector3) == math::float3{3.0F, 1.0F, 2.0F},
                   "float3 free swizzle returns the requested ordering.");

    const math::float4 vector4{1.0F, 2.0F, 3.0F, 4.0F};
    context.expect(vector4.r == 1.0F && vector4.g == 2.0F && vector4.b == 3.0F && vector4.a == 4.0F,
                   "float4 exposes RGBA aliases.");
    context.expect(math::swizzle<3, 2, 1, 0>(vector4) == math::float4{4.0F, 3.0F, 2.0F, 1.0F},
                   "float4 free swizzle supports four-component results.");
    context.expect(math::dot(vector4, vector4) == 30.0F, "float4 dot product is exact.");
}

void test_integer_vectors(TestContext& context)
{
    const math::int3 signed_vector{1, -2, 3};
    context.expect((signed_vector * 2) == math::int3{2, -4, 6}, "int3 supports component-wise scalar arithmetic.");
    context.expect(math::dot(signed_vector, math::int3{2, 3, 4}) == 8, "int3 dot product is exact.");

    const math::uint4 unsigned_vector{1U, 2U, 3U, 4U};
    context.expect((unsigned_vector + math::uint4{4U, 3U, 2U, 1U}) == math::uint4{5U, 5U, 5U, 5U},
                   "uint4 supports vector arithmetic.");
    context.expect(math::swizzle<2, 1>(unsigned_vector) == math::uint2{3U, 2U}, "uint vectors support free swizzle.");
    context.expect(math::one<math::int2> == math::int2{1, 1}, "Integer vector one constant has every component set.");
    context.expect(math::zero<math::uint3> == math::uint3{}, "Generic zero constant uses zero-initialized vectors.");
}

void test_matrices(TestContext& context)
{
    const math::float2x2 matrix{math::float2{2.0F, 0.0F}, math::float2{0.0F, 3.0F}};
    context.expect((matrix * math::float2{4.0F, 5.0F}) == math::float2{8.0F, 15.0F},
                   "Column-major float2x2 multiplies column vectors.");
    context.expect((math::identity<math::float2x2> * matrix) == matrix, "Float matrix identity is a left identity.");
    context.expect((matrix * math::identity<math::float2x2>) == matrix, "Float matrix identity is a right identity.");
    context.expect(math::zero<math::float3x3> == math::float3x3{}, "Float matrix zero constant is zero-initialized.");

    const math::int3x3 integer_identity = math::identity<math::int3x3>;
    context.expect((integer_identity * math::int3{4, -2, 1}) == math::int3{4, -2, 1},
                   "Integer matrix identity preserves vectors.");
    const math::uint4x4 unsigned_identity = math::identity<math::uint4x4>;
    context.expect((unsigned_identity * math::uint4{1U, 2U, 3U, 4U}) == math::uint4{1U, 2U, 3U, 4U},
                   "Unsigned matrix identity preserves vectors.");
}

void test_color(TestContext& context)
{
    context.expect(math::colors::transparent == math::color{0.0F, 0.0F, 0.0F, 0.0F},
                   "Transparent color constant has zero alpha.");
    context.expect(math::colors::red == math::color{1.0F, 0.0F, 0.0F, 1.0F},
                   "Named color constants are linear RGBA values.");

    const math::rgb linear_red{1.0F, 0.0F, 0.0F};
    const math::color opaque_red{linear_red};
    context.expect(opaque_red.linear == linear_red && opaque_red.a == 1.0F,
                   "color composes linear RGB with alpha.");

    const math::hsv hsv = math::rgb_to_hsv(math::rgb{1.0F, 0.0F, 0.0F});
    context.expect(hsv == math::hsv{0.0F, 1.0F, 1.0F}, "Linear RGB converts red to HSV.");
    const math::hsl hsl = math::rgb_to_hsl(math::rgb{1.0F, 0.0F, 0.0F});
    context.expect(hsl == math::hsl{0.0F, 1.0F, 0.5F}, "Linear RGB converts red to HSL.");

    const math::rgb original{0.25F, 0.5F, 0.75F};
    const math::rgb hsv_round_trip = math::hsv_to_rgb(math::rgb_to_hsv(original));
    const math::rgb hsl_round_trip = math::hsl_to_rgb(math::rgb_to_hsl(original));
    const math::rgb srgb_round_trip = math::rgb_to_srgb(math::srgb_to_rgb(math::rgb_to_srgb(original)));
    constexpr float tolerance = 0.0001F;
    context.expect_near(hsv_round_trip.r, original.r, tolerance, "HSV round trip preserves red within test tolerance.");
    context.expect_near(hsv_round_trip.g, original.g, tolerance,
                        "HSV round trip preserves green within test tolerance.");
    context.expect_near(hsv_round_trip.b, original.b, tolerance,
                        "HSV round trip preserves blue within test tolerance.");
    context.expect_near(hsl_round_trip.r, original.r, tolerance, "HSL round trip preserves red within test tolerance.");
    context.expect_near(hsl_round_trip.g, original.g, tolerance,
                        "HSL round trip preserves green within test tolerance.");
    context.expect_near(hsl_round_trip.b, original.b, tolerance,
                        "HSL round trip preserves blue within test tolerance.");
    context.expect_near(srgb_round_trip.r, math::rgb_to_srgb(original).r, tolerance,
                        "sRGB transfer round trip preserves red within test tolerance.");
}

static_assert(std::is_trivially_copyable_v<math::rgb>);
static_assert(std::is_trivially_copyable_v<math::color>);
static_assert(std::is_trivially_copyable_v<math::float4>);
static_assert(std::is_trivially_copyable_v<math::int4>);
static_assert(std::is_trivially_copyable_v<math::uint4>);

} // namespace

int main()
{
    TestContext context;
    test_float_vectors(context);
    test_integer_vectors(context);
    test_matrices(context);
    test_color(context);

    if (context.failures() != 0)
    {
        return 1;
    }

    std::cout << "Math tests passed.\n";
    return 0;
}
