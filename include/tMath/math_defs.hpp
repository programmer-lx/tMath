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


#define TMATH_REGISTER_VECTOR_TYPE(vector_type_name) \
    TMATH_FORCE_INLINE bool operator==(const vector_type_name& lhs, const vector_type_name& rhs) \
    { return TMATH_NAMESPACE_NAME::operator==(lhs, rhs); } \
    TMATH_FORCE_INLINE bool operator!=(const vector_type_name& lhs, const vector_type_name& rhs) \
    { return TMATH_NAMESPACE_NAME::operator!=(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name& operator+=(vector_type_name& lhs, const vector_type_name& rhs) \
    { return TMATH_NAMESPACE_NAME::operator+=(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name& operator-=(vector_type_name& lhs, const vector_type_name& rhs) \
    { return TMATH_NAMESPACE_NAME::operator-=(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name& operator*=(vector_type_name& lhs, const TMATH_NAMESPACE_NAME::vector_field_t<vector_type_name> rhs) \
    { return TMATH_NAMESPACE_NAME::operator*=(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name& operator/=(vector_type_name& lhs, const TMATH_NAMESPACE_NAME::vector_field_t<vector_type_name> rhs) \
    { return TMATH_NAMESPACE_NAME::operator/=(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name operator+(const vector_type_name& lhs, const vector_type_name& rhs) \
    { return TMATH_NAMESPACE_NAME::operator+(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name operator-(const vector_type_name& lhs, const vector_type_name& rhs) \
    { return TMATH_NAMESPACE_NAME::operator-(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name operator*(const vector_type_name& lhs, const TMATH_NAMESPACE_NAME::vector_field_t<vector_type_name> rhs) \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    TMATH_FORCE_INLINE vector_type_name operator/(const vector_type_name& lhs, const TMATH_NAMESPACE_NAME::vector_field_t<vector_type_name> rhs) \
    { return TMATH_NAMESPACE_NAME::operator/(lhs, rhs); }



TMATH_NAMESPACE_BEGIN

namespace detail
{
    struct quat_tag {};
}
/**
 * 由于没办法通过字段区分vector4和quat
 * 所以需要通过标记的方式进行区分，示例:
 * struct Quat
 * {
 *     float x = 0, y = 0, z = 0, w = 1;
 *     TMATH_MARK_AS_QUAT
 * }
 */
#define TMATH_MARK_AS_QUAT using is_quat = TMATH_NAMESPACE_NAME::detail::quat_tag;

using f32 = float;
static_assert(sizeof(f32) == 4);

using f64 = double;
static_assert(sizeof(f64) == 8);

using long_double = long double;

template<typename T>
concept is_signed_int = std::is_integral_v<T> && std::is_signed_v<T>;

template<typename T>
concept is_floating_point = std::is_floating_point_v<T>;

// 数学库只能计算符合这些约束条件的数字
template<typename T>
concept is_number = TMATH_NAMESPACE_NAME::is_signed_int<T> || TMATH_NAMESPACE_NAME::is_floating_point<T>;

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
        requires std::is_same_v<typename T::is_quat, TMATH_NAMESPACE_NAME::detail::quat_tag>;
    };

    template<typename T>
    concept is_f32 = std::is_same_v<T, f32>;

    template<typename T>
    concept is_f64 = std::is_same_v<T, f64>;

    template<typename T>
    concept is_long_double = std::is_same_v<T, long_double>;

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
concept is_vector2_f32 = detail::is_generic_vector2<T, f32>;
template<typename T>
concept is_vector2_f64 = detail::is_generic_vector2<T, f64>;
template<typename T>
concept is_vector2_long_double = detail::is_generic_vector2<T, long_double>;
template<typename T>
concept is_vector2_floating_point = is_vector2_f32<T> || is_vector2_f64<T> || is_vector2_long_double<T>;

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
concept is_vector3_f32 = detail::is_generic_vector3<T, f32>;
template<typename T>
concept is_vector3_f64 = detail::is_generic_vector3<T, f64>;
template<typename T>
concept is_vector3_long_double = detail::is_generic_vector3<T, long_double>;
template<typename T>
concept is_vector3_floating_point = is_vector3_f32<T> || is_vector3_f64<T> || is_vector3_long_double<T>;

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
concept is_vector4_f32 = detail::is_generic_vector4<T, f32>;
template<typename T>
concept is_vector4_f64 = detail::is_generic_vector4<T, f64>;
template<typename T>
concept is_vector4_long_double = detail::is_generic_vector4<T, long_double>;
template<typename T>
concept is_vector4_floating_point = is_vector4_f32<T> || is_vector4_f64<T> || is_vector4_long_double<T>;

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
concept is_quat_f32 = detail::is_generic_quat<T, f32>;
template<typename T>
concept is_quat_f64 = detail::is_generic_quat<T, f64>;
template<typename T>
concept is_quat_long_double = detail::is_generic_quat<T, long_double>;
template<typename T>
concept is_quat = is_quat_f32<T> || is_quat_f64<T> || is_quat_long_double<T>;




// ----------------------------------------------------
// VectorN<T>
// ----------------------------------------------------
template<typename T>
concept is_vector_n = is_vector2<T> || is_vector3<T> || is_vector4<T>;



// ----------------------------------------------------
// VectorN 字段类型萃取
// ----------------------------------------------------
template<is_vector_n V>
struct vector_traits
{
    using field_type = decltype(std::declval<V>().x);
};

template<is_vector_n V>
using vector_field_t = vector_traits<V>::field_type;




// ----------------------------------------------------
// 整数 -> 浮点数 (根据传入的整数的bit大小，自动判断需要用多少bit的浮点数)
// ----------------------------------------------------
template<is_signed_int I>
struct sint_to_floating_point
{
private:
    static constexpr bool requires_f64 = std::numeric_limits<I>::digits > std::numeric_limits<f32>::digits;
    static constexpr bool requires_long_double = std::numeric_limits<I>::digits > std::numeric_limits<f64>::digits;

public:
    using type = std::conditional_t<
        requires_f64,
        std::conditional_t<
            requires_long_double,
            long_double,
            f64
        >,
        f32
    >;
};

template<is_signed_int I>
using sint_to_floating_point_t = sint_to_floating_point<I>::type;


TMATH_NAMESPACE_END