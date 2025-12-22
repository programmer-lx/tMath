#pragma once

#include <format>
#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <gtest/gtest.h>

#if !defined(TMATH_IS_TESTING)
#error "please define TMATH_IS_TESTING macro to enable testing."
#endif

template<std::floating_point F>
F random_f(F min, F max)
{
    static std::mt19937_64 rng{ std::random_device{}() };
    std::uniform_real_distribution<F> dist(min, max);
    return dist(rng);
}

class ScopeTimer
{
public:
    using Clock = std::chrono::high_resolution_clock;

    ScopeTimer()
        : m_start(Clock::now())
    {
    }

    double time_millis() const
    {
        auto end = Clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - m_start;
        return elapsed.count();
    }

private:
    Clock::time_point m_start;
};