#pragma once

#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>
#include <random>
#include <chrono>

#include <gtest/gtest.h>


#define TMATH_TEST_MAIN() \
    GTEST_API_ int main(int argc, char **argv) { \
      printf("Running main() from %s\n", __FILE__); \
      testing::InitGoogleTest(&argc, argv); \
      return RUN_ALL_TESTS(); \
    }

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