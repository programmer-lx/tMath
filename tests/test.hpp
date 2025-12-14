#pragma once

#include <stdexcept>
#include <exception>
#include <iostream>
#include <string>


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
            std::cout << "SUCCEED: " << #condition << std::endl; \
        } \
    } while (0)

#define TEST_END(title) \
    do { std::cout << "--- " << title << "(end)" << " ---\n" << std::endl; } while (0)
