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
 * @brief 计算数组@array的元素个数
 */
#define ARRAY_SIZE(array)   (sizeof(array) / sizeof((array)[0]))

/**
 * @brief 计算结构体@type中成员@member的偏移量
 */
#define OFFSETOF(type, member)   ((unsigned long)&((type *)0)->member)

/**
 * @brief 通过结构体@type中成员@member的地址@ptr计算结构体本身的地址
 */
#define CONTAINER_OF(ptr, type, member)   ((type *)((char *)(ptr) - OFFSETOF(type, member)))

/**
 * @brief 计算两个值@a和@b中的最小值和最大值
 */
#define MIN(a, b)   (((a) < (b)) ? (a) : (b))
#define MAX(a, b)   (((a) > (b)) ? (a) : (b))

/**
 * @brief 计算三个值@a、@b和@c中的最小值和最大值
 */
#define MIN3(a, b, c)   MIN(MIN(a, b), c)
#define MAX3(a, b, c)   MAX(MAX(a, b), c)

/**
 * @brief 对值@x的@bit位进行清零
 */
#define CLEAR_BIT(x, bit)   (x &= ~(1 << bit))
/**
 * @brief 对值@x的@bit位进行置零
 */
#define SET_BIT(x, bit)   (x |= (1 << bit))
/**
 * @brief 获取值@x的@bit位
 */
#define GET_BIT(x, bit)   ((x & (1 << bit)) >> bit)

#if defined(__cplusplus)
}
#endif

#endif /* #ifndef __UTIL_H */
