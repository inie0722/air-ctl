#pragma once

/* Mutex types.  */
enum
{
  mtx_plain     = 0,
  mtx_recursive = 1,
  mtx_timed     = 2
};

#ifdef linux

#include <pthread.h>
#include <stdio.h>

typedef pthread_mutex_t mtx_t;

int mtx_init(mtx_t *__mutex, int __type)
{
    printf("linux\n");
    return pthread_mutex_init(__mutex, NULL);
}

int mtx_lock(mtx_t *__mutex)
{
    return pthread_mutex_lock(__mutex);
}

int mtx_unlock(mtx_t *__mutex)
{
    return pthread_mutex_unlock(__mutex);
}

#endif