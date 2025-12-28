#pragma once

#include <cstddef> // for ptrdiff_t (这个头文件一定要包含，因为下面有些头文件可能用到了 ptrdiff_t ，不包含，可能会编译报错)
#include <cstdint>

#include <limits>
#include <concepts>
#include <type_traits>

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
    template<typename T>
    static constexpr bool is_pure_data_type_v =
        std::is_standard_layout_v<T> &&
        std::is_aggregate_v<T>;
}


// =============================================== Vector ===============================================

struct quat_tag {};
#define TMATH_QUAT_TAG using is_quat = TMATH_NAMESPACE_NAME::quat_tag;

/**
 * 约束原则：
 * 1. 保证VectorN只有x, y, z, w之类的变量 (使用sizeof进行限制，确保没有多余变量)
 * 2. 保证VectorN没有虚函数，是一个类C结构体
 * 3. 不允许自定义构造函数，要确保向量类能使用 `{...}` 构造
 * 4. 允许开发者自定义拷贝函数、移动函数 (如果开发者定义了这类函数，意味着该类型不是trivial类型，
 *    定义了拷贝函数、移动函数之后，该向量在C++标准上就不是trivial类型了，所以禁止使用memcpy等函数直接操作内存，只能使用运算符进行操作)
 */

namespace detail
{
    template<typename T, typename Component, int N>
    static constexpr bool is_vector_n_layout_v = is_pure_data_type_v<T> && (sizeof(T) == N * sizeof(Component));

    template<typename T>
    concept has_quat_tag = requires
    {
        typename T::is_quat;
        requires std::is_same_v<typename T::is_quat, TMATH_NAMESPACE_NAME::quat_tag>;
    };

    template<typename TVec, typename TComponent>
    concept is_generic_vector2 = requires(TVec v)
    {
        v.data;
        requires std::is_same_v<std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<TVec>().data)>>, TComponent>;

    } && is_vector_n_layout_v<TVec, TComponent, 2>;

    template<typename TVec, typename TComponent>
    concept is_generic_vector3 = requires(TVec v)
    {
        v.data;
        requires std::is_same_v<std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<TVec>().data)>>, TComponent>;

    } && is_vector_n_layout_v<TVec, TComponent, 3>;

    template<typename T, typename TComponent>
    concept is_vector4_or_quat = requires(T v)
    {
        v.data;
        requires std::is_same_v<std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<T>().data)>>, TComponent>;

    } && is_vector_n_layout_v<T, TComponent, 4>;

    template<typename TVec, typename TComponent>
    concept is_generic_vector4 = is_vector4_or_quat<TVec, TComponent> && !has_quat_tag<TVec>;

    template<typename TQuat, typename TComponent>
    concept is_generic_quat = is_vector4_or_quat<TQuat, TComponent> && has_quat_tag<TQuat>;
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
    using component_type = std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<V>().data)>>;
};

template<is_vector_n_or_quat V>
using vector_quat_component_t = vector_quat_traits<V>::component_type;


#define TMATH_VECTOR_OPERATORS(vector_type_name) \
    friend constexpr bool operator==(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator==(lhs, rhs); } \
    \
    friend constexpr bool operator!=(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator!=(lhs, rhs); } \
    \
    friend constexpr vector_type_name& operator+=(vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator+=(lhs, rhs); } \
    \
    friend constexpr vector_type_name& operator-=(vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator-=(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number N> \
    friend constexpr vector_type_name& operator*=(vector_type_name& lhs, const N rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*=(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number N> \
    friend constexpr vector_type_name& operator/=(vector_type_name& lhs, const N rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator/=(lhs, rhs); } \
    \
    friend constexpr vector_type_name operator+(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator+(lhs, rhs); } \
    \
    friend constexpr vector_type_name operator-(const vector_type_name& lhs, const vector_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator-(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number N> \
    friend constexpr vector_type_name operator*(const vector_type_name& lhs, const N rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number N> \
    friend constexpr vector_type_name operator/(const vector_type_name& lhs, const N rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator/(lhs, rhs); }


#define TMATH_VECTOR_DATA_INDEX(data_var_name) \
    std::remove_reference_t<decltype(data_var_name[0])>& operator[](int i) { return data_var_name[i]; } \
    const std::remove_reference_t<decltype(data_var_name[0])>& operator[](int i) const { return data_var_name[i]; }


#define TMATH_FULL_VECTOR2(vector_type_name, component_type_name) \
    union \
    { \
        component_type_name data[2]; \
        struct { component_type_name x, y; }; \
        struct { component_type_name r, g; }; \
        struct { component_type_name u, v; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX(data) \
    TMATH_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR3(vector_type_name, component_type_name) \
    union \
    { \
        component_type_name data[3]; \
        struct { component_type_name x, y, z; }; \
        struct { component_type_name r, g, b; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX(data) \
    TMATH_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR4(vector_type_name, component_type_name) \
    union \
    { \
        component_type_name data[4]; \
        struct { component_type_name x, y, z, w; }; \
        struct { component_type_name r, g, b, a; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX(data) \
    TMATH_VECTOR_OPERATORS(vector_type_name)




#define TMATH_QUAT_OPERATORS()

#define TMATH_FULL_QUAT(quat_type_name, component_type_name) \
    TMATH_QUAT_TAG \
    union \
    { \
        component_type_name data[4]; \
        struct { component_type_name x, y, z, w; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX(data)





// =============================================== Matrix ===============================================

/*
 * 矩阵属性:
 * 1. 是否为方阵
 * 2. 矩阵component数据类型，用于判断：位宽、有无符号、有无小数点
 *
 * 函数模板使用的concept:
 * concept is_square_matrix<Mat>, 类型和行数则在函数内部萃取，使用 if constexpr 做分支
 * concept is_non_square_matrix<Mat>, 类型和行列数在函数内部萃取，使用 if constexpr 做分支
 */

// 列主序标签
struct column_major_matrix_tag {};
#define TMATH_MATRIX_COLUMN_MAJOR_TAG using is_column_major = TMATH_NAMESPACE_NAME::column_major_matrix_tag;

namespace detail
{
    template<typename T>
    concept has_column_major_matrix_tag = requires
    {
        typename T::is_column_major;
        requires std::is_same_v<typename T::is_column_major, TMATH_NAMESPACE_NAME::column_major_matrix_tag>;
    };
}

// ----------------------------------------------------
// Matrix 类型萃取
// ----------------------------------------------------
template<typename Mat>
struct matrix_traits
{
    using component_type = std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<Mat>().data)>>;

    static constexpr bool is_column_major = detail::has_column_major_matrix_tag<Mat>;

    /**
     * 逻辑行数 (如果是列主序，则表示为存储结构的列数，但是逻辑上还是行数)
     */
    static constexpr int row_count = is_column_major ? std::extent_v<decltype(std::declval<Mat>().data), 1> : std::extent_v<decltype(std::declval<Mat>().data), 0>;

    /**
     * 逻辑列数 (如果是列主序，则表示为存储结构的行数，但是逻辑上还是列数)
     */
    static constexpr int column_count = is_column_major ? std::extent_v<decltype(std::declval<Mat>().data), 0> : std::extent_v<decltype(std::declval<Mat>().data), 1>;

    /**
     * 是否为方阵
     */
    static constexpr bool is_square_matrix = (row_count == column_count);
};

template<typename Mat>
using matrix_component_t = matrix_traits<Mat>::component_type;

namespace detail
{
    template<typename Mat>
    concept is_generic_matrix = requires
    {
        typename std::remove_reference_t<decltype(std::declval<Mat>().data)>; // .data 字段
    }
    && detail::is_pure_data_type_v<Mat> // 聚合初始化
    && std::is_array_v<decltype(std::declval<Mat>().data)>
    && (std::extent_v<decltype(std::declval<Mat>().data), 0> >= 2)
    && (std::extent_v<decltype(std::declval<Mat>().data), 1> >= 2) // 2D数组
    && (sizeof(matrix_component_t<Mat>) * (matrix_traits<Mat>::column_count * matrix_traits<Mat>::row_count) == sizeof(Mat)) // 大小符合要求
    && std::is_same_v<std::remove_all_extents_t<decltype(std::declval<Mat>().data)>, matrix_component_t<Mat>>;
}

template<typename Mat>
concept is_square_matrix_row_major = detail::is_generic_matrix<Mat> && !detail::has_column_major_matrix_tag<Mat>;

template<typename Mat>
concept is_square_matrix_column_major = detail::is_generic_matrix<Mat> && detail::has_column_major_matrix_tag<Mat>;



#define TMATH_MATRIX_OPERATORS(matrix_type_name)

// 无论是行主序还是列主序m01代表逻辑上第0行第1列，主序只是存储结构有变化，但是逻辑结构始终不变
#define TMATH_FULL_MATRIX4X4_ROW_MAJOR(matrix_type_name, component_type_name) \
    union \
    { \
        component_type_name data[4][4]; \
        struct \
        { \
            component_type_name m00, m01, m02, m03, \
                                m10, m11, m12, m13, \
                                m20, m21, m22, m23, \
                                m30, m31, m32, m33; \
        }; \
    }; \
    TMATH_MATRIX_OPERATORS(matrix_type_name)


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
using max_component_floating_point_t = std::conditional_t<(sizeof(vector_quat_component_t<V1>) >= sizeof(vector_quat_component_t<V2>)), vector_quat_component_t<V1>, vector_quat_component_t<V2>>;

template<is_vector_n V1, is_vector_n V2>
using min_component_floating_point_t = std::conditional_t<(sizeof(vector_quat_component_t<V1>) <= sizeof(vector_quat_component_t<V2>)), vector_quat_component_t<V1>, vector_quat_component_t<V2>>;


TMATH_NAMESPACE_END
