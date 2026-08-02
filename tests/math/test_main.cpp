#include <cmath>
#include <iostream>
#include <numbers>
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

    void expect_near(const math::float3& actual, const math::float3& expected, float tolerance,
                     std::string_view message)
    {
        expect(std::fabs(actual.x - expected.x) <= tolerance && std::fabs(actual.y - expected.y) <= tolerance &&
                   std::fabs(actual.z - expected.z) <= tolerance,
               message);
    }

    void expect_near(const math::quaternion& actual, const math::quaternion& expected, float tolerance,
                     std::string_view message)
    {
        expect(std::fabs(actual.x - expected.x) <= tolerance && std::fabs(actual.y - expected.y) <= tolerance &&
                   std::fabs(actual.z - expected.z) <= tolerance && std::fabs(actual.w - expected.w) <= tolerance,
               message);
    }

    void expect_near(const math::float3x3& actual, const math::float3x3& expected, float tolerance,
                     std::string_view message)
    {
        expect_near(actual.c0, expected.c0, tolerance, message);
        expect_near(actual.c1, expected.c1, tolerance, message);
        expect_near(actual.c2, expected.c2, tolerance, message);
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

    context.expect(math::normalize(vector2) == math::float2{0.6F, 0.8F},
                   "float2 normalization has expected components.");

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

void test_quaternions(TestContext& context)
{
    constexpr float tolerance = 0.0001F;
    constexpr float half_pi = std::numbers::pi_v<float> * 0.5F;

    context.expect(math::identity<math::quaternion> == math::quaternion{0.0F, 0.0F, 0.0F, 1.0F},
                   "Quaternion identity has a zero vector part and unit scalar part.");
    context.expect(math::normalize(math::quaternion{0.0F, 0.0F, 0.0F, 2.0F}) == math::identity<math::quaternion>,
                   "Quaternion normalization returns a direct normalized value.");
    context.expect(math::dot(math::quaternion{1.0F, 2.0F, 3.0F, 4.0F}, math::quaternion{1.0F, 2.0F, 3.0F, 4.0F}) ==
                       30.0F,
                   "Quaternion dot product is exact for known components.");
    context.expect(math::length_squared(math::quaternion{1.0F, 2.0F, 3.0F, 4.0F}) == 30.0F,
                   "Quaternion squared length uses all four components.");
    const math::quaternion vector_value{1.0F, 2.0F, 3.0F, 4.0F};
    context.expect((vector_value + math::quaternion{4.0F, 3.0F, 2.0F, 1.0F}) ==
                       math::quaternion{5.0F, 5.0F, 5.0F, 5.0F},
                   "Quaternion vector traits reuse vector addition.");
    context.expect(math::swizzle<3, 0>(vector_value) == math::float2{4.0F, 1.0F},
                   "Quaternion vector traits reuse vector swizzles.");
    context.expect(math::inverse(math::quaternion{0.0F, 0.0F, 0.0F, 2.0F}) == math::quaternion{0.0F, 0.0F, 0.0F, 0.5F},
                   "Quaternion inverse accounts for non-unit length.");
    context.expect(math::conjugate(math::quaternion{1.0F, -2.0F, 3.0F, -4.0F}) ==
                       math::quaternion{-1.0F, 2.0F, -3.0F, -4.0F},
                   "Quaternion conjugation negates only the vector part.");

    const math::quaternion z_quarter_turn = math::quaternion_from_axis_angle(math::float3{0.0F, 0.0F, 2.0F}, half_pi);
    context.expect_near(math::length(z_quarter_turn), 1.0F, tolerance,
                        "Axis-angle construction normalizes the input axis.");
    context.expect_near(math::rotate(z_quarter_turn, math::float3{1.0F, 0.0F, 0.0F}), math::float3{0.0F, 1.0F, 0.0F},
                        tolerance, "A positive 90-degree Z rotation follows the right-hand rule.");

    const math::quaternion y_half_turn =
        math::quaternion_from_axis_angle(math::float3{0.0F, 1.0F, 0.0F}, std::numbers::pi_v<float>);
    context.expect_near(math::rotate(y_half_turn, math::float3{1.0F, 0.0F, 0.0F}), math::float3{-1.0F, 0.0F, 0.0F},
                        tolerance, "A positive 180-degree Y rotation reverses the X axis.");

    const math::quaternion inverse_rotation = math::inverse(z_quarter_turn);
    context.expect_near(math::hamilton_product(z_quarter_turn, inverse_rotation), math::identity<math::quaternion>,
                        tolerance, "A quaternion multiplied by its inverse produces identity.");
    context.expect_near(math::rotate(inverse_rotation, math::rotate(z_quarter_turn, math::float3{0.25F, -0.5F, 1.0F})),
                        math::float3{0.25F, -0.5F, 1.0F}, tolerance, "Inverse rotation restores the original vector.");

    const math::quaternion x_quarter_turn = math::quaternion_from_axis_angle(math::float3{1.0F, 0.0F, 0.0F}, half_pi);
    const math::float3 basis_x{1.0F, 0.0F, 0.0F};
    const math::float3 x_after_z_then_x = math::rotate(math::hamilton_product(x_quarter_turn, z_quarter_turn), basis_x);
    const math::float3 x_after_x_then_z = math::rotate(math::hamilton_product(z_quarter_turn, x_quarter_turn), basis_x);
    context.expect_near(x_after_z_then_x, math::float3{0.0F, 0.0F, 1.0F}, tolerance,
                        "hamilton_product(A, B) applies B before A.");
    context.expect_near(x_after_x_then_z, math::float3{0.0F, 1.0F, 0.0F}, tolerance,
                        "Quaternion composition is non-commutative.");

    const math::float3x3 x_matrix = math::quaternion_to_float3x3(x_quarter_turn);
    const math::float3x3 z_matrix = math::quaternion_to_float3x3(z_quarter_turn);
    context.expect_near(math::quaternion_to_float3x3(math::hamilton_product(x_quarter_turn, z_quarter_turn)),
                        x_matrix * z_matrix, tolerance,
                        "Quaternion and matrix composition use the same right-to-left contract.");
    context.expect_near(z_matrix * math::float3{0.5F, -1.0F, 2.0F},
                        math::rotate(z_quarter_turn, math::float3{0.5F, -1.0F, 2.0F}), tolerance,
                        "Quaternion and float3x3 vector rotations are equivalent.");

    const math::quaternion source = math::normalize(math::quaternion{0.2F, -0.3F, 0.4F, -0.5F});
    const math::float3x3 source_matrix = math::quaternion_to_float3x3(source);
    const math::quaternion round_trip = math::float3x3_to_quaternion(source_matrix, 0.00001F, 0.00001F);
    context.expect_near(round_trip, math::quaternion{-source.x, -source.y, -source.z, -source.w}, tolerance,
                        "Matrix conversion normalizes and canonicalizes the quaternion sign.");
    context.expect(round_trip.w >= 0.0F, "The canonical quaternion sign makes a non-zero scalar part positive.");

    const math::quaternion opposite_sign{-source.x, -source.y, -source.z, -source.w};
    context.expect_near(math::quaternion_to_float3x3(opposite_sign), source_matrix, tolerance,
                        "Opposite quaternion signs produce the same rotation matrix.");

    const math::float3x3 x_half_turn{{1.0F, 0.0F, 0.0F}, {0.0F, -1.0F, 0.0F}, {0.0F, 0.0F, -1.0F}};
    context.expect(math::float3x3_to_quaternion(x_half_turn, 0.0F, 0.0F) == math::quaternion{1.0F, 0.0F, 0.0F, 0.0F},
                   "A 180-degree matrix uses the first non-zero component for canonical sign.");

    math::float3x3 perturbed_rotation = z_matrix;
    perturbed_rotation.c0.x += 0.0001F;
    const math::quaternion tolerated = math::float3x3_to_quaternion(perturbed_rotation, 0.001F, 0.001F);
    context.expect_near(math::length(tolerated), 1.0F, tolerance,
                        "Matrix conversion accepts caller-selected absolute and relative tolerances.");
}

void test_color(TestContext& context)
{
    context.expect(math::colors::transparent == math::color{0.0F, 0.0F, 0.0F, 0.0F},
                   "Transparent color constant has zero alpha.");
    context.expect(math::colors::red == math::color{1.0F, 0.0F, 0.0F, 1.0F},
                   "Named color constants are linear RGBA values.");

    const math::color opaque_red{1.0F, 0.0F, 0.0F, 1.0F};
    context.expect(opaque_red.r == 1.0F && opaque_red.g == 0.0F && opaque_red.b == 0.0F && opaque_red.a == 1.0F,
                   "color stores RGBA channels directly.");

    const math::color hsv = math::rgb_to_hsv(opaque_red);
    context.expect(hsv == math::color{0.0F, 1.0F, 1.0F, 1.0F}, "Linear RGB converts red to HSV.");
    const math::color hsl = math::rgb_to_hsl(opaque_red);
    context.expect(hsl == math::color{0.0F, 1.0F, 0.5F, 1.0F}, "Linear RGB converts red to HSL.");

    const math::color original{0.25F, 0.5F, 0.75F, 0.8F};
    const math::color hsv_round_trip = math::hsv_to_rgb(math::rgb_to_hsv(original));
    const math::color hsl_round_trip = math::hsl_to_rgb(math::rgb_to_hsl(original));
    const math::color srgb_round_trip = math::rgb_to_srgb(math::srgb_to_rgb(math::rgb_to_srgb(original)));
    constexpr float tolerance = 0.0001F;
    context.expect_near(hsv_round_trip.r, original.r, tolerance, "HSV round trip preserves red within test tolerance.");
    context.expect_near(hsv_round_trip.g, original.g, tolerance,
                        "HSV round trip preserves green within test tolerance.");
    context.expect_near(hsv_round_trip.b, original.b, tolerance,
                        "HSV round trip preserves blue within test tolerance.");
    context.expect_near(hsv_round_trip.a, original.a, tolerance,
                        "HSV round trip preserves alpha within test tolerance.");
    context.expect_near(hsl_round_trip.r, original.r, tolerance, "HSL round trip preserves red within test tolerance.");
    context.expect_near(hsl_round_trip.g, original.g, tolerance,
                        "HSL round trip preserves green within test tolerance.");
    context.expect_near(hsl_round_trip.b, original.b, tolerance,
                        "HSL round trip preserves blue within test tolerance.");
    context.expect_near(hsl_round_trip.a, original.a, tolerance,
                        "HSL round trip preserves alpha within test tolerance.");
    context.expect_near(srgb_round_trip.r, math::rgb_to_srgb(original).r, tolerance,
                        "sRGB transfer round trip preserves red within test tolerance.");
    context.expect_near(srgb_round_trip.g, math::rgb_to_srgb(original).g, tolerance,
                        "sRGB transfer round trip preserves green within test tolerance.");
    context.expect_near(srgb_round_trip.b, math::rgb_to_srgb(original).b, tolerance,
                        "sRGB transfer round trip preserves blue within test tolerance.");
    context.expect_near(srgb_round_trip.a, original.a, tolerance,
                        "sRGB transfer round trip preserves alpha within test tolerance.");
}

static_assert(std::is_trivially_copyable_v<math::color>);
static_assert(!std::is_same_v<math::quaternion, math::float4>);
static_assert(std::is_trivially_copyable_v<math::quaternion>);
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
    test_quaternions(context);
    test_color(context);

    if (context.failures() != 0)
    {
        return 1;
    }

    std::cout << "Math tests passed.\n";
    return 0;
}
