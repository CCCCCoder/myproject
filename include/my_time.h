#ifndef __TIME_H__
#define __TIME_H__

#include <sys/time.h>
#include <unistd.h>
#include "my_types.h"

#define US2NS(us)   (us*1000ULL)
#define SEC2MS(sec) (sec*1000ULL)
#define SEC2US(sec) (sec*1000000ULL)
#define SEC2NS(sec) (sec*1000000000ULL)

#define US2MS(us)   (us/1000ULL)
#define MS2SEC(ms)  (ms/1000ULL)
#define US2SEC(us)  (us/1000000ULL)

static inline S64 get_current_time_us(void)
{
    struct timeval tv;
    gettimeofday(&tv , NULL);
    return (SEC2US(tv.tv_sec) + tv.tv_usec);
}

static inline S64 get_current_time_ms(void)
{
    return US2MS(get_current_time_us());
}

#endif

