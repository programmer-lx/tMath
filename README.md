# tMath - Highly Type-Decoupled, Generic C++ Math Library
[中文](README-CN.md)

tMath is a header-only, C++20 Concepts-based highly generalized math library, focusing on **Template-based Decoupling** and **High-Performance Computation (SIMD-ready)**.

It allows developers to use **ANY** struct that conforms to a specific memory layout (e.g., `struct { float x, y; }`) for vector operations without inheriting from any base classes in the library, achieving **zero-intrusion** abstraction.


## ✨ Highlights
| **Feature**                        | **Description**                                                                                                                                                                                                          |
| ---------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| ✨**Zero Type Intrusion**           | Developers can bring their own data types. Your custom struct (e.g., `Vector2f`) can be used directly in all `tMath` functions without inheritance or wrapping, achieving complete separation of type and functionality. |
| ✨**SIMD Optimization Ready**       | Provide SIMD functions and specialized SIMD concepts to significantly boost the performance of mathematical operations.     |
| **Safe Integer Arithmetic**        | Intelligently handles vector calculations involving integers. Automatic type promotion (e.g., `int32_t` promoted to `double`) eliminates overflow risks and precision issues in calculations.                            |
| **Zero-Cost Quat Differentiation** | Uses the **Tag Type** mechanism to differentiate between `Vector4` and `Quat`, which share the same memory layout, ensuring correct mathematical dispatch without increasing struct size.                                |


## 📐 Design Philosophy: Highly Type Decoupling

The core philosophy of tMath is decoupling. It hands over the definition rights of **data structures** entirely to the developer, while the library's sole responsibility is to verify whether the structure meets the operational requirements through **Concepts**.

### Example 1: Custom Vector Types

```c++
// User's custom type
struct MyVector2
{
    float x;
    float y;
};

struct MyVec2 
{
    float x = 0, y = 0;
};

// One line of code enables operator overloading for custom types (can be defined globally or within a function scope)
TMATH_ENABLE_OPERATORS

// tMath automatically recognizes and provides all operations
MyVector2 p1 = { 1.5f, 2.5f };
MyVector2 p2 = { 3.5f, 2.5f };

// Vector addition (operator+)
MyVector2 p3 = p1 + p2; // p3 = {5.0f, 5.0f}

// Dot product (dot)
float d = tMath::dot(p1, p2); // d = 1.5*3.5 + 2.5*2.5 = 11.5f


// Custom MyVec2 type
MyVec2 vec = { 1000, 0 };
assert(tMath::approximately(tMath::normalized(vec), { 1, 0 }));
```

## Example 2: Hyper-Flexible Type Casting (tMath::precision_cast(vec_or_mat))

```c++
// Developer-defined Vector2 type
struct Vector2f
{
    float x = 0, y = 0;
};

// Developer-defined another Vector2 type
struct Vector2i16
{
    int16_t x = 0, y = 0;
};

// Type casting
Vector2f v2f = { 1.1, 2.2 };
Vector2i16 test = { 1, 2 };
assert(tMath::precision_cast<Vector2i16>(v2f) == test);
```

## Example 3: Automatic Precision Promotion

```c++
using Vector2i32 = struct { int32_t x, y; };

Vector2i32 a = { 5, 10 };
Vector2i32 b = { 2, -5 };

// Return type: automatically promoted to double
auto dis = tMath::distance(a, b); 
// The type of dis is double, and the result is 15.29705854077835449
```

## CMake Integration
1. Add tMath as sub_directory
```cmake
add_subdirectory(${your_project_path}/3rdparty/tMath)
```
2. link tMath
```cmake
add_executable(my_project source.cpp)
target_link_libraries(my_project PRIVATE tMath)
```
