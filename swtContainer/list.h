#pragma once
#ifndef __SWT_LIST_H
#define __SWT_LIST_H

#include "stddef.h"
#include "macro.h"

struct list_head {
    struct list_head *next, *prev;
};
typedef struct list_head *list_head_t;

#define LIST_HEAD_INIT(name) { &(name), &(name) }
 
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)
 
static inline void list_add(list_head_t ptr,list_head_t head){
    (ptr)->next=(head)->next;
    (ptr)->prev=(head);
    (ptr)->next->prev=(ptr);
    (head)->next=(ptr);
}
static inline void list_add_tail(list_head_t ptr,list_head_t head){
    (ptr)->prev=(head)->prev;
    (ptr)->next=(head);
    (ptr)->prev->next=(ptr);
    (head)->prev=(ptr);
}
static inline void list_del(list_head_t ptr){
    (ptr)->prev->next=(ptr)->next;
    (ptr)->next->prev=(ptr)->prev;
    (ptr)->next=NULL;
    (ptr)->prev=NULL;
}
static inline void list_replace(list_head_t ptr,list_head_t newp){
    (newp)->prev=(ptr)->prev;
    (newp)->next=(ptr)->next;
    (newp)->prev->next=(newp);
    (newp)->next->prev=(newp);
}
static inline void list_move_head(list_head_t ptr,list_head_t head){
    list_del(ptr);
    list_add(ptr,head);
}
static inline void list_move_tail(list_head_t ptr,list_head_t head){
    list_del(ptr);
    list_add_tail(ptr,head);
}
#define list_is_empty(head) ((head)->next==(head))
#define list_entry(ptr,type,member)\
container_of(ptr,type,member)

/**
 * @brief	循环整个链表时的for(;x;x)语句（循环时不能删除当前链表节点）
 * @param	pos:	当前循环到的节点，是个临时变量
 * @param	head:	链表入口
 */
#define list_for_each(pos,head)\
for((pos)=(head)->next;(pos)!=(head);(pos)=(pos)->next)

/**
 * @brief	循环整个链表时的for(;x;x)语句（循环时可以删除当前链表节点）
 * @param	pos:	当前循环到的节点，是个临时变量
 * @param	n:		作为临时变量的节点
 * @param	head:	链表入口
 */
#define list_for_each_safe(pos,n,head)\
for((pos)=(head)->next,(n)=pos->next;\
    (pos)->next!=(head);\
    (pos)=(n).(n)=(pos)->next)

#define list_for_each_tail(pos,head)\
for((pos)=(head)->prev;(pos)!=(head);(pos)=(pos)->prev)

#define list_for_each_tail_safe(pos,n,head)\
for((pos)=(head)->prev,(n)=pos->prev;\
    (pos)->prev!=(head);\
    (pos)=(n).(n)=(pos)->prev)

/**
 * @brief	在上层结构体的基础上循环整个链表时的for(;x;x)语句（循环时不能删除当前链表节点）
 * @details	虽然是在上层带有效数据的结构体指针上进行循环，但是实际实现时
 *			是以其中的链表结构体作为依据
 * @param	pos:	要循环的上层结构体临时变量，该结构体中带有链表成员，
 *					pos值用作变量，本身不必预先赋值，但是循环时不能删除它
 * @param	head:	上层结构体中的list_head成员的地址
 * @param	member:	上层结构体中的list_head成员的名称
 * @note	head虽然是链表入口地址，但本身是尾(最后弹出)地址，head->next是头(最先弹出)地址；
 */
#define list_for_each_entry(pos, head, member)                          \
        for ((pos) = list_entry((head)->next, typeof(*(pos)), member);      \
             &(pos)->member != (head);    \
             (pos) = list_entry((pos)->member.next, typeof(*(pos)), member))
 
/**
 * @brief	在上层结构体的基础上循环整个链表时的for(;x;x)语句（支持循环时删除当前链表节点）
 * @details	虽然是在上层带有效数据的结构体指针上进行循环，但是实际实现时
 *			是以其中的链表结构体作为依据
 * @param	pos:	要循环的上层结构体临时变量，该结构体中带有链表成员，
 *					pos值用作变量，本身不必预先赋值，循环时可以删除它
 * @param	n:		上层结构体临时变量
 * @param	head:	上层结构体中的list_head成员的地址
 * @param	member:	上层结构体中的list_head成员的名称
 * @note	head虽然是链表入口地址，但本身是尾(最后弹出)地址，head->next是头(最先弹出)地址；
 */
#define list_for_each_entry_safe(pos, n, head, member)                  \
        for ((pos) = list_entry((head)->next, typeof(*pos), member),      \
                (n) = list_entry((pos)->member.next, typeof(*pos), member); \
             &(pos)->member != (head);                                    \
             (pos) = (n), (n) = list_entry(n->member.next, typeof(*n), member))

#define list_for_each_tail_entry(pos, head, member)                          \
        for ((pos) = list_entry((head)->prev, typeof(*(pos)), member);      \
             &(pos)->member != (head);    \
             (pos) = list_entry((pos)->member.prev, typeof(*(pos)), member))

#define list_for_each_tail_entry_safe(pos, n, head, member)                  \
        for ((pos) = list_entry((head)->prev, typeof(*pos), member),      \
                (n) = list_entry((pos)->member.prev, typeof(*pos), member); \
             &(pos)->member != (head);                                    \
             (pos) = (n), (n) = list_entry(n->member.prev, typeof(*n), member))


static inline void list_fifo_push(list_head_t pos,list_head_t head){
    list_add_tail(pos,head);
}
static inline void list_filo_push(list_head_t pos,list_head_t head){
    list_add(pos,head);
}
static inline list_head_t list_pop(list_head_t head){
    if(list_is_empty(head))return NULL;
    list_head_t rst=head->next;
    list_del(rst);
    return rst;
}
// #define _gen_list_pop(head,member)do{\
//     if(list_is_empty(head->member))return NULL;\
//     typedef(*head) rst=list_entry(head->next,typeof(*head),member);\
//     list_del(rst->member);\
//     return rst;\
// }while(0)


#endif


