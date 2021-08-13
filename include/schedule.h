#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__

#include "my_types.h"
#include "mylist.h"
#include <pthread.h>
#include "my_log.h"

static inline struct list_head *del_item(struct list_head *pList)
{
    struct list_head *pNode = NULL;

    pNode = pList->next;
    list_del(pNode);
    return pNode;
}
                    
typedef struct schedRes{
    struct list_head taskList;
    pthread_mutex_t  taskLock;
}schedRes_t;

typedef enum SchedStatus{
    SCHED_STATUS_CONTINUE = 1,
    SCHED_STATUS_PENDING,
    SCHED_STATUS_FINISH,
}SchedStatus_t;

S32 schedInit(U32 threadNum);

schedRes_t *schedResGet(void);

#endif
