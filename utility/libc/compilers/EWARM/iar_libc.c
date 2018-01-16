/*
 * Copyright (C) 2015-2017 Alibaba Group Holding Limited
 */

#ifdef __ICCARM__
#include <stdarg.h>
#include <sys/types.h>
#include <time.h>
#include <stdio.h>

extern void *aos_malloc(unsigned int size);
extern void aos_alloc_trace(void *addr, size_t allocator);
extern void aos_free(void *mem);
extern void *aos_realloc(void *mem, unsigned int size);
extern long long aos_now_ms(void);

__ATTRIBUTES void *malloc(unsigned int size)
{
    void *mem;

#if (RHINO_CONFIG_MM_DEBUG > 0u && RHINO_CONFIG_GCC_RETADDR > 0u)
    mem = aos_malloc(size | AOS_UNSIGNED_INT_MSB);
#else
    mem = aos_malloc(size);
#endif

    return mem;
}

__ATTRIBUTES void *realloc(void *old, unsigned int newlen)
{
    void *mem;

#if (RHINO_CONFIG_MM_DEBUG > 0u && RHINO_CONFIG_GCC_RETADDR > 0u)
    mem = aos_realloc(old, newlen | AOS_UNSIGNED_INT_MSB);
#else
    mem = aos_realloc(old, newlen);
#endif

    return mem;
}

__ATTRIBUTES void *calloc(size_t len, size_t elsize)
{
    void *mem;

#if (RHINO_CONFIG_MM_DEBUG > 0u && RHINO_CONFIG_GCC_RETADDR > 0u)
    mem = aos_malloc((elsize * len) | AOS_UNSIGNED_INT_MSB);
#else
    mem = aos_malloc(elsize * len);
#endif

    if (mem) {
        memset(mem, 0, elsize * len);
    }

    return mem;
}

__ATTRIBUTES void free(void *mem)
{
    aos_free(mem);
}

__ATTRIBUTES time_t time(time_t *tod)
{
    uint64_t t = aos_now_ms();
    return (time_t)(t / 1000);
}

int *__errno _PARAMS ((void))
{
    return 0;
}

void __assert_func(const char * a, int b, const char * c, const char *d)
{
    while (1);
}

 #endif
