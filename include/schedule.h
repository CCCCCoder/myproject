#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include "my_types.h"
#include "mylist.h"
#include <pthread.h>

typedef struct schedRes{
    struct list_head taskList;
    pthread_mutex_t  taskLock;
}schedRes_t;

typedef enum SchedStatus{
    SCHED_STATUS_CONTINUE = 1,
    SCHED_STATUS_PENDING,
    SCHED_STATUS_FINISH,
}SchedStatus_t;

S32 schedInit(void);

#endif
