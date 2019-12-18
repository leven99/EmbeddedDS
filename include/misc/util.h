/**
  * Copyright (C) 2018-2019, Leven.
  * Copyright (c) 2011-2014, Wind River Systems, Inc.
  *
  * SPDX-License-Identifier: Apache-2.0
  */

#ifndef __UTIL_H
#define __UTIL_H

/* Public define -------------------------------------------------------------*/
#ifndef _ASMLANGUAGE
/**
  * ARRAY_SIZE：获取数组@array中的元素数量
  */
#if defined (__GNUC__)
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

/**
  * 如果@array是数组，则求值为零；如果不是数组，则编译错误
  */
#define IS_ARRAY(array) \
    ZERO_OR_COMPILE_ERROR( \
        !__builtin_types_compatible_p(__typeof__(array), \
                __typeof__(&(array)[0])))

#define ARRAY_SIZE(array) \
    ((unsigned long) (IS_ARRAY(array) + \
        (sizeof(array) / sizeof((array)[0]))))
#else
#define ARRAY_SIZE(array)   (sizeof(array) / sizeof((array)[0]))
#endif

/**
  * 计算结构体@type中成员@member的大小
  */
#define FIELD_SIZEOF(type, member)  (sizeof(((type*)0)->member))

/**
  * 计算两个值@a和@b中的最小值和最大值
  */
#if defined (__GNUC__)
#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })
#define MAX(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a > _b ? _a : _b; })
#else
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#endif

/**
  * 计算三个值@a、@b和@c中的最小值和最大值
  */
#if defined (__GNUC__)
#define MIN3(a, b, c)   MIN((typeof(a))MIN(a, b), c)
#define MAX3(a, b, c)   MAX((typeof(a))MAX(a, b), c)
#else
#define MIN3(a, b, c)   MIN(MIN(a, b), c)
#define MAX3(a, b, c)   MAX(MAX(a, b), c)
#endif

/* KB, MB, GB */
#define KB(x) ((x) << 10)
#define MB(x) (KB(x) << 10)
#define GB(x) (MB(x) << 10)

/* KHZ, MHZ */
#define KHZ(x) ((x) * 1000)
#define MHZ(x) (KHZ(x) * 1000)

#ifndef BIT
#if defined(_ASMLANGUAGE)
#define BIT(n)  (1 << (n))
#else
#define BIT(n)  (1UL << (n))
#endif
#endif /* !BIT */

/**
  * @brief 根据布尔值设置或清除位
  * @param var：要更改的变量
  * @param bit：bit位
  * @param set：值为0清除bit，任何其他值设置bit
  */
#define WRITE_BIT(var, bit, set) ((var) = (set) ? ((var) | BIT(bit)) : ((var) & ~BIT(bit)))
#define BIT_MASK(n) (BIT(n) - 1)

#endif /* !_ASMLANGUAGE */

/* Private function -----------------------------------------------------------*/
/* 测试一个数是否是2的幂次方 */
static inline int is_power_of_two(unsigned int x)
{
    return (x != 0) && !(x & (x - 1));
}

/* 延时@n个时钟周期 */
static inline void nops(unsigned int n)
{
    volatile unsigned int cnt = n;

    for(; cnt; cnt--) {
        __asm volatile("nop");
    }
}

#endif /* #ifndef __UTIL_H */
