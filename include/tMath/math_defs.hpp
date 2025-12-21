#pragma once

#include <limits>
#include <concepts>
#include <type_traits>
#include <cstdint>

#define TMATH_NAMESPACE_NAME tMath
#define TMATH_NAMESPACE_BEGIN namespace TMATH_NAMESPACE_NAME {
#define TMATH_NAMESPACE_END }

// force inline
#if defined (_MSC_VER)
    #define TMATH_FORCE_INLINE __forceinline
#elif defined (__GNUC__) || defined (__clang__)
    #define TMATH_FORCE_INLINE inline __attribute__((__always_inline__))
#else
    #error "TMATH_FORCE_INLINE has not defined!"
#endif


#define TMATH_NODISCARD [[nodiscard]]


TMATH_NAMESPACE_BEGIN

struct quat_tag {};
#define TMATH_MARK_AS_QUAT using is_quat = TMATH_NAMESPACE_NAME::quat_tag;

#define TMATH_VECTOR_OPERATORS(vector_type_name, field_type_name) \
    TMATH_FORCE_INLINE friend constexpr bool operator==(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator==(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr bool operator!=(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator!=(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name& operator+=(vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator+=(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name& operator-=(vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator-=(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name& operator*=(vector_type_name& lhs, const field_type_name rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*=(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name& operator/=(vector_type_name& lhs, const field_type_name rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator/=(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name operator+(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator+(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name operator-(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator-(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name operator*(const vector_type_name& lhs, const field_type_name rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    TMATH_FORCE_INLINE friend constexpr vector_type_name operator/(const vector_type_name& lhs, const field_type_name rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator/(lhs, rhs); }

#define TMATH_VECTOR_INDEX(field_type_name, data_var_name) \
    field_type_name& operator[](int i) { return data_var_name[i]; } \
    const field_type_name& operator[](int i) const { return data_var_name[i]; }

#define TMATH_VECTOR2(vector_type_name, field_type_name) \
    union \
    { \
        struct { field_type_name x, y; }; \
        struct { field_type_name r, g; }; \
        struct { field_type_name u, v; }; \
        field_type_name data[2]; \
    }; \
    TMATH_VECTOR_INDEX(field_type_name, data) \
    TMATH_VECTOR_OPERATORS(vector_type_name, field_type_name)

#define TMATH_VECTOR3(vector_type_name, field_type_name) \
    union \
    { \
        struct { field_type_name x, y, z; }; \
        struct { field_type_name r, g, b; }; \
        field_type_name data[3]; \
    }; \
    TMATH_VECTOR_INDEX(field_type_name, data) \
    TMATH_VECTOR_OPERATORS(vector_type_name, field_type_name)

#define TMATH_VECTOR4(vector_type_name, field_type_name) \
    union \
    { \
        struct { field_type_name x, y, z, w; }; \
        struct { field_type_name r, g, b, a; }; \
        field_type_name data[4]; \
    }; \
    TMATH_VECTOR_INDEX(field_type_name, data) \
    TMATH_VECTOR_OPERATORS(vector_type_name, field_type_name)

#define TMATH_QUAT(quat_type_name, field_type_name) \
    TMATH_MARK_AS_QUAT \
    union \
    { \
        struct { field_type_name x, y, z, w; }; \
        field_type_name data[4]; \
    }; \
    TMATH_VECTOR_INDEX(field_type_name, data)



template<typename T>
concept is_signed_int = std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept is_unsigned_int = std::is_integral_v<T> && !std::is_signed_v<T>;

template<typename T>
concept is_int = std::is_integral_v<T>;

template<typename T>
concept is_floating_point = std::is_floating_point_v<T>;

// 数学库只能计算符合这些约束条件的数字
template<typename T>
concept is_signed_number = TMATH_NAMESPACE_NAME::is_signed_int<T> || TMATH_NAMESPACE_NAME::is_floating_point<T>;

template<typename T>
concept is_number = std::is_arithmetic_v<T>;

namespace detail
{
    /**
     * 约束原则：
     * 1. 保证VectorN只有x, y, z, w之类的变量 (使用sizeof进行限制，确保没有多余变量)
     * 2. 保证VectorN没有虚函数，是一个类C结构体
     * 3. 不允许自定义构造函数，要确保向量类能使用 `{...}` 构造
     * 4. 允许开发者自定义拷贝函数、移动函数 (如果开发者定义了这类函数，意味着该类型不是trivial类型，
     *    定义了拷贝函数、移动函数之后，该向量在C++标准上就不是trivial类型了，所以禁止使用memcpy等函数直接操作内存，只能使用运算符进行操作)
     */
    template<typename T>
    concept is_pure_data_type =
        std::is_standard_layout_v<T> &&
        std::is_aggregate_v<T>;

    template<typename T>
    concept has_z = requires(T v)
    {
        v.z;
    };

    template<typename T>
    concept has_w = requires(T v)
    {
        v.w;
    };

    template<typename T>
    concept has_quat_tag = requires
    {
        typename T::is_quat;
        requires std::is_same_v<typename T::is_quat, TMATH_NAMESPACE_NAME::quat_tag>;
    };

    template<typename TVec, typename TField>
    concept is_generic_vector2 = requires(TVec v)
    {
        v.x;
        v.y;
        requires std::is_same_v<decltype(v.x), TField>;
        requires std::is_same_v<decltype(v.y), TField>;

        requires is_pure_data_type<TVec>;
    } && !has_z<TVec> && !has_w<TVec> && (sizeof(TVec) == sizeof(TField) * 2);

    template<typename TVec, typename TField>
    concept is_generic_vector3 = requires(TVec v)
    {
        v.x;
        v.y;
        v.z;
        requires std::is_same_v<decltype(v.x), TField>;
        requires std::is_same_v<decltype(v.y), TField>;
        requires std::is_same_v<decltype(v.z), TField>;

        requires is_pure_data_type<TVec>;
    } && !has_w<TVec> && (sizeof(TVec) == sizeof(TField) * 3);

    template<typename T, typename TField>
    concept is_vector4_or_quat = requires(T v)
    {
        v.x;
        v.y;
        v.z;
        v.w;
        requires std::is_same_v<decltype(v.x), TField>;
        requires std::is_same_v<decltype(v.y), TField>;
        requires std::is_same_v<decltype(v.z), TField>;
        requires std::is_same_v<decltype(v.w), TField>;

        requires is_pure_data_type<T>;
    } && (sizeof(T) == sizeof(TField) * 4);

    template<typename TVec, typename TField>
    concept is_generic_vector4 = is_vector4_or_quat<TVec, TField> && !has_quat_tag<TVec>;

    template<typename TQuat, typename TField>
    concept is_generic_quat = is_vector4_or_quat<TQuat, TField> && has_quat_tag<TQuat>;
}




// ----------------------------------------------------
// Vector2<T>
// ----------------------------------------------------
template<typename T>
concept is_vector2_float = detail::is_generic_vector2<T, float>;
template<typename T>
concept is_vector2_double = detail::is_generic_vector2<T, double>;
template<typename T>
concept is_vector2_long_double = detail::is_generic_vector2<T, long double>;
template<typename T>
concept is_vector2_floating_point = is_vector2_float<T> || is_vector2_double<T> || is_vector2_long_double<T>;

template<typename T>
concept is_vector2_i8 = detail::is_generic_vector2<T, int8_t>;
template<typename T>
concept is_vector2_i16 = detail::is_generic_vector2<T, int16_t>;
template<typename T>
concept is_vector2_i32 = detail::is_generic_vector2<T, int32_t>;
template<typename T>
concept is_vector2_i64 = detail::is_generic_vector2<T, int64_t>;
template<typename T>
concept is_vector2_sint = is_vector2_i8<T> || is_vector2_i16<T> || is_vector2_i32<T> || is_vector2_i64<T>;

template<typename T>
concept is_vector2 = is_vector2_floating_point<T> || is_vector2_sint<T>;



// ----------------------------------------------------
// Vector3<T>
// ----------------------------------------------------
template<typename T>
concept is_vector3_float = detail::is_generic_vector3<T, float>;
template<typename T>
concept is_vector3_double = detail::is_generic_vector3<T, double>;
template<typename T>
concept is_vector3_long_double = detail::is_generic_vector3<T, long double>;
template<typename T>
concept is_vector3_floating_point = is_vector3_float<T> || is_vector3_double<T> || is_vector3_long_double<T>;

template<typename T>
concept is_vector3_i8 = detail::is_generic_vector3<T, int8_t>;
template<typename T>
concept is_vector3_i16 = detail::is_generic_vector3<T, int16_t>;
template<typename T>
concept is_vector3_i32 = detail::is_generic_vector3<T, int32_t>;
template<typename T>
concept is_vector3_i64 = detail::is_generic_vector3<T, int64_t>;
template<typename T>
concept is_vector3_sint = is_vector3_i8<T> || is_vector3_i16<T> || is_vector3_i32<T> || is_vector3_i64<T>;

template<typename T>
concept is_vector3 = is_vector3_floating_point<T> || is_vector3_sint<T>;



// ----------------------------------------------------
// Vector4<T>
// ----------------------------------------------------
template<typename T>
concept is_vector4_float = detail::is_generic_vector4<T, float>;
template<typename T>
concept is_vector4_double = detail::is_generic_vector4<T, double>;
template<typename T>
concept is_vector4_long_double = detail::is_generic_vector4<T, long double>;
template<typename T>
concept is_vector4_floating_point = is_vector4_float<T> || is_vector4_double<T> || is_vector4_long_double<T>;

template<typename T>
concept is_vector4_i8 = detail::is_generic_vector4<T, int8_t>;
template<typename T>
concept is_vector4_i16 = detail::is_generic_vector4<T, int16_t>;
template<typename T>
concept is_vector4_i32 = detail::is_generic_vector4<T, int32_t>;
template<typename T>
concept is_vector4_i64 = detail::is_generic_vector4<T, int64_t>;
template<typename T>
concept is_vector4_sint = is_vector4_i8<T> || is_vector4_i16<T> || is_vector4_i32<T> || is_vector4_i64<T>;

template<typename T>
concept is_vector4 = is_vector4_floating_point<T> || is_vector4_sint<T>;

// ----------------------------------------------------
// Quat<T> T only floating point
// ----------------------------------------------------
template<typename T>
concept is_quat_float = detail::is_generic_quat<T, float>;
template<typename T>
concept is_quat_double = detail::is_generic_quat<T, double>;
template<typename T>
concept is_quat_long_double = detail::is_generic_quat<T, long double>;
template<typename T>
concept is_quat = is_quat_float<T> || is_quat_double<T> || is_quat_long_double<T>;




// ----------------------------------------------------
// VectorN<T>
// ----------------------------------------------------
template<typename T>
concept is_vector_n = is_vector2<T> || is_vector3<T> || is_vector4<T>;


template<typename T>
concept is_vector_n_or_quat = is_vector_n<T> || is_quat<T>;


// ----------------------------------------------------
// VectorN Quat 字段类型萃取
// ----------------------------------------------------
template<is_vector_n_or_quat V>
struct vector_quat_traits
{
    using field_type = decltype(std::declval<V>().x);
};

template<is_vector_n_or_quat V>
using vector_quat_field_t = vector_quat_traits<V>::field_type;




// ----------------------------------------------------
// 整数 -> 浮点数 (根据传入的整数的bit大小，自动判断需要用多少bit的浮点数)
// ----------------------------------------------------
template<is_signed_int I>
struct sint_to_floating_point
{
private:
    static constexpr bool requires_double = std::numeric_limits<I>::digits > std::numeric_limits<float>::digits;
    static constexpr bool requires_long_double = std::numeric_limits<I>::digits > std::numeric_limits<double>::digits;

public:
    using type = std::conditional_t<
        requires_double,
        std::conditional_t<
            requires_long_double,
            long double,
            double
        >,
        float
    >;
};

template<is_signed_int I>
using sint_to_floating_point_t = sint_to_floating_point<I>::type;


// 提取两个输入的浮点类型的size最大的类型
template<is_floating_point F1, is_floating_point F2>
using max_floating_point_t = std::conditional_t<(sizeof(F1) >= sizeof(F2)), F1, F2>;

// 提取两个输入的浮点类型的size最小的类型
template<is_floating_point F1, is_floating_point F2>
using min_floating_point_t = std::conditional_t<(sizeof(F1) <= sizeof(F2)), F1, F2>;

template<is_vector_n V1, is_vector_n V2>
using max_field_floating_point_t = std::conditional_t<(sizeof(vector_quat_field_t<V1>) >= sizeof(vector_quat_field_t<V2>)), vector_quat_field_t<V1>, vector_quat_field_t<V2>>;

template<is_vector_n V1, is_vector_n V2>
using min_field_floating_point_t = std::conditional_t<(sizeof(vector_quat_field_t<V1>) <= sizeof(vector_quat_field_t<V2>)), vector_quat_field_t<V1>, vector_quat_field_t<V2>>;



// initializers
template<auto... Values>
struct vector_values
{
    template<typename T>
    constexpr operator T() const noexcept
    {
        return T{ Values... };
    }
};


TMATH_NAMESPACE_END