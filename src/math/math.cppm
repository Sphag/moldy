module;

#include <cmath>
#include <concepts>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

export module moldy.math;

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

export {
    namespace math
    {

    using uint = std::uint32_t;

    MOLDY_DEFINE_VECTOR2(float2, float);
    MOLDY_DEFINE_VECTOR3(float3, float);
    MOLDY_DEFINE_VECTOR4(float4, float);
    MOLDY_DEFINE_VECTOR2(int2, int);
    MOLDY_DEFINE_VECTOR3(int3, int);
    MOLDY_DEFINE_VECTOR4(int4, int);
    MOLDY_DEFINE_VECTOR2(uint2, uint);
    MOLDY_DEFINE_VECTOR3(uint3, uint);
    MOLDY_DEFINE_VECTOR4(uint4, uint);

    // NOLINTBEGIN(readability-identifier-naming): HLSL-style public type names.
    struct rgb
    {
        float r{0.0F};
        float g{0.0F};
        float b{0.0F};

        constexpr rgb() noexcept = default;
        constexpr rgb(float red, float green, float blue) noexcept : r(red), g(green), b(blue) {}
    };

    struct color
    {
        rgb linear{};
        float a{0.0F};

        constexpr color() noexcept = default;
        constexpr color(rgb linear_rgb, float alpha = 1.0F) noexcept : linear(linear_rgb), a(alpha) {}
        constexpr color(float red, float green, float blue, float alpha = 1.0F) noexcept
            : color(rgb{red, green, blue}, alpha)
        {
        }
    };

    struct hsv
    {
        float h{0.0F};
        float s{0.0F};
        float v{0.0F};

        constexpr hsv() noexcept = default;
        constexpr hsv(float hue, float saturation, float value) noexcept : h(hue), s(saturation), v(value) {}
    };

    struct hsl
    {
        float h{0.0F};
        float s{0.0F};
        float l{0.0F};

        constexpr hsl() noexcept = default;
        constexpr hsl(float hue, float saturation, float lightness) noexcept : h(hue), s(saturation), l(lightness) {}
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

    [[nodiscard]] constexpr bool operator==(const rgb& left, const rgb& right) noexcept
    {
        return left.r == right.r && left.g == right.g && left.b == right.b;
    }

    [[nodiscard]] constexpr bool operator==(const color& left, const color& right) noexcept
    {
        return left.linear == right.linear && left.a == right.a;
    }

    [[nodiscard]] constexpr bool operator==(const hsv& left, const hsv& right) noexcept
    {
        return left.h == right.h && left.s == right.s && left.v == right.v;
    }

    [[nodiscard]] constexpr bool operator==(const hsl& left, const hsl& right) noexcept
    {
        return left.h == right.h && left.s == right.s && left.l == right.l;
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

    // Precondition: scalar is non-zero.
    template <detail::vector_type TVector>
    [[nodiscard]] constexpr TVector operator/(const TVector& vector, detail::vector_scalar_t<TVector> scalar) noexcept
    {
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
    [[nodiscard]] std::optional<TVector> normalize(const TVector& vector) noexcept
    {
        const float vector_length = length(vector);
        if (vector_length == 0.0F)
        {
            return std::nullopt;
        }

        return vector / vector_length;
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

    template <matrix_type TMatrix> [[nodiscard]] constexpr TMatrix make_identity() noexcept
    {
        using scalar_type = matrix_scalar_t<TMatrix>;
        if constexpr (matrix_traits<TMatrix>::dimension == 2)
        {
            return {{scalar_type{1}, scalar_type{}}, {scalar_type{}, scalar_type{1}}};
        }
        else if constexpr (matrix_traits<TMatrix>::dimension == 3)
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

    } // namespace detail

    template <typename TValue> inline constexpr TValue zero{};
    template <detail::vector_type TValue> inline constexpr TValue one = detail::make_one<TValue>();
    template <detail::matrix_type TValue> inline constexpr TValue identity = detail::make_identity<TValue>();

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

    [[nodiscard]] hsv rgb_to_hsv(const rgb& rgb) noexcept
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
        return {hue, saturation, maximum};
    }

    [[nodiscard]] rgb hsv_to_rgb(const hsv& hsv) noexcept
    {
        const float hue = hsv.h - std::floor(hsv.h);
        const float chroma = hsv.s * hsv.v;
        const float sector = hue * 6.0F;
        const float intermediate = chroma * (1.0F - std::fabs(std::fmod(sector, 2.0F) - 1.0F));
        const float match = hsv.v - chroma;

        if (sector < 1.0F)
        {
            return {chroma + match, intermediate + match, match};
        }
        if (sector < 2.0F)
        {
            return {intermediate + match, chroma + match, match};
        }
        if (sector < 3.0F)
        {
            return {match, chroma + match, intermediate + match};
        }
        if (sector < 4.0F)
        {
            return {match, intermediate + match, chroma + match};
        }
        if (sector < 5.0F)
        {
            return {intermediate + match, match, chroma + match};
        }

        return {chroma + match, match, intermediate + match};
    }

    [[nodiscard]] hsl rgb_to_hsl(const rgb& rgb) noexcept
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
        return {hue, saturation, lightness};
    }

    [[nodiscard]] rgb hsl_to_rgb(const hsl& hsl) noexcept
    {
        const float hue = hsl.h - std::floor(hsl.h);
        const float chroma = (1.0F - std::fabs((2.0F * hsl.l) - 1.0F)) * hsl.s;
        const float sector = hue * 6.0F;
        const float intermediate = chroma * (1.0F - std::fabs(std::fmod(sector, 2.0F) - 1.0F));
        const float match = hsl.l - (chroma * 0.5F);

        if (sector < 1.0F)
        {
            return {chroma + match, intermediate + match, match};
        }
        if (sector < 2.0F)
        {
            return {intermediate + match, chroma + match, match};
        }
        if (sector < 3.0F)
        {
            return {match, chroma + match, intermediate + match};
        }
        if (sector < 4.0F)
        {
            return {match, intermediate + match, chroma + match};
        }
        if (sector < 5.0F)
        {
            return {intermediate + match, match, chroma + match};
        }

        return {chroma + match, match, intermediate + match};
    }

    [[nodiscard]] float srgb_to_rgb(float value) noexcept
    {
        return value <= 0.04045F ? value / 12.92F : std::pow((value + 0.055F) / 1.055F, 2.4F);
    }

    [[nodiscard]] float rgb_to_srgb(float value) noexcept
    {
        return value <= 0.0031308F ? value * 12.92F : (1.055F * std::pow(value, 1.0F / 2.4F)) - 0.055F;
    }

    [[nodiscard]] rgb srgb_to_rgb(const rgb& srgb) noexcept
    {
        return {srgb_to_rgb(srgb.r), srgb_to_rgb(srgb.g), srgb_to_rgb(srgb.b)};
    }

    [[nodiscard]] rgb rgb_to_srgb(const rgb& rgb) noexcept
    {
        return {rgb_to_srgb(rgb.r), rgb_to_srgb(rgb.g), rgb_to_srgb(rgb.b)};
    }

    [[nodiscard]] hsv srgb_to_hsv(const rgb& srgb) noexcept
    {
        return rgb_to_hsv(srgb_to_rgb(srgb));
    }

    [[nodiscard]] rgb hsv_to_srgb(const hsv& hsv) noexcept
    {
        return rgb_to_srgb(hsv_to_rgb(hsv));
    }

    [[nodiscard]] hsl srgb_to_hsl(const rgb& srgb) noexcept
    {
        return rgb_to_hsl(srgb_to_rgb(srgb));
    }

    [[nodiscard]] rgb hsl_to_srgb(const hsl& hsl) noexcept
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
