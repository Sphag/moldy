module;

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#if defined(MOLDY_MATH_ASSERT_BACKEND_CORE)
#include <moldy/core_macros.hpp>
#elif defined(MOLDY_MATH_ASSERT_BACKEND_STANDALONE)
#include <cstdio>
#include <cstdlib>
#elif defined(MOLDY_MATH_ASSERT_BACKEND_CUSTOM)
#include MOLDY_MATH_ASSERT_HEADER
#ifndef MOLDY_MATH_ASSERT
#error "MOLDY_MATH_ASSERT_HEADER must define MOLDY_MATH_ASSERT(expression, message)."
#endif
#else
#error "A moldy.math assertion backend must be selected by CMake."
#endif

export module moldy.math;

#if defined(MOLDY_MATH_ASSERT_BACKEND_CORE)
import moldy.core;
#define MOLDY_MATH_ASSERT(expression, message) MOLDY_ASSERT_MSG((expression), "%s", (message))
#elif defined(MOLDY_MATH_ASSERT_BACKEND_STANDALONE)
#if defined(MOLDY_MATH_ENABLE_ASSERTS)
#define MOLDY_MATH_ASSERT(expression, message)                                                                         \
    ((expression) ? (void)0 : (std::fputs((message), stderr), std::fputc('\n', stderr), std::abort()))
#else
#define MOLDY_MATH_ASSERT(expression, message) ((void)0)
#endif
#endif

#define MOLDY_DEFINE_VECTOR2(TypeName, ScalarType)                                                                     \
    struct TypeName                                                                                                    \
    {                                                                                                                  \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType x;                                                                                              \
            ScalarType r;                                                                                              \
            ScalarType u;                                                                                              \
        };                                                                                                             \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType y;                                                                                              \
            ScalarType g;                                                                                              \
            ScalarType v;                                                                                              \
        };                                                                                                             \
                                                                                                                       \
        constexpr TypeName() noexcept : x(ScalarType{}), y(ScalarType{}) {}                                            \
        constexpr TypeName(ScalarType x_value, ScalarType y_value) noexcept : x(x_value), y(y_value) {}                \
    }

#define MOLDY_DEFINE_VECTOR3(TypeName, ScalarType)                                                                     \
    struct TypeName                                                                                                    \
    {                                                                                                                  \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType x;                                                                                              \
            ScalarType r;                                                                                              \
            ScalarType u;                                                                                              \
        };                                                                                                             \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType y;                                                                                              \
            ScalarType g;                                                                                              \
            ScalarType v;                                                                                              \
        };                                                                                                             \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType z;                                                                                              \
            ScalarType b;                                                                                              \
        };                                                                                                             \
                                                                                                                       \
        constexpr TypeName() noexcept : x(ScalarType{}), y(ScalarType{}), z(ScalarType{}) {}                           \
        constexpr TypeName(ScalarType x_value, ScalarType y_value, ScalarType z_value) noexcept                        \
            : x(x_value), y(y_value), z(z_value)                                                                       \
        {                                                                                                              \
        }                                                                                                              \
    }

#define MOLDY_DEFINE_VECTOR4(TypeName, ScalarType)                                                                     \
    struct TypeName                                                                                                    \
    {                                                                                                                  \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType x;                                                                                              \
            ScalarType r;                                                                                              \
            ScalarType u;                                                                                              \
        };                                                                                                             \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType y;                                                                                              \
            ScalarType g;                                                                                              \
            ScalarType v;                                                                                              \
        };                                                                                                             \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType z;                                                                                              \
            ScalarType b;                                                                                              \
        };                                                                                                             \
        union                                                                                                          \
        {                                                                                                              \
            ScalarType w;                                                                                              \
            ScalarType a;                                                                                              \
        };                                                                                                             \
                                                                                                                       \
        constexpr TypeName() noexcept : x(ScalarType{}), y(ScalarType{}), z(ScalarType{}), w(ScalarType{}) {}          \
        constexpr TypeName(ScalarType x_value, ScalarType y_value, ScalarType z_value, ScalarType w_value) noexcept    \
            : x(x_value), y(y_value), z(z_value), w(w_value)                                                           \
        {                                                                                                              \
        }                                                                                                              \
    }

// clang-format 18 and the CI formatter disagree on export-block brace placement.
// clang-format off
export {
    // clang-format on
    namespace math
    {

    using uint = std::uint32_t;

    // NOLINTBEGIN(readability-identifier-naming): HLSL-style public type names.
    MOLDY_DEFINE_VECTOR2(float2, float);
    MOLDY_DEFINE_VECTOR3(float3, float);
    MOLDY_DEFINE_VECTOR4(float4, float);
    MOLDY_DEFINE_VECTOR2(int2, int);
    MOLDY_DEFINE_VECTOR3(int3, int);
    MOLDY_DEFINE_VECTOR4(int4, int);
    MOLDY_DEFINE_VECTOR2(uint2, uint);
    MOLDY_DEFINE_VECTOR3(uint3, uint);
    MOLDY_DEFINE_VECTOR4(uint4, uint);

    struct color
    {
        float r{0.0F};
        float g{0.0F};
        float b{0.0F};
        float a{0.0F};

        constexpr color() noexcept = default;
        constexpr color(float red, float green, float blue, float alpha = 1.0F) noexcept
            : r(red), g(green), b(blue), a(alpha)
        {
        }
    };

    struct quaternion
    {
        float x{0.0F};
        float y{0.0F};
        float z{0.0F};
        float w{0.0F};

        constexpr quaternion() noexcept = default;
        constexpr quaternion(float x_value, float y_value, float z_value, float w_value) noexcept
            : x(x_value), y(y_value), z(z_value), w(w_value)
        {
        }
    };

    // NOLINTEND(readability-identifier-naming)

    namespace detail
    {

    template <typename TVector> struct vector_traits;

#define MOLDY_DEFINE_VECTOR_TRAITS(TypeName, ScalarType, Dimension)                                                    \
    template <> struct vector_traits<TypeName>                                                                         \
    {                                                                                                                  \
        using scalar_type = ScalarType;                                                                                \
        static constexpr std::size_t dimension = Dimension;                                                            \
    }

    MOLDY_DEFINE_VECTOR_TRAITS(float2, float, 2);
    MOLDY_DEFINE_VECTOR_TRAITS(float3, float, 3);
    MOLDY_DEFINE_VECTOR_TRAITS(float4, float, 4);
    MOLDY_DEFINE_VECTOR_TRAITS(int2, int, 2);
    MOLDY_DEFINE_VECTOR_TRAITS(int3, int, 3);
    MOLDY_DEFINE_VECTOR_TRAITS(int4, int, 4);
    MOLDY_DEFINE_VECTOR_TRAITS(uint2, uint, 2);
    MOLDY_DEFINE_VECTOR_TRAITS(uint3, uint, 3);
    MOLDY_DEFINE_VECTOR_TRAITS(uint4, uint, 4);
    MOLDY_DEFINE_VECTOR_TRAITS(quaternion, float, 4);

    template <typename TScalar, std::size_t TDimension> struct vector_for;
    template <> struct vector_for<float, 2>
    {
        using type = float2;
    };
    template <> struct vector_for<float, 3>
    {
        using type = float3;
    };
    template <> struct vector_for<float, 4>
    {
        using type = float4;
    };
    template <> struct vector_for<int, 2>
    {
        using type = int2;
    };
    template <> struct vector_for<int, 3>
    {
        using type = int3;
    };
    template <> struct vector_for<int, 4>
    {
        using type = int4;
    };
    template <> struct vector_for<uint, 2>
    {
        using type = uint2;
    };
    template <> struct vector_for<uint, 3>
    {
        using type = uint3;
    };
    template <> struct vector_for<uint, 4>
    {
        using type = uint4;
    };

    template <typename TScalar, std::size_t TDimension>
    using vector_for_t = typename vector_for<TScalar, TDimension>::type;

    template <typename TVector>
    concept vector_type = requires { typename vector_traits<TVector>::scalar_type; };

    template <vector_type TVector> using vector_scalar_t = typename vector_traits<TVector>::scalar_type;

    template <vector_type TVector>
    [[nodiscard]] constexpr vector_scalar_t<TVector> component(const TVector& vector, std::size_t index) noexcept
    {
        if constexpr (vector_traits<TVector>::dimension == 2)
        {
            return index == 0 ? vector.x : vector.y;
        }
        else if constexpr (vector_traits<TVector>::dimension == 3)
        {
            return index == 0 ? vector.x : (index == 1 ? vector.y : vector.z);
        }
        else
        {
            return index == 0 ? vector.x : (index == 1 ? vector.y : (index == 2 ? vector.z : vector.w));
        }
    }

    template <typename TScalar, std::size_t TDimension, typename... TValues>
    [[nodiscard]] constexpr vector_for_t<TScalar, TDimension> make_vector(TValues... values) noexcept
    {
        static_assert(sizeof...(TValues) == TDimension);
        return {static_cast<TScalar>(values)...};
    }

    } // namespace detail

    template <typename TScalar, std::size_t TDimension> struct matrix;

    template <typename TScalar> struct matrix<TScalar, 2>
    {
        using column_type = detail::vector_for_t<TScalar, 2>;

        column_type c0{};
        column_type c1{};

        constexpr matrix() noexcept = default;
        constexpr matrix(column_type column0, column_type column1) noexcept : c0(column0), c1(column1) {}
    };

    template <typename TScalar> struct matrix<TScalar, 3>
    {
        using column_type = detail::vector_for_t<TScalar, 3>;

        column_type c0{};
        column_type c1{};
        column_type c2{};

        constexpr matrix() noexcept = default;
        constexpr matrix(column_type column0, column_type column1, column_type column2) noexcept
            : c0(column0), c1(column1), c2(column2)
        {
        }
    };

    template <typename TScalar> struct matrix<TScalar, 4>
    {
        using column_type = detail::vector_for_t<TScalar, 4>;

        column_type c0{};
        column_type c1{};
        column_type c2{};
        column_type c3{};

        constexpr matrix() noexcept = default;
        constexpr matrix(column_type column0, column_type column1, column_type column2, column_type column3) noexcept
            : c0(column0), c1(column1), c2(column2), c3(column3)
        {
        }
    };

    namespace detail
    {

    template <typename TMatrix> struct matrix_traits;

    template <typename TScalar, std::size_t TDimension> struct matrix_traits<matrix<TScalar, TDimension>>
    {
        using scalar_type = TScalar;
        static constexpr std::size_t dimension = TDimension;
    };

    template <typename TMatrix>
    concept matrix_type = requires { typename matrix_traits<TMatrix>::scalar_type; };

    template <matrix_type TMatrix> using matrix_scalar_t = typename matrix_traits<TMatrix>::scalar_type;

    template <typename TValue>
    concept identity_type = matrix_type<TValue> || std::same_as<TValue, quaternion>;

    template <matrix_type TMatrix> [[nodiscard]] constexpr auto column(const TMatrix& value, std::size_t index) noexcept
    {
        if constexpr (matrix_traits<TMatrix>::dimension == 2)
        {
            return index == 0 ? value.c0 : value.c1;
        }
        else if constexpr (matrix_traits<TMatrix>::dimension == 3)
        {
            return index == 0 ? value.c0 : (index == 1 ? value.c1 : value.c2);
        }
        else
        {
            return index == 0 ? value.c0 : (index == 1 ? value.c1 : (index == 2 ? value.c2 : value.c3));
        }
    }

    } // namespace detail

    using float2x2 = matrix<float, 2>;
    using float3x3 = matrix<float, 3>;
    using float4x4 = matrix<float, 4>;
    using int2x2 = matrix<int, 2>;
    using int3x3 = matrix<int, 3>;
    using int4x4 = matrix<int, 4>;
    using uint2x2 = matrix<uint, 2>;
    using uint3x3 = matrix<uint, 3>;
    using uint4x4 = matrix<uint, 4>;

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr bool operator==(const TVector& left, const TVector& right) noexcept
    {
        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return left.x == right.x && left.y == right.y;
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return left.x == right.x && left.y == right.y && left.z == right.z;
        }
        else
        {
            return left.x == right.x && left.y == right.y && left.z == right.z && left.w == right.w;
        }
    }

    [[nodiscard]] constexpr bool operator==(const color& left, const color& right) noexcept
    {
        return left.r == right.r && left.g == right.g && left.b == right.b && left.a == right.a;
    }

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr TVector operator+(const TVector& left, const TVector& right) noexcept
    {
        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return {left.x + right.x, left.y + right.y};
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return {left.x + right.x, left.y + right.y, left.z + right.z};
        }
        else
        {
            return {left.x + right.x, left.y + right.y, left.z + right.z, left.w + right.w};
        }
    }

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr TVector operator-(const TVector& left, const TVector& right) noexcept
    {
        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return {left.x - right.x, left.y - right.y};
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return {left.x - right.x, left.y - right.y, left.z - right.z};
        }
        else
        {
            return {left.x - right.x, left.y - right.y, left.z - right.z, left.w - right.w};
        }
    }

    template <detail::vector_type TVector> [[nodiscard]] constexpr TVector operator-(const TVector& vector) noexcept
    {
        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return {-vector.x, -vector.y};
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return {-vector.x, -vector.y, -vector.z};
        }
        else
        {
            return {-vector.x, -vector.y, -vector.z, -vector.w};
        }
    }

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr TVector operator*(const TVector& vector, detail::vector_scalar_t<TVector> scalar) noexcept
    {
        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return {vector.x * scalar, vector.y * scalar};
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return {vector.x * scalar, vector.y * scalar, vector.z * scalar};
        }
        else
        {
            return {vector.x * scalar, vector.y * scalar, vector.z * scalar, vector.w * scalar};
        }
    }

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr TVector operator*(detail::vector_scalar_t<TVector> scalar, const TVector& vector) noexcept
    {
        return vector * scalar;
    }

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr TVector operator/(const TVector& vector, detail::vector_scalar_t<TVector> scalar) noexcept
    {
        MOLDY_MATH_ASSERT(scalar != detail::vector_scalar_t<TVector>{}, "Vector division requires a non-zero scalar.");

        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return {vector.x / scalar, vector.y / scalar};
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return {vector.x / scalar, vector.y / scalar, vector.z / scalar};
        }
        else
        {
            return {vector.x / scalar, vector.y / scalar, vector.z / scalar, vector.w / scalar};
        }
    }

    template <detail::vector_type TVector>
    [[nodiscard]] constexpr detail::vector_scalar_t<TVector> dot(const TVector& left, const TVector& right) noexcept
    {
        if constexpr (detail::vector_traits<TVector>::dimension == 2)
        {
            return (left.x * right.x) + (left.y * right.y);
        }
        else if constexpr (detail::vector_traits<TVector>::dimension == 3)
        {
            return (left.x * right.x) + (left.y * right.y) + (left.z * right.z);
        }
        else
        {
            return (left.x * right.x) + (left.y * right.y) + (left.z * right.z) + (left.w * right.w);
        }
    }

    [[nodiscard]] constexpr float3 cross(const float3& left, const float3& right) noexcept
    {
        return {(left.y * right.z) - (left.z * right.y), (left.z * right.x) - (left.x * right.z),
                (left.x * right.y) - (left.y * right.x)};
    }

    template <detail::vector_type TVector>
        requires std::same_as<detail::vector_scalar_t<TVector>, float>
    [[nodiscard]] constexpr float length_squared(const TVector& vector) noexcept
    {
        return dot(vector, vector);
    }

    template <detail::vector_type TVector>
        requires std::same_as<detail::vector_scalar_t<TVector>, float>
    [[nodiscard]] float length(const TVector& vector) noexcept
    {
        return std::sqrt(length_squared(vector));
    }

    template <detail::vector_type TVector>
        requires std::same_as<detail::vector_scalar_t<TVector>, float>
    [[nodiscard]] TVector normalize(const TVector& vector) noexcept
    {
        const float vector_length = length(vector);
        MOLDY_MATH_ASSERT(vector_length != 0.0F, "Vector normalization requires a non-zero length.");

        return vector / vector_length;
    }

    [[nodiscard]] constexpr quaternion hamilton_product(const quaternion& left, const quaternion& right) noexcept
    {
        return {(left.w * right.x) + (left.x * right.w) + (left.y * right.z) - (left.z * right.y),
                (left.w * right.y) - (left.x * right.z) + (left.y * right.w) + (left.z * right.x),
                (left.w * right.z) + (left.x * right.y) - (left.y * right.x) + (left.z * right.w),
                (left.w * right.w) - (left.x * right.x) - (left.y * right.y) - (left.z * right.z)};
    }

    [[nodiscard]] constexpr quaternion conjugate(const quaternion& value) noexcept
    {
        return {-value.x, -value.y, -value.z, value.w};
    }

    [[nodiscard]] quaternion inverse(const quaternion& value) noexcept
    {
        const float squared_length = length_squared(value);
        MOLDY_MATH_ASSERT(squared_length != 0.0F, "Quaternion inversion requires a non-zero length.");
        return conjugate(value) / squared_length;
    }

    [[nodiscard]] float3 rotate(const quaternion& rotation, const float3& vector) noexcept
    {
        const float squared_length = length_squared(rotation);
        MOLDY_MATH_ASSERT(squared_length != 0.0F, "Vector rotation requires a non-zero quaternion.");

        const quaternion inverse_rotation = conjugate(rotation) / squared_length;
        const quaternion rotated = hamilton_product(
            hamilton_product(rotation, quaternion{vector.x, vector.y, vector.z, 0.0F}), inverse_rotation);
        return {rotated.x, rotated.y, rotated.z};
    }

    [[nodiscard]] quaternion quaternion_from_axis_angle(const float3& axis, float radians) noexcept
    {
        const float axis_length = length(axis);
        MOLDY_MATH_ASSERT(axis_length != 0.0F, "Axis-angle construction requires a non-zero axis.");

        const float3 normalized_axis = axis / axis_length;
        const float half_angle = radians * 0.5F;
        const float sine = std::sin(half_angle);
        return {normalized_axis.x * sine, normalized_axis.y * sine, normalized_axis.z * sine, std::cos(half_angle)};
    }

    template <std::size_t... TIndices, detail::vector_type TVector>
        requires(sizeof...(TIndices) >= 2 && sizeof...(TIndices) <= 4)
    [[nodiscard]] constexpr auto swizzle(const TVector& vector) noexcept
        -> detail::vector_for_t<detail::vector_scalar_t<TVector>, sizeof...(TIndices)>
    {
        static_assert(((TIndices < detail::vector_traits<TVector>::dimension) && ...));
        return detail::make_vector<detail::vector_scalar_t<TVector>, sizeof...(TIndices)>(
            detail::component(vector, TIndices)...);
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr bool operator==(const TMatrix& left, const TMatrix& right) noexcept
    {
        if constexpr (detail::matrix_traits<TMatrix>::dimension == 2)
        {
            return left.c0 == right.c0 && left.c1 == right.c1;
        }
        else if constexpr (detail::matrix_traits<TMatrix>::dimension == 3)
        {
            return left.c0 == right.c0 && left.c1 == right.c1 && left.c2 == right.c2;
        }
        else
        {
            return left.c0 == right.c0 && left.c1 == right.c1 && left.c2 == right.c2 && left.c3 == right.c3;
        }
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr TMatrix operator+(const TMatrix& left, const TMatrix& right) noexcept
    {
        if constexpr (detail::matrix_traits<TMatrix>::dimension == 2)
        {
            return {left.c0 + right.c0, left.c1 + right.c1};
        }
        else if constexpr (detail::matrix_traits<TMatrix>::dimension == 3)
        {
            return {left.c0 + right.c0, left.c1 + right.c1, left.c2 + right.c2};
        }
        else
        {
            return {left.c0 + right.c0, left.c1 + right.c1, left.c2 + right.c2, left.c3 + right.c3};
        }
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr TMatrix operator-(const TMatrix& left, const TMatrix& right) noexcept
    {
        if constexpr (detail::matrix_traits<TMatrix>::dimension == 2)
        {
            return {left.c0 - right.c0, left.c1 - right.c1};
        }
        else if constexpr (detail::matrix_traits<TMatrix>::dimension == 3)
        {
            return {left.c0 - right.c0, left.c1 - right.c1, left.c2 - right.c2};
        }
        else
        {
            return {left.c0 - right.c0, left.c1 - right.c1, left.c2 - right.c2, left.c3 - right.c3};
        }
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr TMatrix operator*(const TMatrix& matrix, detail::matrix_scalar_t<TMatrix> scalar) noexcept
    {
        if constexpr (detail::matrix_traits<TMatrix>::dimension == 2)
        {
            return {matrix.c0 * scalar, matrix.c1 * scalar};
        }
        else if constexpr (detail::matrix_traits<TMatrix>::dimension == 3)
        {
            return {matrix.c0 * scalar, matrix.c1 * scalar, matrix.c2 * scalar};
        }
        else
        {
            return {matrix.c0 * scalar, matrix.c1 * scalar, matrix.c2 * scalar, matrix.c3 * scalar};
        }
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr TMatrix operator*(detail::matrix_scalar_t<TMatrix> scalar, const TMatrix& matrix) noexcept
    {
        return matrix * scalar;
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr auto
    operator*(const TMatrix& matrix,
              const detail::vector_for_t<detail::matrix_scalar_t<TMatrix>, detail::matrix_traits<TMatrix>::dimension>&
                  vector) noexcept
        -> detail::vector_for_t<detail::matrix_scalar_t<TMatrix>, detail::matrix_traits<TMatrix>::dimension>
    {
        if constexpr (detail::matrix_traits<TMatrix>::dimension == 2)
        {
            return (matrix.c0 * vector.x) + (matrix.c1 * vector.y);
        }
        else if constexpr (detail::matrix_traits<TMatrix>::dimension == 3)
        {
            return (matrix.c0 * vector.x) + (matrix.c1 * vector.y) + (matrix.c2 * vector.z);
        }
        else
        {
            return (matrix.c0 * vector.x) + (matrix.c1 * vector.y) + (matrix.c2 * vector.z) + (matrix.c3 * vector.w);
        }
    }

    template <detail::matrix_type TMatrix>
    [[nodiscard]] constexpr TMatrix operator*(const TMatrix& left, const TMatrix& right) noexcept
    {
        if constexpr (detail::matrix_traits<TMatrix>::dimension == 2)
        {
            return {left * right.c0, left * right.c1};
        }
        else if constexpr (detail::matrix_traits<TMatrix>::dimension == 3)
        {
            return {left * right.c0, left * right.c1, left * right.c2};
        }
        else
        {
            return {left * right.c0, left * right.c1, left * right.c2, left * right.c3};
        }
    }

    [[nodiscard]] float3x3 quaternion_to_float3x3(const quaternion& value) noexcept
    {
        const float quaternion_length = length(value);
        MOLDY_MATH_ASSERT(quaternion_length != 0.0F, "Quaternion-to-matrix conversion requires a non-zero quaternion.");

        const quaternion rotation = value / quaternion_length;
        const float xx = rotation.x * rotation.x;
        const float yy = rotation.y * rotation.y;
        const float zz = rotation.z * rotation.z;
        const float xy = rotation.x * rotation.y;
        const float xz = rotation.x * rotation.z;
        const float yz = rotation.y * rotation.z;
        const float xw = rotation.x * rotation.w;
        const float yw = rotation.y * rotation.w;
        const float zw = rotation.z * rotation.w;

        return {{1.0F - (2.0F * (yy + zz)), 2.0F * (xy + zw), 2.0F * (xz - yw)},
                {2.0F * (xy - zw), 1.0F - (2.0F * (xx + zz)), 2.0F * (yz + xw)},
                {2.0F * (xz + yw), 2.0F * (yz - xw), 1.0F - (2.0F * (xx + yy))}};
    }

    namespace detail
    {

    [[nodiscard]] bool within_tolerance(float actual, float expected, float absolute_tolerance,
                                        float relative_tolerance) noexcept
    {
        const float difference = std::fabs(actual - expected);
        const float scale = std::fmax(std::fabs(actual), std::fabs(expected));
        return difference <= absolute_tolerance + (relative_tolerance * scale);
    }

    [[nodiscard]] constexpr float determinant(const float3x3& value) noexcept
    {
        return dot(value.c0, cross(value.c1, value.c2));
    }

    [[nodiscard]] bool is_proper_rotation_matrix(const float3x3& value, float absolute_tolerance,
                                                 float relative_tolerance) noexcept
    {
        const float value_determinant = determinant(value);
        return within_tolerance(dot(value.c0, value.c0), 1.0F, absolute_tolerance, relative_tolerance) &&
               within_tolerance(dot(value.c1, value.c1), 1.0F, absolute_tolerance, relative_tolerance) &&
               within_tolerance(dot(value.c2, value.c2), 1.0F, absolute_tolerance, relative_tolerance) &&
               within_tolerance(dot(value.c0, value.c1), 0.0F, absolute_tolerance, relative_tolerance) &&
               within_tolerance(dot(value.c0, value.c2), 0.0F, absolute_tolerance, relative_tolerance) &&
               within_tolerance(dot(value.c1, value.c2), 0.0F, absolute_tolerance, relative_tolerance) &&
               value_determinant > 0.0F &&
               within_tolerance(value_determinant, 1.0F, absolute_tolerance, relative_tolerance);
    }

    [[nodiscard]] constexpr quaternion canonicalize_sign(const quaternion& value) noexcept
    {
        const bool negate =
            value.w < 0.0F ||
            (value.w == 0.0F &&
             (value.x < 0.0F || (value.x == 0.0F && (value.y < 0.0F || (value.y == 0.0F && value.z < 0.0F)))));
        return negate ? -value : value;
    }

    } // namespace detail

    [[nodiscard]] quaternion float3x3_to_quaternion(const float3x3& value, float absolute_tolerance,
                                                    float relative_tolerance) noexcept
    {
        MOLDY_MATH_ASSERT(absolute_tolerance >= 0.0F, "Matrix-to-quaternion absolute tolerance must be non-negative.");
        MOLDY_MATH_ASSERT(relative_tolerance >= 0.0F, "Matrix-to-quaternion relative tolerance must be non-negative.");
        MOLDY_MATH_ASSERT(detail::is_proper_rotation_matrix(value, absolute_tolerance, relative_tolerance),
                          "Matrix-to-quaternion conversion requires a proper orthonormal rotation matrix.");
        (void)absolute_tolerance;
        (void)relative_tolerance;

        quaternion result;
        const float trace = value.c0.x + value.c1.y + value.c2.z;
        if (trace > 0.0F)
        {
            const float scale = 2.0F * std::sqrt(trace + 1.0F);
            result = {(value.c1.z - value.c2.y) / scale, (value.c2.x - value.c0.z) / scale,
                      (value.c0.y - value.c1.x) / scale, 0.25F * scale};
        }
        else if (value.c0.x > value.c1.y && value.c0.x > value.c2.z)
        {
            const float scale = 2.0F * std::sqrt(1.0F + value.c0.x - value.c1.y - value.c2.z);
            result = {0.25F * scale, (value.c1.x + value.c0.y) / scale, (value.c2.x + value.c0.z) / scale,
                      (value.c1.z - value.c2.y) / scale};
        }
        else if (value.c1.y > value.c2.z)
        {
            const float scale = 2.0F * std::sqrt(1.0F + value.c1.y - value.c0.x - value.c2.z);
            result = {(value.c1.x + value.c0.y) / scale, 0.25F * scale, (value.c2.y + value.c1.z) / scale,
                      (value.c2.x - value.c0.z) / scale};
        }
        else
        {
            const float scale = 2.0F * std::sqrt(1.0F + value.c2.z - value.c0.x - value.c1.y);
            result = {(value.c2.x + value.c0.z) / scale, (value.c2.y + value.c1.z) / scale, 0.25F * scale,
                      (value.c0.y - value.c1.x) / scale};
        }

        return detail::canonicalize_sign(normalize(result));
    }

    namespace detail
    {

    template <vector_type TVector> [[nodiscard]] constexpr TVector make_one() noexcept
    {
        using scalar_type = vector_scalar_t<TVector>;
        if constexpr (vector_traits<TVector>::dimension == 2)
        {
            return {scalar_type{1}, scalar_type{1}};
        }
        else if constexpr (vector_traits<TVector>::dimension == 3)
        {
            return {scalar_type{1}, scalar_type{1}, scalar_type{1}};
        }
        else
        {
            return {scalar_type{1}, scalar_type{1}, scalar_type{1}, scalar_type{1}};
        }
    }

    template <identity_type TValue> [[nodiscard]] constexpr TValue make_identity() noexcept
    {
        if constexpr (std::same_as<TValue, quaternion>)
        {
            return {0.0F, 0.0F, 0.0F, 1.0F};
        }
        else
        {
            using scalar_type = matrix_scalar_t<TValue>;
            if constexpr (matrix_traits<TValue>::dimension == 2)
            {
                return {{scalar_type{1}, scalar_type{}}, {scalar_type{}, scalar_type{1}}};
            }
            else if constexpr (matrix_traits<TValue>::dimension == 3)
            {
                return {{scalar_type{1}, scalar_type{}, scalar_type{}},
                        {scalar_type{}, scalar_type{1}, scalar_type{}},
                        {scalar_type{}, scalar_type{}, scalar_type{1}}};
            }
            else
            {
                return {{scalar_type{1}, scalar_type{}, scalar_type{}, scalar_type{}},
                        {scalar_type{}, scalar_type{1}, scalar_type{}, scalar_type{}},
                        {scalar_type{}, scalar_type{}, scalar_type{1}, scalar_type{}},
                        {scalar_type{}, scalar_type{}, scalar_type{}, scalar_type{1}}};
            }
        }
    }

    } // namespace detail

    template <typename TValue> inline constexpr TValue zero{};
    template <detail::vector_type TValue> inline constexpr TValue one = detail::make_one<TValue>();
    template <detail::identity_type TValue> inline constexpr TValue identity = detail::make_identity<TValue>();

    namespace colors
    {
    inline constexpr color transparent{0.0F, 0.0F, 0.0F, 0.0F};
    inline constexpr color black{0.0F, 0.0F, 0.0F};
    inline constexpr color white{1.0F, 1.0F, 1.0F};
    inline constexpr color red{1.0F, 0.0F, 0.0F};
    inline constexpr color green{0.0F, 1.0F, 0.0F};
    inline constexpr color blue{0.0F, 0.0F, 1.0F};
    inline constexpr color yellow{1.0F, 1.0F, 0.0F};
    inline constexpr color cyan{0.0F, 1.0F, 1.0F};
    inline constexpr color magenta{1.0F, 0.0F, 1.0F};
    } // namespace colors

    [[nodiscard]] color rgb_to_hsv(const color& rgb) noexcept
    {
        const float maximum = std::fmax(rgb.r, std::fmax(rgb.g, rgb.b));
        const float minimum = std::fmin(rgb.r, std::fmin(rgb.g, rgb.b));
        const float chroma = maximum - minimum;

        float hue = 0.0F;
        if (chroma != 0.0F)
        {
            if (maximum == rgb.r)
            {
                hue = std::fmod((rgb.g - rgb.b) / chroma, 6.0F);
            }
            else if (maximum == rgb.g)
            {
                hue = ((rgb.b - rgb.r) / chroma) + 2.0F;
            }
            else
            {
                hue = ((rgb.r - rgb.g) / chroma) + 4.0F;
            }

            hue /= 6.0F;
            if (hue < 0.0F)
            {
                hue += 1.0F;
            }
        }

        const float saturation = maximum == 0.0F ? 0.0F : chroma / maximum;
        return {hue, saturation, maximum, rgb.a};
    }

    [[nodiscard]] color hsv_to_rgb(const color& hsv) noexcept
    {
        const float hue = hsv.r - std::floor(hsv.r);
        const float chroma = hsv.g * hsv.b;
        const float sector = hue * 6.0F;
        const float intermediate = chroma * (1.0F - std::fabs(std::fmod(sector, 2.0F) - 1.0F));
        const float match = hsv.b - chroma;

        if (sector < 1.0F)
        {
            return {chroma + match, intermediate + match, match, hsv.a};
        }
        if (sector < 2.0F)
        {
            return {intermediate + match, chroma + match, match, hsv.a};
        }
        if (sector < 3.0F)
        {
            return {match, chroma + match, intermediate + match, hsv.a};
        }
        if (sector < 4.0F)
        {
            return {match, intermediate + match, chroma + match, hsv.a};
        }
        if (sector < 5.0F)
        {
            return {intermediate + match, match, chroma + match, hsv.a};
        }

        return {chroma + match, match, intermediate + match, hsv.a};
    }

    [[nodiscard]] color rgb_to_hsl(const color& rgb) noexcept
    {
        const float maximum = std::fmax(rgb.r, std::fmax(rgb.g, rgb.b));
        const float minimum = std::fmin(rgb.r, std::fmin(rgb.g, rgb.b));
        const float chroma = maximum - minimum;
        const float lightness = (maximum + minimum) * 0.5F;

        float hue = 0.0F;
        if (chroma != 0.0F)
        {
            if (maximum == rgb.r)
            {
                hue = std::fmod((rgb.g - rgb.b) / chroma, 6.0F);
            }
            else if (maximum == rgb.g)
            {
                hue = ((rgb.b - rgb.r) / chroma) + 2.0F;
            }
            else
            {
                hue = ((rgb.r - rgb.g) / chroma) + 4.0F;
            }

            hue /= 6.0F;
            if (hue < 0.0F)
            {
                hue += 1.0F;
            }
        }

        const float saturation = chroma == 0.0F ? 0.0F : chroma / (1.0F - std::fabs((2.0F * lightness) - 1.0F));
        return {hue, saturation, lightness, rgb.a};
    }

    [[nodiscard]] color hsl_to_rgb(const color& hsl) noexcept
    {
        const float hue = hsl.r - std::floor(hsl.r);
        const float chroma = (1.0F - std::fabs((2.0F * hsl.b) - 1.0F)) * hsl.g;
        const float sector = hue * 6.0F;
        const float intermediate = chroma * (1.0F - std::fabs(std::fmod(sector, 2.0F) - 1.0F));
        const float match = hsl.b - (chroma * 0.5F);

        if (sector < 1.0F)
        {
            return {chroma + match, intermediate + match, match, hsl.a};
        }
        if (sector < 2.0F)
        {
            return {intermediate + match, chroma + match, match, hsl.a};
        }
        if (sector < 3.0F)
        {
            return {match, chroma + match, intermediate + match, hsl.a};
        }
        if (sector < 4.0F)
        {
            return {match, intermediate + match, chroma + match, hsl.a};
        }
        if (sector < 5.0F)
        {
            return {intermediate + match, match, chroma + match, hsl.a};
        }

        return {chroma + match, match, intermediate + match, hsl.a};
    }

    [[nodiscard]] float srgb_to_rgb(float value) noexcept
    {
        return value <= 0.04045F ? value / 12.92F : std::pow((value + 0.055F) / 1.055F, 2.4F);
    }

    [[nodiscard]] float rgb_to_srgb(float value) noexcept
    {
        return value <= 0.0031308F ? value * 12.92F : (1.055F * std::pow(value, 1.0F / 2.4F)) - 0.055F;
    }

    [[nodiscard]] color srgb_to_rgb(const color& srgb) noexcept
    {
        return {srgb_to_rgb(srgb.r), srgb_to_rgb(srgb.g), srgb_to_rgb(srgb.b), srgb.a};
    }

    [[nodiscard]] color rgb_to_srgb(const color& rgb) noexcept
    {
        return {rgb_to_srgb(rgb.r), rgb_to_srgb(rgb.g), rgb_to_srgb(rgb.b), rgb.a};
    }

    [[nodiscard]] color srgb_to_hsv(const color& srgb) noexcept
    {
        return rgb_to_hsv(srgb_to_rgb(srgb));
    }

    [[nodiscard]] color hsv_to_srgb(const color& hsv) noexcept
    {
        return rgb_to_srgb(hsv_to_rgb(hsv));
    }

    [[nodiscard]] color srgb_to_hsl(const color& srgb) noexcept
    {
        return rgb_to_hsl(srgb_to_rgb(srgb));
    }

    [[nodiscard]] color hsl_to_srgb(const color& hsl) noexcept
    {
        return rgb_to_srgb(hsl_to_rgb(hsl));
    }

    } // namespace math
}

#undef MOLDY_DEFINE_VECTOR2
#undef MOLDY_DEFINE_VECTOR3
#undef MOLDY_DEFINE_VECTOR4
#undef MOLDY_DEFINE_VECTOR_TRAITS
#undef MOLDY_DEFINE_VECTOR_CONSTANTS
#undef MOLDY_DEFINE_IDENTITY
#undef MOLDY_MATH_ASSERT
