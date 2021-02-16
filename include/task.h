#ifndef __TASK_H__
#define __TASK_H__

#include "my_types.h"
#include "schedule.h"
#include "mylist.h"
#include "stdio.h"

struct task;

typedef SchedStatus_t (*schedTaskStepFunc)(struct task *pTask);
typedef void (*schedTaskFinishCb)(struct task *pTask);

///< 将prc的结构转换为TaskOps结构
typedef struct TaskOps{
    U8 maxStep;
    U8 firstStep;
    const char *pName;
    schedTaskFinishCb finishFunc;
    schedTaskStepFunc func[16];
}TaskOps_t;

typedef enum TaskType{
	RPC = 0,
	TASK,
}TaskType_e;

///< 用于调度的结构，挂链表调试等
typedef struct SchedHeader{
    struct list_head node;
	TaskType_e type;
    U32        traceId;
}SchedHeader_t;

typedef struct task{
    SchedHeader_t header;
	TaskOps_t    *pTOps;
    void         *pPrevData;
    U32           step;
    U32           prevStep;
}task_t;

static inline void taskStepSet(task_t *pTask, U8 step)
{
    MY_BUG_ON(pTask == NULL);
    pTask->step = step;
}

static inline void taskPrevDataSet(task_t *pTask, void *pData)
{
    pTask->pPrevData = pData;
}
static inline void *getTaskPrevData(task_t *pTask)
{
    return pTask->pPrevData;
}
task_t *taskCreat(void *pPrivData, TaskOps_t *pOps);
void taskDestroy(task_t *pTask);
S32 taskSubmitToFifo(task_t *pTask);
S32 taskCreatAndSubmit(void *pPrivData, TaskOps_t *pOps);

#endif
