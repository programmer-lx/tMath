#pragma once

#include <type_traits>

#include "ops/func_attr.hpp"
#include "platform.hpp"

TSIMD_NAMESPACE_BEGIN

TSIMD_NAMESPACE_END


#define TSIMD_DETAIL_DYN_DISPATCH_NS_PREFIX DynDispatch

// instruction充当命名空间
#define TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, instruction) \
    &TSIMD_NAMESPACE_NAME::TSIMD_DETAIL_DYN_DISPATCH_NS_PREFIX::instruction::func_name,

#define TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, Scalar)

#define TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, SSE2)

#define TSIMD_DETAIL_AVX_FUNC_IMPL(func_name) TSIMD_DETAIL_ONE_FUNC_IMPL(func_name, AVX)

// 不同后端的函数指针表
#define TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
    TSIMD_DETAIL_SCALAR_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_SSE2_FUNC_IMPL(func_name) \
    TSIMD_DETAIL_AVX_FUNC_IMPL(func_name)

#if !defined(TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY)
    #error "have not defined DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY to cache the simd function pointers"
#endif

#define TSIMD_DYN_EXPORT(func_name) \
    /* 函数指针类型别名 */ \
    using PFN_##func_name = decltype(&TSIMD_NAMESPACE_NAME::TSIMD_DYN_INSTRUCTION::func_name); \
    \
    /* 构建静态数组，存储函数指针 */ \
    static inline PFN_##func_name PFN_##func_name##_table[] = { \
        TSIMD_DETAIL_DYN_DISPATCH_FUNC_POINTER_STATIC_ARRAY(func_name) \
    };

#define TSIMD_DYN_FUNC_POINTER(func_name) \
    PFN_##func_name##_table[TSIMD_NAMESPACE_NAME::InstructionSelector::dyn_func_index()]

#define TSIMD_DYN_CALL(func_name) (TSIMD_DYN_FUNC_POINTER(func_name))



// --------------------------------- FUNC_ATTR字符串描述 ---------------------------------
// 将会在 dispatch_this_file.hpp 文件被多次重定义
#define TSIMD_DYN_FUNC_ATTR tsimd_error_attr
