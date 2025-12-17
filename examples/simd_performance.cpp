#include <vector>

#include "utils.hpp"

#define NOMINMAX
#include <Windows.h>
#include <cfloat>
#include <thread>

#include <tMath/simd/float32_4.hpp>
#include <tMath/vector4.hpp>

struct Vector4f
{
    TMATH_VECTOR4(Vector4f, float)
};

struct Transform
{
    Vector4f position = { FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN };
    Vector4f rotation = { FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN };
    Vector4f scale = { FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN };
};

using namespace tSimd;

struct TransformSimd
{
    float32_4 position = tSimd::set(FLT_MIN);
    float32_4 rotation = tSimd::set(FLT_MIN);
    float32_4 scale = tSimd::set(FLT_MIN);
};

class WindowsTimer
{
public:
    static WindowsTimer& instance()
    {
        static WindowsTimer w;
        return w;
    }

    WindowsTimer()
    {
        timeBeginPeriod(1);
    }

    ~WindowsTimer()
    {
        timeEndPeriod(1);
    }
};

int main()
{
    auto& w = WindowsTimer::instance();

    int frame_count = 3;
    int object_count = 100000;
    int test_count = 10;
    int compute_count = static_cast<int>(std::log2(static_cast<float>(object_count))); // 模拟 O(nlogn) 的对象间遍历计算
    double waste_time_microseconds = 2000;

    for (int t = 0; t < test_count; ++t)
    {
        {
            std::vector<Transform> objects(object_count, Transform());

            Timer timer("no-simd");

            float v = tMath::sin(random_f(0.0f, 10.0f));

            for (int i = 0; i < frame_count; ++i)
            {
                // 假设游戏有其他的运算
                waste_time(waste_time_microseconds);

                for (int j = 0; j < object_count; ++j)
                {
                    auto& trans = objects[j];

                    for (int k = 0; k < compute_count; ++k)
                    {
                        // test sin
                        float v2 = tMath::sin(v);

                        trans.position *= 10;
                        trans.position += { 1, 2, 3, 4 };

                        trans.position.x += v2;
                        trans.position.y += v2;
                        trans.position.z += v2;
                        trans.position.w += v2;

                        trans.rotation *= 3;
                        trans.rotation += { 1, 2, 3, 4 };

                        trans.rotation.x += v2;
                        trans.rotation.y += v2;
                        trans.rotation.z += v2;
                        trans.rotation.w += v2;
                    }
                }
            }

            double end = timer.get_time();
            std::cout << "[no-SIMD] FPS = " << 1000.0 / (end / double(frame_count)) << std::endl;
        }

        {
            std::vector<TransformSimd> objects(object_count, TransformSimd());

            Timer timer("simd");

            float32_4 v = tSimd::sin(tSimd::set(random_f(0.0f, 10.0f)));

            for (int i = 0; i < frame_count; ++i)
            {
                // 假设游戏有其他的运算
                waste_time(waste_time_microseconds);

                for (int j = 0; j < object_count; ++j)
                {
                    auto& trans = objects[j];

                    for (int k = 0; k < compute_count; ++k)
                    {
                        // test sin
                        float32_4 v2 = tSimd::sin(v);
                        // do NOT use v = tSimd::sin(v);

                        trans.position = tSimd::mul_add(trans.position, tSimd::set(10), tSimd::set(1, 2, 3, 4));
                        trans.position = tSimd::add(trans.position, v2);

                        trans.rotation = tSimd::mul_add(trans.rotation, tSimd::set(3), tSimd::set(1, 2, 3, 4));
                        trans.rotation = tSimd::add(trans.rotation, v2);
                    }
                }
            }

            double end = timer.get_time();
            std::cout << "[SIMD] FPS = " << 1000.0 / (end / double(frame_count)) << std::endl;
            std::cout << "=========================" << std::endl;
        }
    }

    return 0;
}