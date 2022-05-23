/*
 * Copyright 2021 Leven.
 */

#ifndef __LLIST_H
#define __LLIST_H

/* Includes ------------------------------------------------------------------*/
#include <stddef.h>
#include <stdbool.h>

/* Public typedef ------------------------------------------------------------*/
struct llist_node {
    struct llist_node *next;
};

/* Public define -------------------------------------------------------------*/
#ifndef OFFSETOF
#define OFFSETOF(type, member)   ((unsigned long)&(((type *)0)->member))
#endif

#ifndef CONTAINER_OF
#define CONTAINER_OF(ptr, type, member)   ((type *)((char *)(ptr) - OFFSETOF(type, member)))
#endif

/**
 * llist_entry: 通过指向成员变量的指针获取结构体变量的地址
 * @param ptr 指向成员变量的指针
 * @param type 结构体类型
 * @param member 结构体成员
 */
#define llist_entry(ptr, type, member) \
    CONTAINER_OF(ptr, type, member)

/**
 * llist_for_each: 遍历链表
 * @param pos 循环游标变量
 * @param head 指向将要遍历链表的指针
 */
#define llist_for_each(pos, head) \
    for ((pos) = (head)->next; pos != NULL; (pos) = (pos)->next)

/* Private function ----------------------------------------------------------*/
static inline void llist_init(struct llist_node *list)
{
    list->next = NULL;
}

static inline void llist_append(struct llist_node *list, struct llist_node *node)
{
    struct llist_node *n = list;

    while (n->next) {
        n = n->next;
    }

    n->next = node;
    node->next = NULL;
}

static inline void llist_insert(struct llist_node *list, struct llist_node *node)
{
    node->next = list->next;
    list->next = node;
}

static inline struct llist_node *llist_first(struct llist_node *list)
{
    return list->next;
}

static inline struct llist_node *llist_tail(struct llist_node *list)
{
    while (NULL != list->next) {
        list = list->next;
    }

    return list;
}

static inline struct llist_node *llist_next(struct llist_node *node)
{
	return node->next;
}

static inline bool llist_empty(struct llist_node *list)
{
	return (list->next == NULL);
}

static inline unsigned long llist_len(const struct llist_node *list)
{
    unsigned long len = 0;
    const struct llist_node *l = list->next;

    while (NULL != l) {
        l = l->next;
        len++;
    }

    return len;
}

/* Public function prototypes ------------------------------------------------*/

#endif /* #ifndef __LLIST_H */
