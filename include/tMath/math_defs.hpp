#pragma once

#include <cstddef> // for ptrdiff_t (这个头文件一定要包含，因为下面有些头文件可能用到了 ptrdiff_t ，不包含，可能会编译报错)
#include <cstdint>
#include <cassert>

#include <limits>
#include <concepts>
#include <type_traits>

#define TMATH_NAMESPACE_NAME tMath
#define TMATH_NAMESPACE_BEGIN namespace TMATH_NAMESPACE_NAME {
#define TMATH_NAMESPACE_END }


#define TMATH_VERSION_MAJOR 0
#define TMATH_VERSION_MINOR 0
#define TMATH_VERSION_PATCH 1

#define TMATH_MAKE_VERSION_COMPONENT(v) (v + 0ULL)

/**
 * @param major 63-48 (16bit)
 * @param minor 47-32 (16bit)
 * @param patch 31-0  (32bit)
 */
#define TMATH_MAKE_VERSION(major, minor, patch) \
    (   (((TMATH_MAKE_VERSION_COMPONENT((major))) & 0xffffULL) << 48ULL) | \
        (((TMATH_MAKE_VERSION_COMPONENT((minor))) & 0xffffULL) << 32ULL) | \
        ( (TMATH_MAKE_VERSION_COMPONENT((patch))) & 0xffffffffULL)   )

#define TMATH_VERSION ( TMATH_MAKE_VERSION(TMATH_VERSION_MAJOR, TMATH_VERSION_MINOR, TMATH_VERSION_PATCH) )

TMATH_NAMESPACE_BEGIN
namespace Version
{
    static constexpr uint64_t version = TMATH_VERSION;
    static constexpr uint64_t make_version(const uint16_t major, const uint16_t minor, const uint32_t patch) noexcept
    {
        return TMATH_MAKE_VERSION(major, minor, patch);
    }
    static constexpr uint16_t major = TMATH_VERSION_MAJOR;
    static constexpr uint16_t minor = TMATH_VERSION_MINOR;
    static constexpr uint32_t patch = TMATH_VERSION_PATCH;
}
TMATH_NAMESPACE_END


// force inline
#if defined (_MSC_VER)
    #define TMATH_FORCE_INLINE __forceinline
#elif defined (__GNUC__) || defined (__clang__)
    #define TMATH_FORCE_INLINE inline __attribute__((__always_inline__))
#else
    #error "TMATH_FORCE_INLINE has not defined!"
#endif

// consteval
#if defined (__GNUC__) || defined (__clang__)
    #define TMATH_CONSTEVAL_FN consteval
    #define TMATH_IF_CONSTEVAL if consteval
#else
    #define TMATH_CONSTEVAL_FN constexpr
    #define TMATH_IF_CONSTEVAL if (std::is_constant_evaluated())
#endif

// std constexpr floating point functions
#if defined (__GNUC__)
    #define TMATH_HAS_STD_CONSTEXPR_FLOATING_POINT_FUNCTIONS
#endif


#define TMATH_NODISCARD [[nodiscard]]
#define TMATH_CALL_CONV
#define TMATH_API

#define TMATH_ASSERT(...) assert(__VA_ARGS__)


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

// ----------------------------------------------------
// 任意数 -> 浮点数 (根据传入的整数的bit大小，自动判断需要用多少bit的浮点数)
// ----------------------------------------------------
template<is_number N>
struct number_to_floating_point
{
    // long double做特殊处理：只有输入类型是long double 才能传出 long double，否则最高只能到达 double
    using type = std::conditional_t<
        std::is_same_v<N, long double>,
        long double,
        std::conditional_t<
            (std::numeric_limits<N>::digits > std::numeric_limits<float>::digits),
            double,
            float
        >
    >;
};

template<is_number N>
using number_to_floating_point_t = number_to_floating_point<N>::type;


// 提取两个输入的浮点类型的size最大的类型
template<is_floating_point F1, is_floating_point F2>
using max_floating_point_t = std::conditional_t<(sizeof(F1) >= sizeof(F2)), F1, F2>;

// 提取两个输入的浮点类型的size最小的类型
template<is_floating_point F1, is_floating_point F2>
using min_floating_point_t = std::conditional_t<(sizeof(F1) <= sizeof(F2)), F1, F2>;


// 用于在 requires 排除某些 conditions(或concepts)
template<bool... Conditions>
static constexpr bool exclude = !(Conditions || ...);


namespace detail
{
    template<typename T>
    static constexpr bool is_pure_data_type_v =
        std::is_standard_layout_v<T> &&
        std::is_aggregate_v<T>;

    template<typename T>
    concept has_data_field = requires(T t)
    {
        t.data;
    };
}


// =============================================== Vector ===============================================

struct color_tag {};
#define TMATH_COLOR_TAG using tmath_type_tag = TMATH_NAMESPACE_NAME::color_tag;

struct quat_tag {};
#define TMATH_QUAT_TAG using tmath_type_tag = TMATH_NAMESPACE_NAME::quat_tag;

// ----------------------------------------------------
// VectorN 类型萃取
// ----------------------------------------------------
template<detail::has_data_field V>
struct vector_traits
{
    using component_type = std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<V>().data)>>;
    static constexpr int component_count = std::extent_v<decltype(std::declval<V>().data), 0>;
};

template<detail::has_data_field V>
using vector_component_t = vector_traits<V>::component_type;

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
    concept has_type_tag = requires
    {
        typename T::tmath_type_tag;
    };

    template<typename T>
    concept mark_as_color = requires
    {
        typename T::tmath_type_tag;
        requires std::is_same_v<typename T::tmath_type_tag, TMATH_NAMESPACE_NAME::color_tag>;
    };

    template<typename T>
    concept mark_as_quat = requires
    {
        typename T::tmath_type_tag;
        requires std::is_same_v<typename T::tmath_type_tag, TMATH_NAMESPACE_NAME::quat_tag>;
    };
    
    template<typename V>
    concept is_generic_vector = requires(V v)
    {
        v.data;
        requires std::is_same_v<std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<V>().data)>>, typename vector_traits<V>::component_type>;
        requires is_vector_n_layout_v<V, typename vector_traits<V>::component_type, vector_traits<V>::component_count>;
        requires std::rank_v<decltype(std::declval<V>().data)> == 1; // 数组维度必须是1
        requires is_signed_number<std::remove_all_extents_t<decltype(std::declval<V>().data)>>; // component的类型必须是有符号数字
    };
}

// ----------------------------------------------------
// VectorN<T>
// ----------------------------------------------------
template<typename T>
concept is_vector_n = detail::is_generic_vector<T> && !detail::has_type_tag<T>;

template<typename T>
concept is_vector_n_floating_point = is_vector_n<T> && is_floating_point<vector_component_t<T>>;

template<typename T>
concept is_vector_n_sint = is_vector_n<T> && is_signed_int<vector_component_t<T>>;

// ----------------------------------------------------
// Vector2<T>
// ----------------------------------------------------
template<typename T>
concept is_vector2 = is_vector_n<T> && (vector_traits<T>::component_count == 2);

template<typename T>
concept is_vector2_float = is_vector2<T> && std::is_same_v<vector_component_t<T>, float>;

template<typename T>
concept is_vector2_floating_point = is_vector2<T> && is_floating_point<vector_component_t<T>>;

template<typename T>
concept is_vector2_sint = is_vector2<T> && is_signed_int<vector_component_t<T>>;



// ----------------------------------------------------
// Vector3<T>
// ----------------------------------------------------
template<typename T>
concept is_vector3 = is_vector_n<T> && (vector_traits<T>::component_count == 3);

template<typename T>
concept is_vector3_float = is_vector3<T> && std::is_same_v<vector_component_t<T>, float>;

template<typename T>
concept is_vector3_floating_point = is_vector3<T> && is_floating_point<vector_component_t<T>>;

template<typename T>
concept is_vector3_sint = is_vector3<T> && is_signed_int<vector_component_t<T>>;



// ----------------------------------------------------
// Vector4<T>
// ----------------------------------------------------
template<typename T>
concept is_vector4 = is_vector_n<T> && (vector_traits<T>::component_count == 4) && !detail::mark_as_quat<T>;

template<typename T>
concept is_vector4_float = is_vector4<T> && std::is_same_v<vector_component_t<T>, float>;

template<typename T>
concept is_vector4_floating_point = is_vector4<T> && is_floating_point<vector_component_t<T>>;

template<typename T>
concept is_vector4_sint = is_vector4<T> && is_signed_int<vector_component_t<T>>;


// ----------------------------------------------------
// Color<T>: T is any type of number
// ----------------------------------------------------
template<typename C>
struct color_traits
{
    using component_type = std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<C>().data)>>;
    static constexpr int component_count = std::extent_v<decltype(std::declval<C>().data), 0>;

    static_assert(component_count == 3 || component_count == 4, "color's component count MUST be 3 or 4.");
};

template<typename C>
using color_component_t = color_traits<C>::component_type;

template<typename T>
concept is_color = detail::is_generic_vector<T> && (color_traits<T>::component_count == 3 || color_traits<T>::component_count == 4) && detail::mark_as_color<T>;

template<typename T>
concept is_color_floating_point = is_color<T> && is_floating_point<color_component_t<T>>;

template<typename T>
concept is_color_int = is_color<T> && is_int<color_component_t<T>>;



// ----------------------------------------------------
// Quat<T> T only floating point
// ----------------------------------------------------
template<typename Q>
struct quat_traits
{
    using component_type = std::remove_all_extents_t<std::remove_reference_t<decltype(std::declval<Q>().data)>>;
    static constexpr int component_count = std::extent_v<decltype(std::declval<Q>().data), 0>;

    static_assert(component_count == 4, "quat's component count MUST be 4.");
};

template<typename Q>
using quat_component_t = quat_traits<Q>::component_type;

template<typename T>
concept is_quat = detail::is_generic_vector<T> && (quat_traits<T>::component_count == 4) && detail::mark_as_quat<T>;


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
#define TMATH_MATRIX_COLUMN_MAJOR_TAG using tmath_matrix_major_tag = TMATH_NAMESPACE_NAME::column_major_matrix_tag;

namespace detail
{
    template<typename T>
    concept mark_as_column_major_matrix = requires
    {
        typename T::tmath_matrix_major_tag;
        requires std::is_same_v<typename T::tmath_matrix_major_tag, TMATH_NAMESPACE_NAME::column_major_matrix_tag>;
    };
}

// ----------------------------------------------------
// Matrix 类型萃取
// ----------------------------------------------------
template<detail::has_data_field Mat>
struct matrix_traits
{
    using vector_type = std::remove_all_extents_t<decltype(Mat::data)>;
    using component_type = vector_component_t<vector_type>;

    static constexpr bool is_column_major = detail::mark_as_column_major_matrix<Mat>;

    /**
     * 第0维数组的大小
     */
    static constexpr int vector_count = std::extent_v<decltype(Mat::data), 0>;

    /**
     * 第1维数组的大小 (内部向量长度)
     */
    static constexpr int vector_component_count = vector_traits<vector_type>::component_count;

    /**
     * 逻辑行数 (如果是列主序，则表示为存储结构的列数，但是逻辑上还是行数)
     */
    static constexpr int row_count = is_column_major ? vector_component_count : vector_count;

    /**
     * 逻辑列数 (如果是列主序，则表示为存储结构的行数，但是逻辑上还是列数)
     */
    static constexpr int column_count = is_column_major ? vector_count : vector_component_count;

    /**
     * 是否为方阵
     */
    static constexpr bool is_square_matrix = (row_count == column_count);

    // check
    static_assert(sizeof(component_type) * row_count * column_count == sizeof(Mat), "Matrix size mismatch!");
};

template<detail::has_data_field Mat>
using matrix_component_t = matrix_traits<Mat>::component_type;

namespace detail
{
    template<typename Mat, typename Component, int Rows, int Columns>
    static constexpr bool is_matrix_layout_v = is_pure_data_type_v<Mat> && (sizeof(Mat) == sizeof(Component) * Rows * Columns);

    template<typename Mat>
    concept is_generic_matrix = requires
    {
        typename std::remove_reference_t<decltype(std::declval<Mat>().data)>; // .data 字段
        requires std::is_array_v<decltype(std::declval<Mat>().data)>; // data字段是一个数组
        requires std::rank_v<decltype(std::declval<Mat>().data)> == 1; // .data数组维度必须是1
        requires std::extent_v<decltype(std::declval<Mat>().data), 0> >= 2; // 数组长度必须 >= 2 至少是 mat2x2
        requires is_vector_n<std::remove_all_extents_t<decltype(std::declval<Mat>().data)>>; // 每一行(列)是向量类型
        requires is_matrix_layout_v<Mat, matrix_component_t<Mat>, matrix_traits<Mat>::row_count, matrix_traits<Mat>::column_count>; // 内存布局符合要求
    };
}

template<typename Mat>
concept is_matrix_row_major = detail::is_generic_matrix<Mat> && !detail::mark_as_column_major_matrix<Mat>;

template<typename Mat>
concept is_matrix_column_major = detail::is_generic_matrix<Mat> && detail::mark_as_column_major_matrix<Mat>;

template<typename Mat>
concept is_matrix_any_major = is_matrix_row_major<Mat> || is_matrix_column_major<Mat>;

template<typename Mat>
concept is_square_matrix_row_major = is_matrix_row_major<Mat> && matrix_traits<Mat>::is_square_matrix;

template<typename Mat>
concept is_square_matrix_column_major = is_matrix_column_major<Mat> && matrix_traits<Mat>::is_square_matrix;

template<typename Mat>
concept is_square_matrix_any_major = is_matrix_any_major<Mat> && matrix_traits<Mat>::is_square_matrix;


TMATH_NAMESPACE_END
