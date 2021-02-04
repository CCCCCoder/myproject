#ifndef __TASK_H__
#define __TASK_H__

#include "my_types.h"
#include "schedule.h"
#include "mylist.h"
#include "stdio.h"

struct task;

typedef SchedStatus_t (*schedTaskStepFunc)(struct task *pTask);
typedef void (*schedTaskFinishCb)(struct task *pTask);

typedef struct TaskOps{
    U8 maxStep;
    U8 firstStep;
    const char *pName;
    schedTaskFinishCb finishFunc;
    schedTaskStepFunc func[16];
}TaskOps_t;

typedef struct task{
    struct list_head node;
    U32        traceId;
    TaskOps_t *pOps;
    void      *pPrevData;
    U32        step;
    U32        prevStep;
}task_t;

static inline void taskStepSet(task_t *pTask, U8 step)
{
    MY_BUG_ON(pTask == NULL);
    pTask->step = step;
}

task_t *taskCreat(void *pPrivData, TaskOps_t *pOps);
void taskDestroy(task_t *pTask);
S32 taskSubmitToFifo(task_t *pTask);
S32 taskCreatAndSubmit(void *pPrivData, TaskOps_t *pOps);

#endif
