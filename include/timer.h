#ifndef __TIMER_H__
#define __TIMER_H__

#include <pthread.h>
#include "rbtree.h"

typedef void (*timer_cb)(void *);

typedef enum timer_state{
    MY_TIMER_STOP    = 0,
    MY_TIMER_RUNNING = 1,
}timer_state_t;

typedef struct mytimer_item{
    struct rb_node  node;
    U64             time_stamp;
    timer_cb        pfunc;
    void           *pData;
}mytimer_item_t;

typedef struct mytimer_cfg{
    struct rb_root   root;
    timer_state_t    state;
    mytimer_item_t  *pItem;
    pthread_mutex_t  timer_lock;
    pthread_mutex_t  timer_mutex;
    pthread_cond_t   timer_Cond;
}mytimer_cfg_t;

mytimer_cfg_t *get_Timer(void);
S32 mytimer_Init(void);
S32 myTimer_item_add(mytimer_item_t *pTimer_item);

#endif

