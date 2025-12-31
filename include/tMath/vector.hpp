#pragma once

#include <utility>
#include <cmath>
#include <array>
#include <numeric>

#include "math_defs.hpp"
#include "impl/fwd_vector.hpp"
#include "number.hpp"
#include "impl/vector2.hpp"
#include "impl/vector3.hpp"
#include "impl/vector4.hpp"


TMATH_NAMESPACE_BEGIN

// ============================================= operators =============================================
template<is_vector_n VecN>
constexpr bool operator==(const VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        return ( (lhs.data[I] == rhs.data[I]) && ... );
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr bool operator!=(const VecN& lhs, const VecN& rhs) noexcept
{
    return !(lhs == rhs);
}

template<is_vector_n VecN>
constexpr VecN& operator+=(VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        ( (lhs.data[I] += rhs.data[I]), ... );
    }(std::make_index_sequence<N>{});

    return lhs;
}

template<is_vector_n VecN>
constexpr VecN operator+(const VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using value_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<value_t>(lhs.data[I] + rhs.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr VecN& operator-=(VecN& lhs, const VecN& rhs) noexcept
{
    const auto N = vector_traits<VecN>::component_count;

    [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        ( (lhs.data[I] -= rhs.data[I]), ... );
    }(std::make_index_sequence<N>{});

    return lhs;
}

template<is_vector_n VecN>
constexpr VecN operator-(const VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using value_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<value_t>(lhs.data[I] - rhs.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN, is_number Num>
constexpr VecN& operator*=(VecN& lhs, const Num scalar) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using value_t = vector_component_t<VecN>;

    [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        ( (lhs.data[I] *= static_cast<value_t>(scalar)), ... );
    }(std::make_index_sequence<N>{});

    return lhs;
}

template<is_vector_n VecN, is_number Num>
constexpr VecN operator*(const VecN& lhs, const Num scalar) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using value_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<value_t>(lhs.data[I] * static_cast<value_t>(scalar))...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN, is_number Num>
constexpr VecN& operator/=(VecN& lhs, const Num scalar) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using value_t = vector_component_t<VecN>;

    [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        ( (lhs.data[I] /= static_cast<value_t>(scalar)), ... );
    }(std::make_index_sequence<N>{});

    return lhs;
}

template<is_vector_n VecN, is_number Num>
constexpr VecN operator/(const VecN& lhs, const Num scalar) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using value_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<value_t>(lhs.data[I] / static_cast<value_t>(scalar))...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN, is_number Num>
void safe_divide_inplace(VecN& v, const Num divisor, const VecN& fallback) noexcept
{
    if (is_invalid_divisor(divisor))
    {
        v = fallback;
        return;
    }

    v /= divisor;
}

template<is_vector_n VecN, is_number Num>
VecN safe_divide(const VecN& v, const Num divisor, const VecN& fallback) noexcept
{
    VecN result = v;
    safe_divide_inplace(result, divisor, fallback);
    return result;
}



// ============================================= casts =============================================
template<is_vector_n Ret, is_vector_n In>
constexpr Ret vector_cast(const In& v) noexcept
{
    using comp_t = vector_component_t<Ret>;
    constexpr auto RetN = vector_traits<Ret>::component_count;
    constexpr auto InN = vector_traits<In>::component_count;

    // 截断/直接返回
    if constexpr (RetN <= InN)
    {
        return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> Ret
        {
            return {
                static_cast<comp_t>(v.data[I])...
            };
        }(std::make_index_sequence<RetN>{});
    }
    // Ret多出来的位置填充0
    else // RetN > InN
    {
        return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> Ret
        {
            return {
                (I < InN ? static_cast<comp_t>(v.data[I]) : static_cast<comp_t>(0))...
            };
        }(std::make_index_sequence<RetN>{});
    }
}



// ============================================= functions =============================================
template<is_vector_n_floating_point VecN>
constexpr VecN to_degrees(const VecN& radians) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            to_degrees(radians.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
constexpr VecN to_radians(const VecN& degrees) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            to_radians(degrees.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr VecN min(const VecN& a, const VecN& b) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            TMATH_NAMESPACE_NAME::min(a.data[I], b.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr VecN max(const VecN& a, const VecN& b) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            TMATH_NAMESPACE_NAME::max(a.data[I], b.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
VecN abs(const VecN& v) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            TMATH_NAMESPACE_NAME::abs(v.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr VecN cwise_mul(const VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;
    
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<comp_t>(lhs.data[I] * rhs.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr VecN cwise_div(const VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;
    
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<comp_t>(lhs.data[I] / rhs.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
VecN sin(const VecN& v) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<comp_t>(std::sin(v.data[I]))...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
VecN cos(const VecN& v) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<comp_t>(std::cos(v.data[I]))...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
VecN tan(const VecN& v) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<comp_t>(std::tan(v.data[I]))...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr vector_component_t<VecN> dot(const VecN& lhs, const VecN& rhs) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using ret_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> ret_t
    {
        return static_cast<ret_t>(( (lhs.data[I] * rhs.data[I]) + ... ));
    }(std::make_index_sequence<N>{});
}

template<is_number... N>
    requires exclude<sizeof...(N) == 4, sizeof...(N) == 6, sizeof...(N) == 8>
constexpr auto dot(const N... comp) noexcept
{
    // static_assert(sizeof...(N) != 4, "you can include vector2.hpp to use dot(x1, y1, x2, y2)");
    // static_assert(sizeof...(N) != 6, "you can include vector3.hpp to use dot(x1, y1, z1, x2, y2, z2)");
    // static_assert(sizeof...(N) != 8, "you can include vector4.hpp to use dot(x1, y1, z1, w1, x2, y2, z2, w2)");
    static_assert(sizeof...(N) % 2 == 0, "you must input two vector components like dot(x1, y1, z1  ,   x2, y2, z2)");

    using ret_t = std::common_type_t<number_to_floating_point_t<N>...>;
    constexpr size_t half = sizeof...(N) / 2;

    std::array<ret_t, sizeof...(N)> arr{ static_cast<ret_t>(comp)... };

    return std::transform_reduce(
        arr.begin(), arr.begin() + half,        // first vector
        arr.begin() + half,                     // second vector
        ret_t{0},                               // init
        std::plus{},                            // sum
        std::multiplies{}                       // multiply corresponding elements
    );
}

template<is_vector_n_floating_point VecN>
bool approximately_all(const VecN& a, const VecN& b, const vector_component_t<VecN> tolerance = Epsilon<vector_component_t<VecN>>) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> bool
    {
        return ( (approximately(a.data[I], b.data[I], tolerance)) && ... );
    }(std::make_index_sequence<N>{});
}

/**
 * @return 若该分量近似相等，则该分量全部bit置为1，否则被置为0
 */
template<is_vector_n_floating_point VecN>
VecN approximately_cwise(const VecN& a, const VecN& b, const vector_component_t<VecN> tolerance = Epsilon<vector_component_t<VecN>>) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            (approximately(a.data[I], b.data[I], tolerance) ? one_block<comp_t>() : static_cast<comp_t>(0))...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
vector_component_t<VecN> magnitude(const VecN& v) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    using comp_t = vector_component_t<VecN>;

    if constexpr (N <= 3)
    {
        return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> comp_t
        {
            return static_cast<comp_t>(std::hypot( v.data[I]... ));
        }(std::make_index_sequence<N>{});
    }
    else
    {
        return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> comp_t
        {
            return static_cast<comp_t>(std::sqrt( ((v.data[I] * v.data[I]) + ...) ));
        }(std::make_index_sequence<N>{});
    }
}

/**
 * 求N维向量的模长
 * @param comp 分量，按照 x, y, z, w, ... 的顺序输入
 */
template<is_number... Comp>
    requires (sizeof...(Comp) >= 2)
auto magnitude(const Comp... comp) noexcept
{
    using ret_t = std::common_type_t<number_to_floating_point_t<Comp>...>;

    if constexpr (sizeof...(Comp) <= 3)
    {
        return static_cast<ret_t>(std::hypot( static_cast<ret_t>(comp)... ));
    }
    else
    {
        return static_cast<ret_t>(std::sqrt( ((static_cast<ret_t>(comp) * static_cast<ret_t>(comp)) + ...) ));
    }
}

template<is_vector_n_floating_point VecN>
void normalize_inplace(VecN& v) noexcept
{
    using F = vector_component_t<VecN>;
    constexpr auto N = vector_traits<VecN>::component_count;

    const F inv_mag = static_cast<F>(1) / magnitude(v);

    [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        ( (v.data[I] *= inv_mag), ... );
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
void safe_normalize_inplace(VecN& v, const VecN& fallback) noexcept
{
    using F = vector_component_t<VecN>;
    constexpr auto N = vector_traits<VecN>::component_count;

    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        v = fallback;
        return;
    }

    const F inv_mag = static_cast<F>(1) / mag;
    [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        ( (v.data[I] *= inv_mag), ... );
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
VecN normalized(const VecN& v) noexcept
{
    using F = vector_component_t<VecN>;
    constexpr auto N = vector_traits<VecN>::component_count;

    const F inv_mag = static_cast<F>(1) / magnitude(v);

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            (v.data[I] * inv_mag)...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point RetVecN, is_vector_n_sint VecN_SInt>
RetVecN normalized(const VecN_SInt& v) noexcept
{
    static_assert(vector_traits<RetVecN>::component_count == vector_traits<VecN_SInt>::component_count, "two vector's component count MUST be equal.");

    using ret_comp_t = vector_component_t<RetVecN>;
    constexpr auto N = vector_traits<RetVecN>::component_count;

    const ret_comp_t inv_mag = static_cast<ret_comp_t>(1) / [&]<size_t... I>(std::index_sequence<I...>) constexpr -> ret_comp_t
    {
        return static_cast<ret_comp_t>(magnitude(v.data[I]...));
    }(std::make_index_sequence<N>{});

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> RetVecN
    {
        return {
            (static_cast<ret_comp_t>(v.data[I]) * inv_mag)...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
VecN safe_normalized(const VecN& v, const VecN& fallback) noexcept
{
    using F = vector_component_t<VecN>;
    constexpr auto N = vector_traits<VecN>::component_count;

    const F mag = magnitude(v);
    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const F inv_mag = static_cast<F>(1) / mag;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            static_cast<F>(v.data[I] * inv_mag)...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point RetVecN, is_vector_n_sint VecN_SInt>
RetVecN safe_normalized(const VecN_SInt& v, const RetVecN& fallback) noexcept
{
    static_assert(vector_traits<RetVecN>::component_count == vector_traits<VecN_SInt>::component_count, "two vector's component count MUST be equal.");

    using ret_comp_t = vector_component_t<RetVecN>;
    constexpr auto N = vector_traits<RetVecN>::component_count;

    const ret_comp_t mag = [&]<size_t... I>(std::index_sequence<I...>) constexpr -> ret_comp_t
    {
        return static_cast<ret_comp_t>(magnitude(v.data[I]...));
    }(std::make_index_sequence<N>{});

    if (is_invalid_divisor(mag))
    {
        return fallback;
    }

    const ret_comp_t inv_mag = static_cast<ret_comp_t>(1) / mag;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> RetVecN
    {
        return {
            static_cast<ret_comp_t>(v.data[I] * inv_mag)...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
vector_component_t<VecN> distance(const VecN& a, const VecN& b) noexcept
{
    return magnitude(a - b);
}

template<is_vector_n_sint VecN_SInt>
auto distance(const VecN_SInt& a, const VecN_SInt& b) noexcept
{
    using ret_t = number_to_floating_point_t<vector_component_t<VecN_SInt>>;
    constexpr auto N = vector_traits<VecN_SInt>::component_count;

    const VecN_SInt delta = a - b;
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> ret_t
    {
        return static_cast<ret_t>(magnitude(static_cast<ret_t>(delta.data[I])...));
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN>
constexpr VecN clamp(const VecN& v, const VecN& min, const VecN& max) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            clamp(v.data[I], min.data[I], max.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n_floating_point VecN, is_floating_point F>
constexpr VecN lerp(const VecN& a, const VecN& b, const F t) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            lerp(a.data[I], b.data[I], t)...
        };
    }(std::make_index_sequence<N>{});
}



// ...参数是 vector_type_name，考虑到有些向量类型是泛型类型，比如Vector<float, 4>，这个逗号会被认为是宏参数的分隔符
#define TMATH_GENERIC_VECTOR_OPERATORS(...) \
    friend constexpr inline bool operator==(const __VA_ARGS__& lhs, const __VA_ARGS__& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator==(lhs, rhs); } \
    \
    friend constexpr inline bool operator!=(const __VA_ARGS__& lhs, const __VA_ARGS__& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator!=(lhs, rhs); } \
    \
    friend constexpr inline __VA_ARGS__& operator+=(__VA_ARGS__& lhs, const __VA_ARGS__& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator+=(lhs, rhs); } \
    \
    friend constexpr inline __VA_ARGS__& operator-=(__VA_ARGS__& lhs, const __VA_ARGS__& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator-=(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number Num> \
    friend constexpr inline __VA_ARGS__& operator*=(__VA_ARGS__& lhs, const Num rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*=(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number Num> \
    friend constexpr inline __VA_ARGS__& operator/=(__VA_ARGS__& lhs, const Num rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator/=(lhs, rhs); } \
    \
    friend constexpr inline __VA_ARGS__ operator+(const __VA_ARGS__& lhs, const __VA_ARGS__& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator+(lhs, rhs); } \
    \
    friend constexpr inline __VA_ARGS__ operator-(const __VA_ARGS__& lhs, const __VA_ARGS__& rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator-(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number Num> \
    friend constexpr inline __VA_ARGS__ operator*(const __VA_ARGS__& lhs, const Num rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator*(lhs, rhs); } \
    \
    template<TMATH_NAMESPACE_NAME::is_signed_number Num> \
    friend constexpr inline __VA_ARGS__ operator/(const __VA_ARGS__& lhs, const Num rhs) noexcept \
    { return TMATH_NAMESPACE_NAME::operator/(lhs, rhs); }

#define TMATH_VECTOR_OPERATORS(vector_type_name) TMATH_GENERIC_VECTOR_OPERATORS(vector_type_name)


#define TMATH_FULL_VECTOR_N_BUILTIN(component_type_name, dimension) \
    component_type_name data[dimension]; \
    TMATH_VECTOR_DATA_INDEX

#define TMATH_FULL_VECTOR_N(vector_type_name, component_type_name, dimension) \
    TMATH_FULL_VECTOR_N_BUILTIN(component_type_name, dimension) \
    TMATH_GENERIC_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR2_BUILTIN(component_type_name) \
    union \
    { \
        component_type_name data[2]; \
        struct { component_type_name x, y; }; \
        struct { component_type_name r, g; }; \
        struct { component_type_name u, v; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX

#define TMATH_FULL_VECTOR2(vector_type_name, component_type_name) \
    TMATH_FULL_VECTOR2_BUILTIN(component_type_name) \
    TMATH_GENERIC_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR3_BUILTIN(component_type_name) \
    union \
    { \
        component_type_name data[3]; \
        struct { component_type_name x, y, z; }; \
        struct { component_type_name r, g, b; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX

#define TMATH_FULL_VECTOR3(vector_type_name, component_type_name) \
    TMATH_FULL_VECTOR3_BUILTIN(component_type_name) \
    TMATH_GENERIC_VECTOR_OPERATORS(vector_type_name)

#define TMATH_FULL_VECTOR4_BUILTIN(component_type_name) \
    union \
    { \
        component_type_name data[4]; \
        struct { component_type_name x, y, z, w; }; \
        struct { component_type_name r, g, b, a; }; \
    }; \
    TMATH_VECTOR_DATA_INDEX

#define TMATH_FULL_VECTOR4(vector_type_name, component_type_name) \
    TMATH_FULL_VECTOR4_BUILTIN(component_type_name) \
    TMATH_GENERIC_VECTOR_OPERATORS(vector_type_name)



// 预设向量类
template<is_signed_number ComponentType, int Dimension>
struct Vector
{
    TMATH_FULL_VECTOR_N_BUILTIN(ComponentType, Dimension)
};

template<is_signed_number ComponentType>
struct Vector4
{
    TMATH_FULL_VECTOR4_BUILTIN(ComponentType)
};

template<is_signed_number ComponentType>
struct Vector3
{
    TMATH_FULL_VECTOR3_BUILTIN(ComponentType)
};

template<is_signed_number ComponentType>
struct Vector2
{
    TMATH_FULL_VECTOR2_BUILTIN(ComponentType)
};

TMATH_NAMESPACE_END
