#pragma once

#include <utility>
#include <cmath>
#include <array>
#include <numeric>

#include "math_defs.hpp"
#include "fwd.hpp"
#include "floating_point.hpp"

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
    static_assert(vector_traits<Ret>::component_count == vector_traits<In>::component_count, "The component count of two vector MUST be equal.");

    using comp_t = vector_component_t<Ret>;
    constexpr auto N = vector_traits<Ret>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> Ret
    {
        return {
            static_cast<comp_t>(v.data[I])...
        };
    }(std::make_index_sequence<N>{});
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
VecN abs(const VecN& v) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;
    
    return [&]<size_t... I>(std::index_sequence<I...>) constexpr -> VecN
    {
        return {
            std::abs(v.data[I])...
        };
    }(std::make_index_sequence<N>{});
}

template<is_vector_n VecN>
constexpr VecN hadamard_mul(const VecN& lhs, const VecN& rhs) noexcept
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
constexpr VecN hadamard_div(const VecN& lhs, const VecN& rhs) noexcept
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
    requires exclude<sizeof...(N) == 4, sizeof...(N) == 6, sizeof...(N) == 8> // 已经给vector2, 3, 4特化，include "vector2.hpp" to use dot(x1, y1, x2, y2), ...
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
bool approximately(const VecN& a, const VecN& b, const vector_component_t<VecN> tolerance = MinTolerance<vector_component_t<VecN>>) noexcept
{
    constexpr auto N = vector_traits<VecN>::component_count;

    return [&]<size_t... I>(std::index_sequence<I...>) constexpr
    {
        return ( (approximately(a.data[I], b.data[I], tolerance)) && ... );
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
template<typename... Comp>
    requires (sizeof...(Comp) >= 2 && match_all<is_number<Comp>...>)
auto magnitude(const Comp... comp) noexcept
{
    using ret_t = number_to_floating_point_t<std::common_type_t<Comp...>>;

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

TMATH_NAMESPACE_END
