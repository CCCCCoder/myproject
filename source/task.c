#include "task.h"
#include "my_types.h"
#include "schedule.h"
#include "mylist.h"
#include <pthread.h>
#include "my_log.h"
#include <string.h>
#include "my_memory.h"

S32 taskSubmitToFifo(task_t *pTask)
{
    S32 rc = MY_SUCCESS;
    schedRes_t       *pSchedRes = schedResGet();
    pthread_mutex_t  *pLock     = &pSchedRes->taskLock;
    struct list_head *pTaskList = &pSchedRes->taskList;

    MY_BUG_ON(NULL == pSchedRes);

 	pthread_mutex_lock(pLock);
	list_add_tail(&pTask->node,pTaskList);
	pthread_mutex_unlock(pLock);   
    return rc;
}

task_t *taskCreat(void *pPrivData, TaskOps_t *pOps)
{
    task_t *pTask = 0;

    pTask = (task_t *)my_alloc(sizeof(*pTask));
    if (NULL == pTask) {
        goto end;
    }
    memset((void *)pTask, 0, sizeof(*pTask));
    pTask->pPrevData = pPrivData;
    pTask->pOps = pOps;
    pTask->step = pOps->firstStep;
    pTask->prevStep = pTask->step;

end:
    return pTask;
}

void taskDestroy(task_t *pTask)
{
    if (NULL != pTask) {
        my_free((U64 *)pTask);
    }
    
    return;
}

S32 taskCreatAndSubmit(void *pPrivData, TaskOps_t *pOps)
{
    task_t *pTask = NULL;
    S32     rc    = MY_SUCCESS;

    pTask = taskCreat(pPrivData, pOps);
    if (NULL == pTask) {
        rc = -1;
        goto end;
    }

    rc = taskSubmitToFifo(pTask);
    if (MY_SUCCESS != rc) {
        taskDestroy(pTask);
    }
end:
    return rc;
}
