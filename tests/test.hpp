#pragma once

#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>
#include <random>
#include <chrono>


#define TEST_TITLE(title) \
    do { \
        std::cout << "--- " << title << " ---" << std::endl; \
    } while (0)

#define TEST_BOOL(condition) \
    do { \
        if (!(condition)) { \
            std::cerr << "--- TEST FAILED ---" << std::endl; \
            std::cerr << "  Condition: " << #condition << std::endl; \
            std::cerr << "  File:      " << __FILE__ << std::endl; \
            std::cerr << "  Line:      " << __LINE__ << std::endl; \
            throw std::runtime_error(""); \
        } else { \
            std::cout << "Testing: " << #condition << '\n'; \
        } \
    } while (0)

#define TEST_BOOL_N(count, exp) \
    do { for (int __i = 0; __i < count; ++__i) { exp } } while (0)

#define TEST_END(title) \
    do { std::cout << "--- " << title << "(end)" << " ---\n" << std::endl; } while (0)

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