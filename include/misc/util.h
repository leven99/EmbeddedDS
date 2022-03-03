/*
 * Copyright (C) 2018-2019, Leven.
 * Copyright (c) 2011-2014, Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __UTIL_H
#define __UTIL_H

#if defined(__cplusplus)
    extern "C" {
#endif

/* Public define -------------------------------------------------------------*/
/**
 * @brief 获取数组@array中的元素数量
 */
#if defined (__GNUC__)
#define ZERO_OR_COMPILE_ERROR(cond) ((int) sizeof(char[1 - 2 * !(cond)]) - 1)

/**
 * @brief 如果@array是数组，则求值为零；如果不是数组，则编译错误
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
 * @brief 计算结构体@type中成员@member的大小
 */
#define FIELD_SIZEOF(type, member)  (sizeof(((type *)0)->member))

/**
 * @brief 计算两个值@a和@b中的最小值和最大值
 */
#if defined (__GNUC__)
#define MIN(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a < _b ? _a : _b; })
#define MAX(a, b) ({ typeof(a) _a = (a); typeof(b) _b = (b); _a > _b ? _a : _b; })
#else
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))
#endif

/**
 * @brief 计算三个值@a、@b和@c中的最小值和最大值
 */
#if defined (__GNUC__)
#define MIN3(a, b, c)   MIN((typeof(a))MIN(a, b), c)
#define MAX3(a, b, c)   MAX((typeof(a))MAX(a, b), c)
#else
#define MIN3(a, b, c)   MIN(MIN(a, b), c)
#define MAX3(a, b, c)   MAX(MAX(a, b), c)
#endif

#if defined(__cplusplus)
    }
#endif

#endif /* #ifndef __UTIL_H */
