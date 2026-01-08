#pragma once

#include <cstddef>
#include <cstdint>
#include <cstdlib>

#include <format>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include <gtest/gtest.h>

#if !defined(TMATH_IS_TESTING) && !defined(TSIMD_IS_TESTING)
#error "please define TMATH_IS_TESTING or TSIMD_IS_TESTING macro to enable testing."
#endif

#define TMATH_EXPECT_IS_NAN(val) EXPECT_TRUE(std::isnan(val))
#define TMATH_EXPECT_IS_POSITIVE(val) EXPECT_TRUE(!std::signbit(val))
#define TMATH_EXPECT_IS_NEGATIVE(val) EXPECT_TRUE(std::signbit(val))

template<std::floating_point F>
F random_f(F min, F max)
{
    static std::mt19937_64 rng{ static_cast<unsigned long>(std::time(nullptr)) };
    std::uniform_real_distribution<F> dist(min, max);
    return dist(rng);
}

class ScopeTimer
{
public:
    using Clock = std::chrono::high_resolution_clock;

    explicit ScopeTimer(std::string name = "ScopeTimer")
        : m_name(std::move(name)), m_start(Clock::now())
    {
    }

    // 析构函数：对象生命周期结束时自动触发
    ~ScopeTimer()
    {
        double elapsed = time_millis();
        std::cout << "[" << m_name << "] Elapsed time: "
                  << elapsed << " ms" << std::endl;
    }

    double time_millis() const
    {
        auto end = Clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - m_start;
        return elapsed.count();
    }

private:
    std::string m_name;
    Clock::time_point m_start;
};

inline void* my_aligned_alloc(size_t size, size_t alignment) {
    void* ptr = nullptr;
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    // Windows 平台
    ptr = _aligned_malloc(size, alignment);
#else
    // POSIX 平台 (GCC/Clang)
    if (posix_memalign(&ptr, alignment, size) != 0)
    {
        return nullptr;
    }
#endif
    return ptr;
}

inline void my_aligned_free(void* ptr) {
#if defined(_MSC_VER) || defined(__MINGW32__)
    _aligned_free(ptr);
#else
    std::free(ptr);
#endif
}