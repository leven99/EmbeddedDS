/*
 * Copyright (C) 2018-2019 Leven.
 * Copyright (C) 2013 Stefani Seibold <stefani@seibold.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __FIFO_H
#define __FIFO_H

#if defined(__cplusplus)
    extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Public typedef ------------------------------------------------------------*/
struct __fifo {
    uint32_t    in;       /* 入队索引，in  必须为无符号数据类型 */
    uint32_t    out;      /* 出队索引，out 必须为无符号数据类型 */
    uint32_t    mask;     /* 队列总大小，mask 必须为 2 的幂次方 */
    uint32_t    esize;    /* 数据元素的大小 */
    uint8_t    *data;     /* 数据指针 */
};

/* Public define -------------------------------------------------------------*/
/**
 * fifo_init - 使用预分配的缓冲区静态队列初始化
 * @fifo:   预分配的队列缓冲区
 * @buffer: 需要使用的预分配缓冲区
 * @size:   缓冲区大小，必须为2的幂次方
 */
#define fifo_init(fifo, buffer, size) \
( \
    __fifo_init(fifo, buffer, size, sizeof(*buffer)) \
)

/**
 * fifo_in - 向队列入队数据
 * @fifo: 队列指针
 * @buf:  入队的数据
 * @len:  入队的数据元素数量
 * @return：返回入队的数据元素数量
 */
#define	fifo_in(fifo, buffer, len) \
( \
    __fifo_in(fifo, buffer, len) \
)

/**
 * fifo_out - 从队列出队数据
 * @fifo: 队列指针
 * @buf:  出队的数据缓冲区指针
 * @len:  出队的最大数据元素数量
 */
#define	fifo_out(fifo, buffer, len) \
( \
    __fifo_out(fifo, buffer, len) \
)

/* Private function -----------------------------------------------------------*/
/**
 * @brief  清除队列中中的所有内容
 * @param  fifo：队列指针
 * @retval None.
 * @note   使用fifo_reset()很危险，应仅在队列没被锁定或其他线程没有访问时调用
 */
static inline void fifo_reset(struct __fifo *fifo)
{
    fifo->in  = 0U;
    fifo->out = 0U;
}

/**
 * @brief  返回队列大小
 * @param  fifo：队列指针
 * @retval 队列大小
 */
static inline uint32_t fifo_size(struct __fifo *fifo)
{
    return (fifo->mask + 1U);
}

/**
 * @brief  返回队列数据元素的大小
 * @param  fifo：队列指针
 * @retval 队列数据元素的大小
 */
static inline uint32_t fifo_esize(struct __fifo *fifo)
{
    return (fifo->esize);
}

/**
 * @brief  返回队列中已使用数据元素的长度
 * @param  fifo：队列指针
 * @retval 队列中已使用数据元素的长度
 */
static inline uint32_t fifo_len(struct __fifo *fifo)
{
    /*
     * 假设共有32个数据元素，且入队13个数据元素
     * .------------------------------------------.
     * | 0 | 1 | 2 | 3 | ... | 12 | 13 | ... | 31 |
     * |--------------------------|---------------|
     *  \______13个数据元素______/ \_____________/
     *   \                      /       \
     *    \                 in_/         \
     *     \_out                          \_剩余自由空间
     *
     * 已使用数据元素长度(len)13 = 入队偏移量(in)13 - 出队偏移量(out)0
     */
    return (fifo->in - fifo->out);
}

/**
 * @brief  返回队列中未使用数据元素的长度
 * @param  fifo：队列指针
 * @retval 队列中未使用数据元素的长度
 */
static inline uint32_t fifo_avail(struct __fifo *fifo)
{
    return (fifo_size(fifo) - fifo_len(fifo));
}

/**
 * @brief  队列是否为空
 * @param  fifo：队列指针
 * @retval 如果队列为空，返回true
 */
static inline bool fifo_is_empty(struct __fifo *fifo)
{
    return (fifo->in == fifo->out);
}

/**
 * @brief  队列是否以满
 * @param  fifo：队列指针
 * @retval 如果队列已满，返回true
 */
static inline bool fifo_is_full(struct __fifo *fifo)
{
    return (fifo_len(fifo) > fifo->mask);
}

/* Public function prototypes ------------------------------------------------*/
/**
 * 队列初始化
 */
extern void __fifo_init(struct __fifo *fifo, uint8_t *data_buf, uint32_t size, size_t esize);

/**
 * 向队列中入队数据
 */
extern uint32_t __fifo_in(struct __fifo *fifo, const uint8_t *buffer, uint32_t len);

/**
 * 从队列中出队数据
 */
extern uint32_t __fifo_out(struct __fifo *fifo, uint8_t *buffer, uint32_t len);

#if defined(__cplusplus)
    }
#endif

#endif /* #ifndef __FIFO_H */
