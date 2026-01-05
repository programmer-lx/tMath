#pragma once

#include <cstdlib>

#include <type_traits>
#include <new>

#include "impl/platform.hpp"
#include "impl/ops/simd_instruction_select.hpp"

TSIMD_NAMESPACE_BEGIN

inline void* aligned_allocate(size_t bytes, size_t alignment) noexcept
{
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    return _aligned_malloc(bytes, alignment);
#else
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, bytes) != 0)
    {
        return nullptr;
    }
    return ptr;
#endif
}

template<typename T>
    requires std::is_pointer_v<T>
inline T aligned_allocate(size_t bytes, size_t alignment) noexcept
{
    return static_cast<T>(aligned_allocate(bytes, alignment));
}

inline void aligned_free(void* mem) noexcept
{
#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__MINGW64__)
    _aligned_free(mem);
#else
    free(mem);
#endif
}


// 以最大对齐字节进行分配
template<typename T>
struct AlignedAllocator
{
    static_assert(!std::is_const_v<T>);
    static_assert(!std::is_function_v<T>);
    static_assert(!std::is_reference_v<T>);

    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;

    static size_t alignment()
    {
        return InstructionSelector::required_alignment();
    }

    constexpr AlignedAllocator() noexcept = default;

    constexpr AlignedAllocator(const AlignedAllocator&) noexcept = default;

    template <class Other>
    constexpr AlignedAllocator(const AlignedAllocator<Other>&) noexcept {}

    constexpr ~AlignedAllocator() = default;

    constexpr AlignedAllocator& operator=(const AlignedAllocator&) = default;

    [[nodiscard]] constexpr T* allocate(const size_t count)
    {
        static size_t align = InstructionSelector::required_alignment();

        size_t bytes = count * sizeof(T);
        void* ptr = aligned_allocate(bytes, align);

        if (!ptr)
        {
            throw std::bad_alloc();
        }

        return static_cast<T*>(ptr);
    }

    constexpr void deallocate(T* const mem, const size_t count)
    {
        aligned_free(mem);
    }
};

TSIMD_NAMESPACE_END
