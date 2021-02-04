#ifndef __MY_LOG_H__
#define __MY_LOG_H__

#include "stdio.h"

typedef struct log_level{
    
}log_level_t;

static inline char *level_2_str()
{

};

#define MY_LOG(fmt, ...) printf("[%s][%u]"fmt, __FUNCTION__, __LINE__ ,##__VA_ARGS__)

#endif

