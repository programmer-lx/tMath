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

/**
 * @param major 63-48 (16bit)
 * @param minor 47-32 (16bit)
 * @param patch 31-0  (32bit)
 */
#define TMATH_MAKE_VERSION(major, minor, patch) \
    (   (((major) & 0xffffull) << 48ull) | \
        (((minor) & 0xffffull) << 32ull) | \
        ( (patch) & 0xffffffffull)   )

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


#define TMATH_NODISCARD [[nodiscard]]

#if defined(TMATH_IS_TESTING)
    #define TMATH_TEST_ASSERT(...) assert(__VA_ARGS__)
#else
    #define TMATH_TEST_ASSERT(...)
#endif


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

// 用于在 requires 排除某些 conditions(或concepts)
template<bool... Conditions>
static constexpr bool exclude = !(Conditions || ...);


template<bool... Conditions>
static constexpr bool match_all = (Conditions && ...);


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

struct quat_tag {};
#define TMATH_QUAT_TAG using is_quat = TMATH_NAMESPACE_NAME::quat_tag;

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
    concept has_quat_tag = requires
    {
        typename T::is_quat;
        requires std::is_same_v<typename T::is_quat, TMATH_NAMESPACE_NAME::quat_tag>;
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
// Vector2<T>
// ----------------------------------------------------
template<typename T>
concept is_vector2 = detail::is_generic_vector<T> && (vector_traits<T>::component_count == 2);

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
concept is_vector3 = detail::is_generic_vector<T> && (vector_traits<T>::component_count == 3);

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
concept is_vector4 = detail::is_generic_vector<T> && (vector_traits<T>::component_count == 4) && !detail::has_quat_tag<T>;

template<typename T>
concept is_vector4_float = is_vector4<T> && std::is_same_v<vector_component_t<T>, float>;

template<typename T>
concept is_vector4_floating_point = is_vector4<T> && is_floating_point<vector_component_t<T>>;

template<typename T>
concept is_vector4_sint = is_vector4<T> && is_signed_int<vector_component_t<T>>;


// ----------------------------------------------------
// VectorN<T>
// ----------------------------------------------------
template<typename T>
concept is_vector_n = detail::is_generic_vector<T>;

template<typename T>
concept is_vector_n_floating_point = is_vector_n<T> && is_floating_point<vector_component_t<T>>;

template<typename T>
concept is_vector_n_sint = is_vector_n<T> && is_signed_int<vector_component_t<T>>;


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


#define TMATH_VECTOR_DATA_INDEX \
    constexpr std::remove_extent_t<decltype(data)>& operator[](int i) { return data[i]; } \
    constexpr const std::remove_extent_t<decltype(data)>& operator[](int i) const { return data[i]; }


#define TMATH_FULL_VECTOR2(vector_type_name, component_type_name) \
    union \
    { \
        component_type_name data[2]; \
        struct { component_type_name x, y; }; \
        struct { component_type_name r, g; }; \
        struct { component_type_name u, v; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX \
    TMATH_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR3(vector_type_name, component_type_name) \
    union \
    { \
        component_type_name data[3]; \
        struct { component_type_name x, y, z; }; \
        struct { component_type_name r, g, b; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX \
    TMATH_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR4(vector_type_name, component_type_name) \
    union \
    { \
        component_type_name data[4]; \
        struct { component_type_name x, y, z, w; }; \
        struct { component_type_name r, g, b, a; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX \
    TMATH_VECTOR_OPERATORS(vector_type_name)




// ----------------------------------------------------
// Quat<T> T only floating point
// ----------------------------------------------------

// ----------------------------------------------------
// Quat 字段类型萃取
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
concept is_quat = detail::is_generic_vector<T> && (quat_traits<T>::component_count == 4) && detail::has_quat_tag<T>;


#define TMATH_QUAT_OPERATORS()

#define TMATH_FULL_QUAT(quat_type_name, component_type_name) \
    TMATH_QUAT_TAG \
    union \
    { \
        component_type_name data[4]; \
        struct { component_type_name x, y, z, w; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX





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
template<detail::has_data_field Mat>
struct matrix_traits
{
    using vector_type = std::remove_all_extents_t<decltype(Mat::data)>;
    using component_type = vector_component_t<vector_type>;

    static constexpr bool is_column_major = detail::has_column_major_matrix_tag<Mat>;

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
concept is_matrix_row_major = detail::is_generic_matrix<Mat> && !detail::has_column_major_matrix_tag<Mat>;

template<typename Mat>
concept is_matrix_column_major = detail::is_generic_matrix<Mat> && detail::has_column_major_matrix_tag<Mat>;

template<typename Mat>
concept is_matrix_any_major = is_matrix_row_major<Mat> || is_matrix_column_major<Mat>;

template<typename Mat>
concept is_square_matrix_row_major = is_matrix_row_major<Mat> && matrix_traits<Mat>::is_square_matrix;

template<typename Mat>
concept is_square_matrix_column_major = is_matrix_column_major<Mat> && matrix_traits<Mat>::is_square_matrix;

template<typename Mat>
concept is_square_matrix_any_major = is_matrix_any_major<Mat> && matrix_traits<Mat>::is_square_matrix;



#define TMATH_MATRIX_OPERATORS(matrix_type_name) \
    template<TMATH_NAMESPACE_NAME::is_vector_n Vec> \
    constexpr friend Vec operator*(const matrix_type_name& lhs, const Vec& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    \
    constexpr friend matrix_type_name operator*(const matrix_type_name& lhs, const matrix_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    \
    constexpr friend bool operator==(const matrix_type_name& lhs, const matrix_type_name& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator==(lhs, rhs); }


#define TMATH_FULL_MATRIX4X4_ROW_MAJOR(matrix_type_name, component_type_name)


// ----------------------------------------------------
// 任意数 -> 浮点数 (根据传入的整数的bit大小，自动判断需要用多少bit的浮点数)
// ----------------------------------------------------
template<is_number N>
struct number_to_floating_point
{
    using type = std::conditional_t<
        (std::numeric_limits<N>::digits > std::numeric_limits<float>::digits),
        double,
        float
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

template<is_vector_n V1, is_vector_n V2>
using max_component_floating_point_t = std::conditional_t<(sizeof(vector_component_t<V1>) >= sizeof(vector_component_t<V2>)), vector_component_t<V1>, vector_component_t<V2>>;

template<is_vector_n V1, is_vector_n V2>
using min_component_floating_point_t = std::conditional_t<(sizeof(vector_component_t<V1>) <= sizeof(vector_component_t<V2>)), vector_component_t<V1>, vector_component_t<V2>>;


TMATH_NAMESPACE_END
