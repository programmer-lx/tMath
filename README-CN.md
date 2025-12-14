# tMath - 高度类型解耦、泛型化 C++ 数学库

tMath 是一个header-only，基于 C++20 Concepts 的高度泛型化数学库，专注于类型解耦（Template-based Decoupling）和高性能运算（SIMD-ready）。

它允许开发者使用任何符合特定内存布局的结构体（例如 struct { float x, y; }）进行向量运算，无需继承库内基类，实现了零侵入性的抽象。

## ✨核心亮点

| **特性**         | **描述**                                                                      |
| -------------- | --------------------------------------------------------------------------- |
| ✨**类型零侵入性**    | 开发者可以自带数据类型。您的自定义结构体（如 `Vector2f`）可直接用于所有 `tMath` 函数，无需继承或封装，实现了类型与功能的完全分离。 |
| ✨**SIMD 优化就绪** | 针对 `Vector4` 等关键类型，API 设计已预留 SIMD 优化的接口。底层特化完成后，上层代码将自动获得性能提升。              |
| **安全的整数运算**    | 智能处理整数的向量计算。通过自动类型提升（如 `int32_t` 提升到 `double`）来消除计算中的溢出风险和精度问题。             |
| **Quat 零开销区分** | 使用 **Tag Type** 机制区分内存布局相同的 `Vector4` 和 `Quat`，保证了数学运算的正确分派，且不增加结构体大小。      |



## 📐 设计哲学：类型解耦

tMath 的核心理念是解耦。它将数据结构的定义权完全交给了开发者，而库只负责通过 Concepts 检查结构是否符合运算要求。

### 示例1：自定义Vector类型

```c++
// 用户的自定义类型
struct MyVector2
{
    float x;
    float y;
};

struct MyVec2 
{
    float x = 0, y = 0;
};

// tMath 自动识别并提供所有运算
MyVector2 p1 = { 1.5f, 2.5f };
MyVector2 p2 = { 3.5f, 2.5f };

// 一行代码支持自定义类型的运算符重载 (可以全局定义，也可以定义在函数作用域内)
TMATH_ENABLE_OPERATORS

// 向量加法 (operator+)
MyVector2 p3 = p1 + p2; // p3 = {5.0f, 5.0f}

// 点积运算 (dot)
float d = tMath::dot(p1, p2); // d = 1.5*3.5 + 2.5*2.5 = 11.5f


// 自定义MyVec2类型
MyVec2 vec = { 1000, 0 };
assert(tMath::approximately(tMath::normalized(vec), { 1, 0 }));
```

### 示例2：超级自由的类型转换 (tMath::precision_cast<T>(vec_or_mat))

```c++
// 开发者自定义 Vector2 类型
struct Vector2f
{
    float x = 0, y = 0;
};

// 开发者自定义另一个 Vector2 类型
struct Vector2i16
{
    int16_t x = 0, y = 0;
};

// 类型转换
Vector2f v2f = { 1.1, 2.2 };
Vector2i16 test = { 1, 2 };
assert(tMath::precision_cast<Vector2i16>(v2f) == test);
```

### 示例3：自动精度提升

```c++
using Vector2i32 = struct { int32_t x, y; };

Vector2i32 a = { 5, 10 };
Vector2i32 b = { 2, -5 };

// 返回类型：自动提升到 double
auto dis = tMath::distance(a, b); 
// dis 的类型是 double，结果为 15.29705854077835449
```
