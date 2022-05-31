/*
 * Copyright 2021 Leven.
 */

#ifndef __DLIST_H
#define __DLIST_H

/* Includes ------------------------------------------------------------------*/
#include <stdbool.h>

/* Public typedef ------------------------------------------------------------*/
struct dlist_node {
    struct dlist_node *next, *prev;
};

/* Public define -------------------------------------------------------------*/
#ifndef OFFSETOF
#define OFFSETOF(type, member)   ((unsigned long)&(((type *)0)->member))
#endif

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member)   ((type *)((char *)(ptr) - OFFSETOF(type, member)))
#endif

/**
 * dlist_entry: 通过指向成员变量的指针获取结构体变量的地址
 * @param ptr 指向成员变量的指针
 * @param type 结构体类型
 * @param member 结构体成员
 */
#define dlist_entry(ptr, type, member) \
    CONTAINER_OF(ptr, type, member)

/**
 * dlist_for_each: 遍历链表
 * @param pos 循环游标变量
 * @param head 指向将要遍历链表的指针
 */
#define dlist_for_each(pos, head) \
    for ((pos) = (head)->next; pos != head; (pos) = (pos)->next)

/* Private function ----------------------------------------------------------*/
static inline void dlist_init(struct dlist_node *list)
{
    list->next = list;
    list->prev = list;
}

static inline void dlist_insert_after(struct dlist_node *list, struct dlist_node *node)
{
    list->next->prev = node;
    node->next = list->next;

    list->next = node;
    node->prev = list;
}

static inline void dlist_insert_before(struct dlist_node *list, struct dlist_node *node)
{
    list->prev->next = node;
    node->prev = list->prev;

    list->prev = node;
    node->next = list;
}

static inline bool dlist_empty(struct dlist_node *list)
{
	return (list->next == list);
}

static inline unsigned long dlist_len(const struct dlist_node *list)
{
    unsigned long len = 0;
    const struct dlist_node *l = list;

    while (l->next != l) {
        l = l->next;
        len++;
    }

    return len;
}

/* Public function prototypes ------------------------------------------------*/

#endif /* #ifndef __DLIST_H */
