#include "stdio.h"
#include "task.h"
#include "schedule.h"
#include <pthread.h>
#include "mylist.h"
#include "memory.h"
#include "my_log.h"

schedRes_t gschedRes;

schedRes_t *schedResGet(void)
{
    return &gschedRes;
}

static SchedStatus_t runTaskStep(task_t *pTask)
{
    SchedStatus_t status = SCHED_STATUS_PENDING;

    MY_BUG_ON(pTask->step >= pTask->pOps->maxStep);
    MY_BUG_ON(pTask->pOps->func[pTask->step] == NULL);
    
    if ((pTask->pOps->maxStep > pTask->step) && (pTask->pOps->func[pTask->step] != NULL)) {
        status = pTask->pOps->func[pTask->step](pTask);
    } else {
        status = SCHED_STATUS_FINISH;
    }
    
    return status;
}

static void taskFinish(task_t *pTask)
{
    MY_BUG_ON(NULL == pTask);
    
    if (pTask->pOps->finishFunc != NULL) {
        pTask->pOps->finishFunc(pTask);
    }
    taskDestroy(pTask);
    return;
}

static void runTask(task_t *pTask)
{
    SchedStatus_t     status    = 0;
    
    do {
        status = runTaskStep(pTask);
    }while(SCHED_STATUS_CONTINUE == status);
    
    if (SCHED_STATUS_FINISH == status) {
        taskFinish(pTask);
    }
}

static void *thdSchedule(void *arg)
{
    task_t           *pTask     = NULL;
    schedRes_t       *pSchedRes = schedResGet();
    pthread_mutex_t  *pLock     = &pSchedRes->taskLock;
    struct list_head *pTaskList = &pSchedRes->taskList;
    struct list_head *pTmpList  = NULL;
    struct list_head *pTaskNode = NULL;

    printf("%s\n", __FUNCTION__);

    for (;;) {
        if (!list_empty(pTaskList)) {
            pthread_mutex_lock(pLock);
            list_for_each_safe(pTaskNode, pTmpList, pTaskList) {
                list_del(pTaskNode);
                pthread_mutex_unlock(pLock);
                pTask = list_entry(pTaskNode, task_t, node);
                
                MY_BUG_ON(pTask == NULL);

                runTask(pTask);
                
                pthread_mutex_lock(pLock);
            }
            pthread_mutex_unlock(pLock);
        } else {
            MY_LOG("schedule sleep 1\n");
            sleep(1);
        }
    }
}

S32 schedInit(void)
{
    S32 rc = MY_SUCCESS;
    pthread_t tid;
    schedRes_t *pSchedRes = schedResGet();

    INIT_LIST_HEAD(&pSchedRes->taskList);
    rc = pthread_mutex_init(&pSchedRes->taskLock, NULL);
    if (rc != MY_SUCCESS) {
        MY_BUG();
    }

    rc = pthread_create(&tid, NULL, thdSchedule, NULL);
    if (rc != MY_SUCCESS) {
        MY_BUG(); 
    }    
    return rc;
}

