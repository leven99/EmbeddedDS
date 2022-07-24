/*
 * A generic kernel KFIFO implementation
 *
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

/* Includes ------------------------------------------------------------------*/
#include <string.h>

#include "fifo.h"
#include "misc/util.h"

/* Private function prototypes -----------------------------------------------*/
/*
 * 环形队列入队子函数
 */
static void _fifo_copy_in(const struct __fifo *fifo, const uint8_t *src, uint32_t len, uint32_t off);

/*
 * 环形队列出队子函数
 */
static void _fifo_copy_out(const struct __fifo *fifo, uint8_t *dst, uint32_t len, uint32_t off);

/* Public functions ----------------------------------------------------------*/
/**
 * @brief  预分配缓冲区静态队列初始化
 * @param  fifo：队列指针
 *         data_buff：数据域指针
 *         size：队列大小
 *         esize: 数据元素大小
 * @retval None.
 */
void __fifo_init(struct __fifo *fifo, uint8_t *data_buf, uint32_t size, size_t esize)
{
    size /= esize;    /* 计算数据元素空间大小下的队列总大小 */

    fifo->in    = 0U;
    fifo->out   = 0U;
    fifo->mask  = size - 1U;
    fifo->esize = esize;
	fifo->data  = data_buf;
}

/**
 * @brief  向队列中入队数据
 * @param  fifo：队列指针
 *         buffer：入队的数据缓冲区指针
 *         len：   入队的数据长度
 * @retval 入队字节数
 */
uint32_t __fifo_in(struct __fifo *fifo, const uint8_t *buffer, uint32_t len)
{
    uint32_t tmp_len;

    /* 计算队列中剩余可用空间的大小（总空间大小-已使用空间大小） */
    tmp_len = (fifo->mask + 1U) - (fifo->in - fifo->out);
    if(len > tmp_len) {
        len = tmp_len;
    }

    /* 执行入队操作，将入队的数据缓冲区数据拷贝到队列中 */
    _fifo_copy_in(fifo, buffer, len, fifo->in);

    /* 递增fifo-in，利用无符号数据类型的溢出特性环绕 */
    fifo->in += len;

    return len;
}

/**
 * @brief  从队列中出队数据
 * @param  fifo：队列指针
 *         buffer：出队的数据缓冲区指针
 *         len：   出队的数据长度
 * @retval 出队字节数
 */
uint32_t __fifo_out(struct __fifo *fifo, uint8_t *buffer, uint32_t len)
{
    uint32_t tmp_len;

    /* 计算队列中已使用的空间大小 */
    tmp_len = fifo->in - fifo->out;
    if (len > tmp_len) {
        len = tmp_len;
    }

    /* 执行出队操作，将队列中的数据拷贝到出队的数据缓冲区中 */
    _fifo_copy_out(fifo, buffer, len, fifo->out);

    /* 递增fifo-out，利用无符号数据类型的溢出特性环绕 */
    fifo->out += len;

    return len;
}

/* Private functions ---------------------------------------------------------*/
static void _fifo_copy_in(const struct __fifo *fifo, const uint8_t *src, uint32_t len, uint32_t off)
{
    uint32_t size  = fifo->mask + 1U;
    uint32_t esize = fifo->esize;
    uint32_t tmp_len;

    /* 计算队列中数据入队的起始位置in */
    off &= fifo->mask;

    /* 数据域数据元素的数据类型非字节 */
    if (esize != 1U) {
        off  *= esize;
        size *= esize;
        len  *= esize;
    }
    /* size - off表示当前队列fifo->in位置至队列尾部的大小 */
    tmp_len = MIN(len, size - off);

    /*
     * 假设共有32个数据元素，入队13个数据，接着出队3个数据：
     * .--------------------------------------------------.
     * | 0 | 1 | 2 | 3 | 4 |... | 12 | 13 | ... | 30 | 31 |
     * |-----------|-----------------|--------------------|
     *            / \_10个数据元素__/ \__________________/
     *       out_/                 /         \
     *                         in_/           \_剩余自由空间
     *
     * 此时未使用数据元素22 = 出队偏移量(out)3 + 剩余自由空间19。
     *
     * 当再次入队21个数据时：
     * .--------------------------------------------------.
     * | 0 | 1 | 2 | 3 | 4 |... | 12 | 13 | ... | 30 | 31 |
     * |-------|---|-----------------|--------------------|
     *  \_____/ \   \__10个数据元素_/ \___________________/
     *     \     \   \                       \
     *      \     \   \___________out         \
     *       \     \__________in               \_19个数据元素
     *        \
     *         \_2个数据元素
     *
     * 超过的2个数据元素将存储到队列的头部，剩余自由空间1个。
     */

    /* 首先拷贝tmp_len字节到队列fifo->in位置至队列尾部位置的空间中 */
    memcpy(fifo->data + off, src, tmp_len);

    /* 然后拷贝剩余len-tmp_len字节（如果还有的话）到队列头部 */
    memcpy(fifo->data, src + tmp_len, len - tmp_len);
}

static void _fifo_copy_out(const struct __fifo *fifo, uint8_t *dst, uint32_t len, uint32_t off)
{
    uint32_t size  = fifo->mask + 1U;
    uint32_t esize = fifo->esize;
    uint32_t tmp_len;

    /* 计算队列中数据出队的起始位置out */
    off &= fifo->mask;

    /* 数据元素的数据类型非字节 */
    if (esize != 1U) {
        off  *= esize;
        size *= esize;
        len  *= esize;
    }
    tmp_len = MIN(len, size - off);

    memcpy(dst, fifo->data + off, tmp_len);
    memcpy(dst + tmp_len, fifo->data, len - tmp_len);
}
