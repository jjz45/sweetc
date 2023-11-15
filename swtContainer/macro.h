#pragma once
#ifndef __SWT_MACRO_H
#define __SWT_MACRO_H

//根据一个结构体变量中的成员变量的地址，类型以及成员名来获取整个结构体变量的指针。
//通过将0地址强制类型转换后的成员变量取地址得到成员变量的偏移
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE*)0)->MEMBER)
//通过成员变量的偏移和成员变量的地址，得到容器的地址，这个可以用于异构操作，且上层无需关心派生类
#define container_of(ptr, type, member) ({          \
        const typeof( ((type *)0)->member ) *__mptr = (const typeof( ((type *)0)->member ) *)(ptr); \
        (type *)( (char *)__mptr - offsetof(type,member) );})

#endif

#define WRITE_ONCE(var, val) \
    (*((volatile typeof(val) *)(&(var))) = (val))

#define __CONCAT(a, b) a##b

/*
 * swap - swap value of @a and @b
 */
#define swap(a, b) \
    do { typeof(a) __tmp = (a); (a) = (b); (b) = __tmp; } while (0)


#define likely(x) __builtin_exp ect(!!(x), 1)
#define unlikely(x) __builtin_exp ect(!!(x), 0)


#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))



#define BSWAP_8(x) ((x) & 0xff)
#define BSWAP_16(x) ((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define BSWAP_32(x) ((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define BSWAP_64(x) ((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))


